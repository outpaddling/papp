#include <iostream>
#include <vector>

using namespace std;

#include "source_file.h"
#include "macro.h"
#include "statement.h"
#include "papp.h"
#include "error.h"

/***************************************************************************
 * Description:
 *  Read a macro definition and build a macro class object from it.
 *
 * Arguments:
 *  infile:     Input stream of source code containing macro def
 *  argString:  Arguments from .macro directive
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

void macro :: readDef(source_file *source, string argString)

{
    /*
     *  This is not a very efficient implementation.  It would probably
     *  be better to use C strings.  I wrote this using the standard string
     *  class to provide a small but fairly complex C++ code example for
     *  students.  JB
     */
    
    istream     *infile;
    string      line;
    statement   st;
    size_t      endName,
		openParen,
		startArg,
		endArg,
		punct;
    
    // Parse definition line for name and arguments
    //cerr << "New macro: " << argString << '\n';
    endName = argString.find_first_of(" \t(");
    name = argString.substr(0, endName);
    //cerr << "Name: '" << name << "'\n";

    // Skip whitespace between name and '('
    openParen = argString.find('(', endName);
    if ( openParen == string::npos )
	syntaxError(source, "Expected '(': ", &argString);
    
    // Get args from macro def
    startArg = argString.find_first_not_of(" \t", openParen+1);
    while ( startArg != string::npos )
    {
	if ( argString[startArg] == '$' )
	{
	    // Find end of argument variable
	    endArg = argString.find_first_of(" ,)",startArg+1);
	    if ( endArg != string::npos )
	    {
		string  newArg = argString.substr(startArg, endArg - startArg);
		// cerr << "Arg: '" << newArg << "'\n";
		args.push_back(newArg);
	    }

	    // Skip whitespace, if any
	    punct = argString.find_first_of(",)", endArg);
	    if ( punct == string::npos )
		syntaxError(source, "Expected ',' or ')': ", &argString);
	}
	else if ( argString[startArg] == ')' )
	    punct = startArg;   // No arguments
	else
	    syntaxError(source, "Expected '$' or ')': ", &argString);

	// Find start of next arg or end of string
	startArg = argString.find_first_not_of(" \t", punct+1);
    }
    
    // Argument should be followed by a ',' or ')'
    if ( argString[punct] == ',' )
	syntaxError(source, "Expected '$': ", &argString);
    if ( argString[punct] != ')' )
	syntaxError(source, "Malformed argument list: ", &argString);
    
    // Read text
    infile = source->get_infile();
    while ( getline(*infile, line) )
    {
	remove_cr(line);
	
	source->increment_line();
	st.tokenize(source, line);
	//cerr << "opcode: '" << st.get_opcode() << "'\n";
	if ( st.get_opcode() == ".end_macro" )
	    break;
	//cerr << "Macro text: " << line << '\n';
	text.push_back(line);
    }
    
    if ( infile->eof() )
	syntaxError(source, "EOF encountered while defining macro.\n");
}


/***************************************************************************
 * Description:
 *  Expand a macro reference to stream outfile
 *
 * Arguments:
 *  argString:  Arguments to macro reference, begins with '('
 *  outfile:    Address of output stream
 *
 * History: 
 *  Sep 2009    J Bacon
 *  Nov 2010    B Mathwig   Fixed error in move past replaced arg
 ***************************************************************************/

void macro :: expand(source_file *source, const string &argString, ostream *outfile)

{
    vector<string>  inArgs;
    size_t  startSearch,
	    startArg,
	    endArg;
    string  call = get_name() + argString,
	    arg;
    
    (*outfile) << "\t# Macro " << call << '\n';
    
    // Skip whitespace
    startSearch = 0;
    do
    {
	startArg = argString.find_first_not_of(" \t", startSearch + 1);
	if ( startArg == string::npos )
	    syntaxError(source, "Expecting argument: ", &call);
	
	// Any arguments?
	if ( argString[startArg] != ')' )
	{
	    endArg = argString.find_first_of(" \t,)", startArg + 1);
	    if ( endArg == string::npos )
		syntaxError(source, "Malformed argument list: ", &call);
	    arg = argString.substr(startArg, endArg - startArg);
	    //cerr << "Arg: '" << arg << "'\n";
	    inArgs.push_back(arg);
	}
	else
	    endArg = startArg;
	
	startSearch = argString.find_first_of(",)", endArg);
	if ( startSearch == string::npos )
	    syntaxError(source, "Expecting , or ): ", &call);
    }   while ( argString[startSearch] != ')' );
    
    // cerr << inArgs.size() << " " << args.size() << '\n';
    if ( inArgs.size() != args.size() )
	syntaxError(source, "Wrong number of args for macro: ", &call);
    
    // Output macro text in place of reference
    for (size_t line=0; line < text.size(); ++line)
    {
	//cerr << "Outputting macro line " << line << '\n';
	string  out_string = text[line];
	// Replace formal arguments with invocation arguments
	//cerr << "replacing args in " << out_string << '\n';
	for (size_t arg=0; arg < args.size(); ++arg)
	{
	    size_t  startArg = 0;
	    while ( (startArg = out_string.find(args[arg], startArg)) != string::npos )
	    {
		//cerr << startArg << '\n';
		out_string.replace(startArg, args[arg].size(), inArgs[arg]);
		
		// Move past newly replaced argument
		startArg += inArgs[arg].size();
	    }
	}
	(*outfile) << out_string << '\n';
    }
    (*outfile) << "\t# End " << call << "\n\n";
}


/***************************************************************************
 * Description:
 *  Reinitialize a macro object
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

void macro :: clear(void)

{
    name.clear();
    args.clear();
    text.clear();
}

