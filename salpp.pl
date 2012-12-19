#!/usr/bin/perl
#
# Author:       J Bacon
# Title:        SAL Preprocessor
# Description:  
#   Expands macros in a SAL (Simple Assembly Language) program.
#   Macros take the form
#       .macro name [arguments]
#       statements
#       .endmacro
#
# Command-line arguments:
#   
# Return values:
#

while ( $record = <STDIN> )
{
    $statement[0] = "";
    @statement = split(" ",$record);
    print "Statement[0] = ", $statement[0], "\n";
    if ( $statement[0] eq ".macro" )
    {
	print "Defining macro...\n";
	do 
	{
	    $record = <STDIN>;
	    @statement = split(" ", $record);
	    print "'", $statement[0], "'\n";
	}   while ( $statement[0] ne ".endmacro" );
	print "End of macro.\n";
    }
    else
    {
	#print $record;
    }
}

