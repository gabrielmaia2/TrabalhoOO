#ifndef PROGRAM_H
#define PROGRAM_H

class Program final
{
private:
	Program();
	
	Program(const Program &) = delete;
	Program(Program &&) = delete;
	
	void operator=(const Program &) = delete;
	void operator=(Program &&) = delete;
public:
	~Program();
	
	static Program &instance();
};

#endif
