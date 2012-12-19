#include <iostream>
#include <string>
#include <cctype>
#include <sysexits.h>

using namespace std;

#include "source_file.h"
#include "statement.h"
#include "error.h"

/***************************************************************************
 * Description:
 *  Default constructor
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

statement :: statement(void)

{
    type = UNKNOWN;
}


/***************************************************************************
 * Description:
 *  Parse out label and opcode from an assembly language source statement.
 *  Put the rest of the line in private var "argString".
 *
 * Arguments:
 *  line:   String containing entire line of code
 *
 * History: 
 *  Jul 2009    J Bacon
 ***************************************************************************/

void statement :: tokenize(source_file *source, string &line)

{
    size_t  endLabel = 0,
	    startOpcode,
	    endOpcode,
	    startArgs;
    
    // If first non-whitespace is #, it's a comment
    size_t first = line.find_first_not_of(" \t");
    if ( (first == string::npos) || (line[first] == '#') )
    {
	type = COMMENT;
	return;
    }
    
    // If line[0] is non-whitespace, extract label
    if ( (line.length() != 0) && !isspace(line[0]) )
    {
	endLabel = line.find(':');
	if ( endLabel != string::npos )
	    label = line.substr(0,endLabel);
	else
	{
	    // Could be a constant definition (NAME = VAL)
	    // Leave the rest of syntax checking to the assembler.
	    if ( line.find('=') == string::npos )
		syntaxError(source, "Malformed label definition.\n");
	}
    }
    
    // Extract instruction
    startOpcode = line.find_first_not_of(" \t", endLabel+1);
    if ( startOpcode != string::npos )
    {
	endOpcode = line.find_first_of(" \t(", startOpcode);
	opcode = line.substr(startOpcode, endOpcode-startOpcode);
	// cerr << "Found opcode: " << opcode << '\n';
    }
    else
    {
	// Blank line
	if ( label.empty() )
	    type = COMMENT;
	else
	    type = OTHER;
	return;
    }

    // Set enum type for efficient identification
    if ( opcode == ".macro" )
	type = MACRO_DEF;
    else if ( opcode == ".include" )
	type = INCLUDE;
    else if ( opcode == ".const" )
	type = CONST_DEF;
    else
	type = OTHER;
	
    // Set args string to rest of line
    startArgs = line.find_first_not_of(" \t", endOpcode);
    if ( startArgs != string::npos )
    {
	argString = line.substr(startArgs,string::npos);
	if ( argString[0] == '(' )
	    type = MACRO_REF;
	
	// Separate tokens and perform text substitutions
	// while ( (arg = getNextArg()) != string::npos )
	// {
	//     if ( constTable.lookup(arg) )
	//         arg = replaceConst(arg)
	// }
    }
    else
	argString = "";
}

