/***************************************************************************
 * Description:
 *
 * Arguments:
 *
 * Returns:
 *
 * History: 
 *
 ***************************************************************************/

#include <iostream>     // cin, cout, cerr
#include <fstream>
#include <sysexits.h>   // Standardized exit codes EX_OK, EX_USAGE, etc.

using namespace std;

#include "source_file.h"


source_file :: source_file(void)

{
    line_num = 0;
    filename = strdup("Unknown file");
}


source_file :: source_file(const char *new_filename)

{
    ifstream *temp = new ifstream(new_filename);
    if ( temp->fail() )
    {
	cerr << "Error: Could not open " << new_filename << ".\n";
	exit(EX_UNAVAILABLE);
    }
    
    line_num = 0;
    infile = temp;
    filename = strdup(new_filename);
}


source_file :: source_file(istream *stream)

{
    line_num = 0;
    filename = strdup("Unknown file");
    infile = stream;
}


source_file :: ~source_file()

{
    free(filename);
}

