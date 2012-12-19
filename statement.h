// Enum for opcode/directive types for efficient identification

typedef enum
{
    MACRO_DEF,
    MACRO_REF,
    INCLUDE,
    CONST_DEF,
    COMMENT,
    OTHER,
    UNKNOWN
}   stype;

// Separates a line of assembly code into label, opcode/directive
// and argument string.  Argument strings are further parsed by
// more specialized code elsewhere.

class statement
{
    public:
	statement(void);
	void    tokenize(source_file *source, string &line);
	inline stype   get_type(void)
	{
	    return type;
	}
	inline const string & get_opcode(void)
	{
	    return opcode;
	}
	inline const string & get_args(void)
	{
	    return argString;
	}
    private:
	stype   type;
	string  label;
	string  opcode;
	string  argString;
};

