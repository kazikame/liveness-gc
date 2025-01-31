//============================================================================
// Name        : Simulator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <fstream>
#include "Simulator.h"
#include <unordered_map>
#include "Demands.h"
#include <ctime>
#include <sys/stat.h>
#include<algorithm>
#include<stdint.h>

using namespace std;
using namespace Scheme::AST;
using namespace Scheme::Demands;
using namespace Scheme::output;

double gctime=0;
int gccount=0;
vector<int> heap_map_size;
extern demand_grammar gLivenessData;
extern unordered_map<string, expr_demand_grammars*> localLivenessMap;
extern unordered_map<string, unsigned int> func_heap_cell_reqd;


/*
 * For finding out the ratio of heap cell size
 */
struct live_heap_cell
{
	cell_type typecell;
	union
	{
		struct
		{
			cons *car, *cdr;
			bool can_delete_car;
		}cell;
		int intVal;
		bool boolval;
		std::string* stringVal;
		struct
		{
			ExprNode* expr;
			cons* arg1;
			cons* arg2;
			int16_t arg1_name;
			int16_t arg2_name;
		}closure;
	}val;
	void *forward;
	bool inWHNF;
};
struct reach_heap_cell
{
	cell_type typecell;
	union
	{
		struct
		{
			cons *car, *cdr;
		}cell;
		int intVal;
		bool boolval;
		std::string* stringVal;
		struct
		{
			ExprNode* expr;
			cons* arg1;
			cons* arg2;
		}closure;
	}val;
	void *forward;
	bool inWHNF;
};
//End of declarations.



demand_grammar filter_grammar(demand_grammar gLivenessData, vector<string> filter_criteria)
{
	demand_grammar filtered_grammar;
	for (auto s : filter_criteria)
	{
		filtered_grammar[s] = gLivenessData[s];
	}
	return filtered_grammar;
}

void read_filter_criteria_from_file(string filepath, vector<string> &v)
{
	ifstream infile(filepath);
	while(!infile.eof())
	{
		string s;
		infile >> s;
		v.push_back(s);
	}
	return;
}


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
	//cout << "Simulator created successfully" << endl;
	//Do all global initialization here
}

void print_path (path prod_path, ostream& out)
{

	std::stringstream ss;
	for_each(prod_path.begin(), prod_path.end(), [&ss] (const std::string& s) { ss << "." << s; });
	if (ss.str().size() > 1)
	{
		std::string a = ss.str().substr(1); //Remove the first "." character
		out << a;
	}
}

void write_grammar_to_text_file(demand_grammar *g, string filename)
{
	std::ofstream gram_file(filename);
	//Convert the set into a vector and sort it and print it to a file
	for(auto l:*g)
	{
		vector<list<string> > vec(l.second.begin(), l.second.end());
		sort(vec.begin(), vec.end(), StringListComparer);
		unsigned int i = 0;
		gram_file << l.first << "->";
		for(auto p:vec)
		{
			print_path(p, gram_file);
			++i;
			if (i < vec.size())
				gram_file << "|";
		}
		gram_file << endl;
	}
	gram_file.close();
	return;
}

string outdir = "./output/";

Simulator& Simulator::run(std::string pgmFilePath, int hsize, int numkeys) //This method or the constructor should take other parameters like gctype, heap size etc...
{
	//Do any per program initialization here.
	Scheme::SchemeDriver driver;
//	cout << "Parsing " << pgmFilePath << endl;
	std::pair<bool, long> parseResult = driver.parse(pgmFilePath.c_str());
	double dfa_gen_time = 0;
	if(!parseResult.first)
	{
		cout << "Error while parsing " << parseResult.second<<endl;
		exit(-1);
	}
	pgm = driver.get_anf_prog();
	std::string pgmname = pgmFilePath.substr(pgmFilePath.find_last_of("/") + 1,
			                                 pgmFilePath.find_first_of(".", pgmFilePath.find_last_of("/")) - pgmFilePath.find_last_of("/") - 1);
///////////////////////////////////////////////////////////////////////
	bool filesCached = true;
	struct stat buffer;
    mkdir(outdir.c_str(), 0755);
    mkdir((outdir+pgmname).c_str(), 0755);
	bool state_map_file =  (stat ((outdir + pgmname + "/fsmdump-" + pgmname + "-state-map").c_str(), &buffer) == 0);
	bool state_transition_file =  (stat ((outdir + pgmname + "/fsmdump-" + pgmname + "-state-transition-table").c_str(), &buffer) == 0);
	filesCached = state_map_file && state_transition_file;

	if (gc_type == gc_live && !filesCached )
	{
		clock_t dfa_gen_start = clock();
		
		//Instead of driver.process returning an integer why can't it return the grammar?
		driver.process();
		//Use pgm->liveness_data as the final grammar

		gLivenessData.insert(pgm->liveness_data.begin(), pgm->liveness_data.end()) ;
		gLivenessData[PREFIX_DEMAND + SEPARATOR + "all" ] = rule({{"0", PREFIX_DEMAND + SEPARATOR + "all" }, {"1", PREFIX_DEMAND + SEPARATOR + "all" },{E}});

		//Add a dummy pgmpt with D/all liveness to handle liveness for cons cells on the print stack
		rule symbolic_demand = rule({{ PREFIX_DEMAND + SEPARATOR + "all" }});
		gLivenessData["L/-1/c"] = symbolic_demand;


		//cout << "program name " << pgmname << endl;
		write_grammar_to_text_file(&gLivenessData, outdir + pgmname + "/program-cfg.txt");



		/*int i = 1;
		for(auto expr_grmr:localLivenessMap)
		{
		    write_grammar_to_text_file(expr_grmr.second->second, "testgrammar.txt" + to_string(i));
		    ++i;
		}*/


		//Simplify grammar
		simplifyCFG(&gLivenessData);
		write_grammar_to_text_file(&gLivenessData, outdir + pgmname + "/simplified-program-cfg.txt");
		//Convert CFG to strongly regular grammar
		regular_demand_grammar *reg = regularize(&gLivenessData);
//		cout << "Converted CFG into strongly regular grammar" << endl;
		gLivenessData.clear(); //clear the original grammar
		gLivenessData.swap(*(reg->first));
		write_grammar_to_text_file(&gLivenessData, outdir + pgmname + "/program-reg.txt");
		Scheme::Demands::sanitize(&gLivenessData); //Remove empty productions
//		std::cout << "Sanitized the regular grammar"<<std::endl;


		automaton *nfa = Scheme::Demands::getNFAsFromRegularGrammar(&gLivenessData, pgmname);
		Scheme::Demands::printNFAToFile(nfa, outdir + pgmname + "/program-nfa.txt");
//		exit(1);
		std::unordered_set<std::string> start_states;
		for (auto nt:gLivenessData)
			start_states.insert(nt.first);
		nfa = Scheme::Demands::simplifyNFA(start_states, nfa);
		Scheme::Demands::printNFAToFile(nfa, outdir + pgmname + "/program-simplified-nfa.txt");

		automaton* dfa = convertNFAtoDFA(start_states, nfa, pgmname);
		Scheme::Demands::printNFAToFile(dfa, outdir + pgmname + "/program-dfa.txt");


		//While converting DFA to 2D array and writing to file, set the associations for the (prog_pt, varname) instead of e-paths
		std::map<std::string, std::unordered_set<std::string>> label_set_map;
		for (auto p : *(pgm->progpt_map))
		{
			label_set_map[p.first] = p.second->label_set;
		}
		numkeys = Scheme::Demands::writeDFAToFile(pgmname, dfa, label_set_map);

		Scheme::Demands::write_func_heap_requirement(pgmname, func_heap_cell_reqd);

		dfa_gen_time = (double(clock() - dfa_gen_start)/CLOCKS_PER_SEC);

		//Scheme::Demands::minimizeDFA(dfa, start_states);
	}
	else if (gc_type == gc_live) //TODO : Make it compile conditionally only when we are dumping graphviz files
	{
		std::ofstream file(outdir + "anf_prog.txt");
		driver.get_anf_prog()->print(file, 0, true, true, Scheme::output::SCHEME);
		file.close();

//		cout <<"Reading data from cached files "<<endl;
		const int SZ = 1024 * 1024;
		std::vector<char> buff(SZ, '\0');
		ifstream ifs( outdir + pgmname + "/fsmdump-" + pgmname + "-state-map" );
		int n = 0;
		while( int cc = FileRead( ifs, buff ) )
		{
			n += CountLines( buff, cc );
		}
		numkeys = n + 1;
	}

///////////////////////////////////////////////////////////////////////
//	cout << "Num of states " << numkeys << endl;
	//Do the initialization of the dfa state map and transition table
	//initialize(pgmFilePath, numkeys, gc_type);
	clock_t pstart = clock();
	initialize(pgmname, numkeys, gc_type);
	allocate_heap(hsize * 2);



	empty_environment("psuedo-main");
	init_gc_stats();

	Scheme::AST::cons* r = pgm->evaluate();

	//remove the psuedo-main environment added
	delete_environment();

	std::string filepath = "diable.txt";
	if (gc_type == gc_live)
		filepath = outdir + "live.txt";
	else if (gc_type == gc_plain)
		filepath = outdir + "plain.txt";
	else if (gc_type == gc_freq)
		filepath = outdir + "freq.txt";


	std::ofstream outfile(filepath, ios::out);

	printval(r, outfile);
	outfile << endl;


//	cout << "Completed program evaluation" << endl;
	clock_t pend = clock();
	if (gc_type != gc_disable)
		finish_gc_stats();
	//Remove all the parameters and use private variables in the Simulator class
	cout << "Heap total="<<hsize<<" Heap left="<<current_heap()<<" Heap used="<<(hsize - current_heap())<<endl;
	detail_gc();
	if (gc_type == gc_live)
	{
		cleanup(numkeys-1);
		// cout << "Maximum extra memory required during LGC " << *std::max_element(heap_map_size.begin(), heap_map_size.end()) << endl;
		//cout << "size of rgc heap cell = " << sizeof(reach_heap_cell) << endl;
		//cout << "size of lgc heap cell = " << sizeof(live_heap_cell) << endl;
	}

	cout << "GC Invocations="<<gccount<<" GC Time="<<gctime<<endl;
	cout << "DFA Gen Time="<<dfa_gen_time<<endl;
	cout << "Num of DFA states="<<numkeys<<endl;
	cout << "Program Execution Time="<<((double(pend - pstart)/CLOCKS_PER_SEC) - gctime)<<" seconds"<<endl;

	return *this;
}

Simulator::~Simulator()
{
}

clock_tick gc_freq_threshold = 20; // some random value
GCStatus getGCType(string gctypestr)
{
	GCStatus gc_type;
	std::string gc_freq_prefix("gc-freq=");
	if (!gctypestr.compare(0, gc_freq_prefix.size(), gc_freq_prefix)) {
		gc_freq_threshold = atoi(gctypestr.substr(gc_freq_prefix.size()).c_str());
		gc_type = gc_freq;
	}
	else if (gctypestr == "gc-freq")
		gc_type = gc_freq;
	else if (gctypestr == "gc-plain")
		gc_type = gc_plain;
	else if (gctypestr == "gc-live")
		gc_type = gc_live;
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
		cerr << "USAGE: " << endl;
		cerr << "\t" << argv[0] << "  <filename>  <heapsize>  <gc-type>" << endl;
		cerr << endl;
		cerr << "\t<gc-type> is one of: gc-live, gc-plain, gc-freq[=<threshold>]" << endl;
		cerr << endl;
		//printHelp(); //TODO: implement printhelp() method
		return(EXIT_FAILURE);
	}
	vector<string> cmdline_args;
	int optind = 1;

	string filepath = argv[1];
	long heapsize = stol(argv[2]);
	GCStatus gctype = getGCType(argv[3]);
	//	cout << "Calling " << prog_name << " with " << filepath << " " << heapsize << " " << gctype;
	if (gctype == gc_freq) cout << "=" << gc_freq_threshold;
	cout << endl;

	Simulator s(gctype);
	try
	{
		s.run(filepath, heapsize, 0); //This method doesn't need to take numkeys as parameter
	}
	catch(exception &e)
	{
		cerr << "Terminating program because of " << e.what() << endl;
	}

	return 0;
}


