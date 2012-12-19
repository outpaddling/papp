#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Macros consist of a name, argument list, and any number of lines
// of code.

class macro
{
    public:
	void readDef(source_file *source, string line);
	inline const string &get_name(void)
	{
	    return name;
	}
	void expand(source_file *source, const string &args, ostream *outfile);
	void clear(void);
    private:
	string          name;
	vector<string>  args;
	vector<string>  text;
};

