#include <cassert>
#include <utility>
#include <fstream>
#include "SchemeDriver.h"
#include "Utils.hpp"
#include <typeinfo>

Scheme::output::options Scheme::output::global_options;

using namespace std;

using Scheme::output::global_options;

Scheme::SchemeDriver::SchemeDriver()
    : parser(NULL), scanner(NULL), program(NULL), anf_program(NULL)
{}

Scheme::SchemeDriver::~SchemeDriver()
{

}

std::pair<bool, long> Scheme::SchemeDriver::parse(const char * infilename)
 {

	if(infilename == NULL)
	{
		std::cout << "Input file name is null "<< std::endl;
		return std::make_pair(false, -1);
	}

    std::ifstream in_file(infilename);
    if(!in_file.good())
    {
    	std::cout << "Input file is not good" <<std::endl;
    	return std::make_pair(false, -1);
    }

    delete program;
    program = NULL;
    delete anf_program;
    anf_program = NULL;

    delete(scanner);
    delete(parser);

    timeval start, end;
    gettimeofday(&start, NULL);


    scanner = new SchemeScanner(&in_file);
    parser = new SchemeParser(*scanner, *this);


    int res = parser->parse();
    if(res) return std::make_pair(false, -1);

    // program is of type ProgramNode
    //cout<<typeid(program).name()<<'\n';
    anf_program = program->getANF();
    anf_program->doLabel(true);

    // auto revCallGraph = anf_program->makeRevCallGraph();

    // cout<<"\n\nPrinting the reverse call graph:-\n\n";
    // for (auto i : revCallGraph)
    // {
    //     cout<<i.first<<": ";
    //     for (auto j : i.second)
    //     {
    //         cout<<j<<", ";
    //     }
    //     cout<<'\n';
    // }
    // cout<<"\n\n";

    string inputfilename(infilename);
    string inputfilepath = inputfilename.substr(0, inputfilename.find_last_of("/") + 1);
    inputfilename = inputfilename.substr(inputfilename.find_last_of("/") + 1);
    inputfilename = inputfilepath + "anf_" + inputfilename;
    cout << "The anf file is being written to  " << inputfilename << endl;
    ofstream anf_file(inputfilename);
    anf_program->print(anf_file, 0, true, false, Scheme::output::SCHEME);
    anf_file.close();

    gettimeofday(&end, NULL);

    return std::make_pair(true, getElapsedTimeInUS(start, end));
}



long Scheme::SchemeDriver::process()
{
    timeval start, end;
    gettimeofday(&start, NULL);

    program->doLabel(true);
    anf_program->doLabel(true);
    Scheme::AST::revCallGraph = anf_program->makeRevCallGraph();
    program_grammars = anf_program->transformDemand();

    gettimeofday(&end, NULL);

    return getElapsedTimeInUS(start, end);
}

std::string Scheme::SchemeDriver::getErrors() const {
    return error_stream.str();
}

std::ostream & Scheme::SchemeDriver::getErrorStream() {
    return error_stream;
}

void Scheme::SchemeDriver::set_prog(Scheme::AST::ProgramNode * prog) {
    program = prog;
}

// bool Scheme::SchemeDriver::printAST(std::ostream & screen, std::ostream & logger) {
    

//     if(!program) return false;

//     timeval start, end;
//     Scheme::AST::ProgramNode * prog = global_options.ast_anf ? anf_program : program;

//     if(global_options.json_ast != "") {
//         logger << "  => Writing JSON AST to " << global_options.json_ast << " . . . ";
//         if(global_options.json_ast == Scheme::output::piped) {
//             gettimeofday(&start, NULL);
//             prog->print(screen, 0, true, global_options.ast_label, Scheme::output::JSON);
//             gettimeofday(&end, NULL);
//         } else {
//             std::ofstream file(global_options.json_ast);
//             gettimeofday(&start, NULL);
//             prog->print(file, 0, true, global_options.ast_label, Scheme::output::JSON);
//             gettimeofday(&end, NULL);
//             file.close();
//         }
//         logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
//     }

//     if(global_options.plain_ast != "") {
//         logger << "  => Writing PLAIN AST to " << global_options.plain_ast << " . . . ";
//         if(global_options.plain_ast == Scheme::output::piped) {
//             gettimeofday(&start, NULL);
//             prog->print(screen, 0, true, global_options.ast_label, Scheme::output::PLAIN);
//             gettimeofday(&end, NULL);
//         } else {
//             std::ofstream file(global_options.plain_ast);
//             gettimeofday(&start, NULL);
//             prog->print(file, 0, true, global_options.ast_label, Scheme::output::PLAIN);
//             gettimeofday(&end, NULL);
//             file.close();
//         }
//         logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
//     }

//     if(global_options.scheme_ast != "") {
//         logger << "  => Writing SCHEME AST to " << global_options.scheme_ast << " . . . ";
//         if(global_options.scheme_ast == Scheme::output::piped) {
//             gettimeofday(&start, NULL);
//             prog->print(screen, 0, true, global_options.ast_label, Scheme::output::SCHEME);
//             gettimeofday(&end, NULL);
//         } else {
//             std::ofstream file(global_options.scheme_ast);
//             gettimeofday(&start, NULL);
//             prog->print(file, 0, true, global_options.ast_label, Scheme::output::SCHEME);
//             gettimeofday(&end, NULL);
//             file.close();
//         }
//         logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
//     }

//     return true;
// }




