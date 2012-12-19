
class source_file
{
    public:
	source_file(void);
	source_file(const char *new_filename);
	source_file(istream *instream);
	~source_file(void);
	inline istream *get_infile(void)
	{
	    return infile;
	}
	inline char *get_filename(void)
	{
	    return filename;
	}
	inline size_t get_line_num(void)
	{
	    return line_num;
	}
	inline void increment_line(void)
	{
	    ++line_num;
	}
    
    private:
	char    *filename;
	istream *infile;
	size_t  line_num;
};

