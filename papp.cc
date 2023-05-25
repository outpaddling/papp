#include <iostream>     // cin, cout, cerr
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <sysexits.h>   // Standardized exit codes EX_OK, EX_USAGE, etc.
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>

using namespace std;

#include "source_file.h"
#include "mactable.h"
#include "statement.h"
#include "papp.h"
#include "error.h"

bool    Debug = false;

int     main(int argc,char *argv[])

{
    ifstream infile;
    char    *filename;
    
    switch(argc)
    {
	case    1:
	{
	    // No command line args: process stdin
	    source_file mainSource(&cin);
	    preprocess(&mainSource, &cout);
	    break;
	}
	case    2:
	{
	    // One command line arg: use as source file
	    filename = argv[1];
	    source_file  mainSource(filename);
	    preprocess(&mainSource, &cout);
	    break;
	}
	case    3:
	{
	    if ( strcmp(argv[1], "-d") == 0 )
	    {
		Debug = true;
		// One command line arg: use as source file
		filename = argv[2];
		source_file  mainSource(filename);
		preprocess(&mainSource, &cout);
	    }
	    else
		usage(argv);
	    break;
	}
	default:
	    usage(argv);
	    break;
    }
    return EX_OK;
}


void    usage(char *argv[])

{
    cerr << "Usage: " << argv[0] << " [-d] source-file\n";
    exit(EX_USAGE);
}


/***************************************************************************
 * Description:
 *  Parse an assembly language source file in stream infile for macro
 *  definitions, macro references, and includes.  Output source code with
 *  include files inserted and macros substituted to stream outfile.
 *
 * Arguments:
 *  infile:     Address of input stream
 *  outfile:    Address of output stream
 *
 * Returns:
 *  See sysexits.h
 *
 * History: 
 *  Aug 2009    J Bacon
 ***************************************************************************/

int     preprocess(source_file *source, ostream *outfile)

{
    string          line_text;
    macro           tempMacro,
		    *macPtr;
    static mactable macroTable;
    
    while ( getline(*source->get_infile(), line_text) )
    {
	statement   st;
	string      includeFileName;

	remove_cr(line_text);
	
	source->increment_line();
	st.tokenize(source, line_text);
	switch(st.get_type())
	{
	    case    CONST_DEF:
		// constTable.add()
		break;
	    
	    case    MACRO_DEF:
		// Parse input and create new macro entry
		tempMacro.readDef(source, st.get_args());
		
		// Add new entry to macro table
		macroTable.add(tempMacro);
		
		// tempMacro stays in scope for next round, so reinitialize
		tempMacro.clear();
		break;

	    case    INCLUDE:
	    {
		// Extract filename from .include argument text
		includeFileName = getPath(source, st.get_args());
		
		// Recursively preprocess include file, sending output
		// to same out stream
		source_file includeSource(includeFileName.c_str());
		preprocess(&includeSource, outfile);
		break;
	    }
	    case    MACRO_REF:
		macPtr = macroTable.find(st.get_opcode());
		if ( macPtr != NULL )
		{
		    char    macro_filename[L_tmpnam+1] = "/tmp/pappXXXXX";
		    int     fd;
		    static int  files = 0;
		    
		    // Create a temp file for the macro
		    // Deprecated: tmpnam(macro_filename);
		    if ( (fd = mkstemp(macro_filename)) == -1 )
		    {
			cerr << "Error opening temp file " << macro_filename << '\n';
			exit(EX_CANTCREAT);
		    }
		    ofstream macroFile(macro_filename);
		    close(fd);
		    if ( macroFile.fail() )
		    {
			cerr << "Could not open temp file: " << macro_filename << ": "
			    << strerror(errno) << '\n';
			unlink(macro_filename);
			exit(EX_CANTCREAT);
		    }
		    ++files;
		    if ( Debug )
			cerr << "open files = " << files << '\n';
		    
		    // Expand macro, dumping to a temp file
		    macPtr->expand(source, st.get_args(), &macroFile);
		    macroFile.close();
		    if ( macroFile.fail() )
			cerr << "Failed to close temp file!\n";
		    --files;
		    
		    // Parse the temp file
		    source_file macroSource(macro_filename);
		    preprocess(&macroSource, outfile);
		    unlink(macro_filename);
		    break;
		}
		// Else fall through to source echo
		// Fixme: tokenize should check macro table and not
		// return MACRO_REF unless it really is a macro call.

	    case    COMMENT:
	    case    OTHER:
	    default:
		// Replace with output function that substitutes
		// constants
		(*outfile) << line_text << '\n';
	}
    }
    return EX_OK;
}


/***************************************************************************
 * Description:
 *  Returns true of a string is blank, i.e. is empty or contains only
 *  whitespace characters.
 *
 * Arguments:
 *  line_text:   A string class object
 *
 * History: 
 *  Aug 2009    J Bacon
 ***************************************************************************/

bool    blank(string &line_text)
{
    char    *p;
    
    for (p=(char *)line_text.c_str(); *p != '\0'; ++p)
	if ( !isspace(*p) )
	    return false;
    return true;
}


/***************************************************************************
 * Description:
 *  Extract a filename from between double quotes in argString.
 *
 * Arguments:
 *  argString:  String object containing all argument text from
 *              a directive such as .include.
 *
 * Returns:
 *  A string object containing only the filename.
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

string  getPath(source_file *source, const string &argString)

{
    size_t  startName,
	    endName;
    string  fileName, 
	    pathName;
    struct stat st;
    
    startName = argString.find('"');
    if ( startName == string::npos )
	syntaxError(source, "Expected opening \": ", &argString);
    endName = argString.find('"', ++startName + 1);
    if ( endName == string::npos )
	syntaxError(source, "Expected closing \": ", &argString);
    
    pathName = fileName = argString.substr(startName, endName-startName);
    if ( stat(pathName.c_str(), &st) != 0 )
    {
	pathName = string(PREFIX) + string("/share/papp/include/") + fileName;
    }
    if ( stat(pathName.c_str(), &st) != 0 )
    {
	pathName = string(PREFIX) + string("/share/EPC/include/") + fileName;
    }
    return pathName;
}


void    remove_cr(string &line_text)

{
    size_t          cr_pos;
    
    // Remove carriage return if present.  It interferes with
    // searches by adding an extraneous character before npos.
    cr_pos = line_text.find_first_of('\r');
    if ( cr_pos != string::npos )
    {
	//cerr << "Removing cr at " << cr_pos << '\n';
	line_text.erase(cr_pos, 1);
    }
}

