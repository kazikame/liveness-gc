//============================================================================
// Name        : Simulator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <fstream>
#include "gc.h"
//#include "Utils.hpp"
#include "Simulator.h"
#include <unordered_map>
#include "DemandStructure.h"
#include <ctime>
#include <sys/stat.h>

using namespace std;
using namespace Scheme::AST;
using namespace Scheme::Demands;
using namespace Scheme::output;

double gctime=0;
int gcinvoke=0;



unsigned int FileRead( istream & is, vector <char> & buff ) {
    is.read( &buff[0], buff.size() );
    return is.gcount();
}

unsigned int CountLines( const vector <char> & buff, int sz ) {
    int newlines = 0;
    const char * p = &buff[0];
    for ( int i = 0; i < sz; i++ ) {
    	if ( p[i] == '\n' ) {
    		newlines++;
    	}
    }
    return newlines;
}


struct StringListCompare
{
	bool operator () (const list<std::string> & p_lhs, const list<std::string> & p_rhs)
	{
		const size_t lhsLength = p_lhs.size() ;
		const size_t rhsLength = p_rhs.size() ;

		return (lhsLength < rhsLength) ; // compares with the length
	}
} StringListComparer;


Simulator::Simulator(int gctype)
{
	pgm = NULL;
	conscount = 0;
	gc_invoke = 0;
	gc_type = (GCStatus)gctype;
	heap_size = 0;
	cout << "Simulator created successfully" << endl;
	//Do all global initialization here
}

Simulator& Simulator::run(std::string pgmFilePath, int hsize) //This method or the constructor should take other parameters like gctype, heap size etc...
{
	Scheme::SchemeDriver driver;
	cout << "Parsing " << pgmFilePath << endl;
	std::pair<bool, long> parseResult = driver.parse(pgmFilePath.c_str());
	if(!parseResult.first)
	{
		cout << "Error while parsing " << parseResult.second<<endl;
		exit(-1);
	}
	pgm = driver.get_anf_prog();
	std::string pgmname = pgmFilePath.substr(pgmFilePath.find_last_of("/") + 1,
			                                 pgmFilePath.find_first_of(".", pgmFilePath.find_last_of("/")) - pgmFilePath.find_last_of("/") - 1);

	if (gc_type == gc_live)
	{
		double resint = driver.process();
		cout<<"Time taken to build liveness tables: "<<resint/1000000<<"s\n\n";
		// cout<<"Printing Reverse Call Graph:\n ";
		Scheme::AST::printRevCallGraph();
		string pgmdir = pgmFilePath.substr(0, pgmFilePath.find_last_of("/") + 1);
		string progLivePath = pgmdir + pgmname + "_progLiveness";
		string funcLivePath = pgmdir + pgmname + "_funcLiveness";
		Scheme::Demands::printLivenessToFile(progLiveness, progLivePath);
		Scheme::Demands::printLivenessToFile(functionCallDemands, funcLivePath);
		cout<<"Program Liveness printed to:\t"<<progLivePath<<endl;
		cout<<"Function Demand Transfomers printed to:\t"<<funcLivePath<<endl;
		cout<<"\n\n";
	}
	clock_t pstart = clock();
	initialize2(gc_type);
	allocate_heap(hsize * 2);
	empty_environment("main");
	init_gc_stats();
	resultValue r = pgm->evaluate();
	switch(r.type)
	{
	case intType : cout << "ans = " << r.val.intVal << endl;
	break;
	case stringType :  cout << "ans = " << r.val.stringVal << endl;
	break;
	case boolType : cout << "ans = " << r.val.boolVal << endl;
	break;
	case heap : cout << "loc ans = ";
				printval(r.val.addrVal);
				cout<<endl;
	break;
	default : cerr<<"ans = Error!"<<endl;
	}

	cout << "Completed program evaluation" << endl;
	clock_t pend = clock();
	if (gc_type != gc_disable)
		finish_gc_stats();
	//Remove all the parameters and use private variables in the Simulator class
	cout << "Heap total="<<hsize<<" Heap left="<<current_heap()<<" Heap used="<<(hsize - current_heap())<<endl;
	#ifdef GC_ENABLE_STATS
		detail_gc();
	#endif
	// if (gc_type == gc_live)
	// 	cleanup(numkeys-1);

	cout << "GC Invocations="<<gcinvoke<<" GC Time="<<gctime<<endl;
	cout << "Program Execution Time="<<((double(pend - pstart)/CLOCKS_PER_SEC) - gctime)<<" seconds"<<endl;

	return *this;
}

Simulator::~Simulator()
{
}

GCStatus getGCType(string gctypestr)
{
	GCStatus gc_type;
	if (gctypestr == "gc-plain")
		gc_type = gc_plain;
	else if (gctypestr == "gc-live")
		gc_type = gc_live;
	else if (gctypestr == "gc-freq")
		gc_type = gc_freq;
	else
		gc_type = gc_disable;

	return gc_type;
}


int main(int argc, char ** argv)
{
	std::string prog_name = argv[0];
	if (argc < 4)
	{
		cerr << "Insufficient number of arguments " << endl;
		 //printHelp(); //TODO: implement printhelp() method
		 return(EXIT_FAILURE);
	}
	vector<string> cmdline_args;

	string filepath = argv[1];
	long heapsize = stol(argv[2]);
	GCStatus gctype = getGCType(argv[3]);
	cout << "Calling " << prog_name << " with " << filepath << " " << heapsize << " " << gctype << endl;

	Simulator s(gctype);
	try
	{
		s.run(filepath, heapsize); //This method doesn't need to take numkeys as parameter
	}
	catch(exception &e)
	{
		cerr << "Terminating program because of " << e.what() << endl;
	}

	return 0;
}


