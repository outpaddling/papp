#include <vector>
#include "macro.h"

class mactable
{
    public:
	int add(macro &new_macro);
	macro *find(const string &name);
    private:
	vector<macro> macros;
};

