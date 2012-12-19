int     preprocess(source_file *source, ostream *outfile);
string  opcode(string &line);
bool    macro_ref(string &line);
string  getPath(source_file *source, const string &argString);
void    remove_cr(string &line_text);
void    usage(char *argv[]);

