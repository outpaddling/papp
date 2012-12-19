#include <iostream>

using namespace std;

#include "source_file.h"
#include "mactable.h"

/***************************************************************************
 * Description:
 *  Add a new macro to the table
 *
 * Arguments:
 *  newMacro:  macro to be added
 *
 * Returns:
 *  0 on success
 *
 * History: 
 *  Sep 2009    J Bacon
 ***************************************************************************/

int mactable :: add(macro &newMacro)

{
    macros.push_back(newMacro);
    return 0;
}


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

macro *mactable :: find(const string &name)

{
    size_t  c;
    
    for (c = 0; c < macros.size(); ++c)
    {
	//cerr << name << " == " << macros[c].get_name() << "?\n";
	if ( macros[c].get_name() == name )
	    return &macros[c];
    }
    return NULL;
}

