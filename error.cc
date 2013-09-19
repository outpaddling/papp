#include <iostream>
#include <string>
#include <sysexits.h>
#include <cstdlib>

using namespace std;

#include "source_file.h"

void    errorMsg(source_file *source, const char *msg)

{
    cerr << "Syntax error: " << source->get_filename() << 
	", line " << source->get_line_num() << ": " << msg;
}


/***************************************************************************
 * Description:
 *  Print a syntax error message and terminate the process.
 *
 * Arguments:
 *  msg:    Main text of message.
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

void    syntaxError(source_file *source, const char *msg)

{
    errorMsg(source, msg);
    cerr << '\n';
    exit(EX_DATAERR);
}


/***************************************************************************
 * Description:
 *  Print a syntax error message and terminate the process.
 *
 * Arguments:
 *  msg:    Main text of message.
 *  code:   Additional text, usually code that triggered the error
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

void    syntaxError(source_file *source, const char *msg, const string *code)

{
    errorMsg(source, msg);
    cerr << *code << '\n';
    exit(EX_DATAERR);
}

