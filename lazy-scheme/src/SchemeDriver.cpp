#include <cassert>
#include <utility>
#include <fstream>
#include "SchemeDriver.h"
#include "Utils.hpp"


Scheme::output::options Scheme::output::global_options;

using namespace std;

using Scheme::output::global_options;

Scheme::SchemeDriver::SchemeDriver()
    : parser(NULL), scanner(NULL), program(NULL), anf_program(NULL),program_grammars(NULL), combined_grammar(NULL), approx_grammar(NULL)
{}

Scheme::SchemeDriver::~SchemeDriver()
{
	if (program)
		delete(program);
	if (anf_program)
		delete(anf_program);
	if (program_grammars)
	{
		if (program_grammars->second)
			delete(program_grammars->second);
		if (program_grammars->first)
			delete(program_grammars->first);
    delete(program_grammars);
	}
	if (combined_grammar)
		delete(combined_grammar);
	if (approx_grammar)
		delete(approx_grammar);

	if(scanner)
		delete(scanner);
	if (parser)
		delete(parser);
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

    anf_program = program->getANF();
    //anf_program->doLabel(true);

    string inputfilename(infilename);
    string inputfilepath = inputfilename.substr(0, inputfilename.find_last_of("/") + 1);
    inputfilename = inputfilename.substr(inputfilename.find_last_of("/") + 1);
    inputfilename = inputfilepath + "anf_" + inputfilename;
//    cout << "The anf file is being written to  " << inputfilename << endl;
//    ofstream anf_file(inputfilename);
//    anf_program->print(anf_file, 0, true, false, Scheme::output::SCHEME);
//    anf_file.close();
//    cout << "Wrote anf file successfully" << endl;
    gettimeofday(&end, NULL);

    return std::make_pair(true, getElapsedTimeInUS(start, end));
}

long Scheme::SchemeDriver::process()
{
    timeval start, end;
    gettimeofday(&start, NULL);

    program->doLabel(true);
    anf_program->doLabel(true);

    program_grammars = anf_program->transformDemand(Scheme::Demands::rule({{}}));

    combined_grammar = Scheme::Demands::solve_functions_and_combine(program_grammars);
    combined_grammar->emplace("D/all", Scheme::Demands::rule({{Scheme::Demands::T0, "D/all"},
        {Scheme::Demands::T1, "D/all"},
        {Scheme::Demands::E}
    }));
    //Scheme::output::dumpGrammar(cout, combined_grammar);
    //approx_grammar = Scheme::Demands::regularize(combined_grammar);

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

bool Scheme::SchemeDriver::printGrammar(std::ostream & screen, std::ostream & logger) {
    if(!program) return false;

    timeval start, end;
    Scheme::AST::ProgramNode * prog = global_options.ast_anf ? anf_program : program;

    if(global_options.cfg != "") {
        logger << "  => Writing CFG to " << global_options.cfg << " . . . ";
        if(global_options.cfg == Scheme::output::piped) {
            gettimeofday(&start, NULL);
            Scheme::output::dumpGrammar(screen, combined_grammar);
            gettimeofday(&end, NULL);
        } else {
            std::ofstream file(global_options.cfg);
            gettimeofday(&start, NULL);
            Scheme::output::dumpGrammar(file, combined_grammar);
            gettimeofday(&end, NULL);
            file.close();
        }
        logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
    }

    if(global_options.approx_cfg != "") {
        logger << "  => Writing Approx Regular CFG to " << global_options.approx_cfg << " . . . ";
        if(global_options.approx_cfg == Scheme::output::piped) {
            gettimeofday(&start, NULL);
            Scheme::output::dumpGrammar(screen, approx_grammar);
            gettimeofday(&end, NULL);
        } else {
            std::ofstream file(global_options.approx_cfg);
            gettimeofday(&start, NULL);
            Scheme::output::dumpGrammar(file, approx_grammar);
            gettimeofday(&end, NULL);
            file.close();
        }
        logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
    }

    return true;
}

bool Scheme::SchemeDriver::printAST(std::ostream & screen, std::ostream & logger) {
    

    if(!program) return false;

    timeval start, end;
    Scheme::AST::ProgramNode * prog = global_options.ast_anf ? anf_program : program;

    if(global_options.json_ast != "") {
        logger << "  => Writing JSON AST to " << global_options.json_ast << " . . . ";
        if(global_options.json_ast == Scheme::output::piped) {
            gettimeofday(&start, NULL);
            prog->print(screen, 0, true, global_options.ast_label, Scheme::output::JSON);
            gettimeofday(&end, NULL);
        } else {
            std::ofstream file(global_options.json_ast);
            gettimeofday(&start, NULL);
            prog->print(file, 0, true, global_options.ast_label, Scheme::output::JSON);
            gettimeofday(&end, NULL);
            file.close();
        }
        logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
    }

    if(global_options.plain_ast != "") {
        logger << "  => Writing PLAIN AST to " << global_options.plain_ast << " . . . ";
        if(global_options.plain_ast == Scheme::output::piped) {
            gettimeofday(&start, NULL);
            prog->print(screen, 0, true, global_options.ast_label, Scheme::output::PLAIN);
            gettimeofday(&end, NULL);
        } else {
            std::ofstream file(global_options.plain_ast);
            gettimeofday(&start, NULL);
            prog->print(file, 0, true, global_options.ast_label, Scheme::output::PLAIN);
            gettimeofday(&end, NULL);
            file.close();
        }
        logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
    }

    if(global_options.scheme_ast != "") {
        logger << "  => Writing SCHEME AST to " << global_options.scheme_ast << " . . . ";
        if(global_options.scheme_ast == Scheme::output::piped) {
            gettimeofday(&start, NULL);
            prog->print(screen, 0, true, global_options.ast_label, Scheme::output::SCHEME);
            gettimeofday(&end, NULL);
        } else {
            std::ofstream file(global_options.scheme_ast);
            gettimeofday(&start, NULL);
            prog->print(file, 0, true, global_options.ast_label, Scheme::output::SCHEME);
            gettimeofday(&end, NULL);
            file.close();
        }
        logger << getElapsedTimeInUS(start, end) << "us." << std::endl;
    }

    return true;
}




