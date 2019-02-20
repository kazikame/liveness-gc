#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__ 1
#include <string.h>
#include "SchemeDriver.h"


class Simulator
{
private:
	Scheme::AST::ProgramNode* pgm;
	int conscount;
	int gc_invoke;
	GCStatus gc_type;
	int heap_size;
	//time gcstart;
	//time gcend;

public:
	Simulator(int gctype);
	Simulator& run(std::string pgmFilePath, int hsize); //This method or the constructor should take other parameters like gctype, heap size etc...
	~Simulator();
};

#endif
