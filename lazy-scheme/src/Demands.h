#ifndef __DEMANDS_H__
#define __DEMANDS_H__ 1

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/unordered_set.hpp>

namespace Scheme {

namespace Demands {

typedef std::list<std::string> path;
typedef boost::unordered_set<path> rule;
typedef std::unordered_map<std::string, rule> demand_grammar;

typedef std::pair<
		demand_grammar *,   /* program point -> demand */
		demand_grammar *    /* variable name -> demand */
		> expr_demand_grammars;

enum recursion_t {NONE = 0, LEFT = 1, RIGHT = 2, CYCLE = 3};
typedef std::vector<
		std::pair<
		recursion_t,
		std::unordered_set<std::string>
>> partitions;
typedef std::pair<
		demand_grammar *,     /* approx regular grammar */
		partitions * /* partitions of non_terminals*/
		> regular_demand_grammar;

typedef std::unordered_map<std::string, std::unordered_set<std::string>> transitions;
typedef std::unordered_map<std::string, transitions> state_transitions;
typedef std::pair<
		std::unordered_set<std::string>,     /* final  states */
		state_transitions
		> automaton;

const std::string SEPARATOR = "/";
const std::string STATE_SEPARATOR = "+";

const std::string PREFIX_DEMAND = "D";
const std::string SUFFIX_APPROX = "*";
const std::string SUFFIX_FSTATE = "!";
const std::string PREFIX_TRANSFORMER = "T";

/* Terminal symbols in the grammar*/
const std::string E = "e"; // "\u03B5";
const std::string T0 = "0";
const std::string T1 = "1";
const std::string T0b = "0b"; // "0\u0305";
const std::string T1b = "1b"; // "1\u0305";
const std::string TXb = "Xb"; // "X\u0305";
const std::unordered_set<std::string> Terminals({E, T0, T1, T0b, T1b, TXb});



automaton* getNFAsFromRegularGrammar(const demand_grammar* gram, const std::string); //This version of getNFAs shares the automatons created.
automaton * getNFAsFromRegCFG(const regular_demand_grammar *); //This does not share the automatons created
void printNFAToFile(automaton *nfa, std::string filename);
automaton* simplifyNFA(std::unordered_set<std::string> start_states, automaton *nfa);
void printSetofStates(std::unordered_set<std::string> states);
automaton* convertNFAtoDFA(std::unordered_set<std::string> start_states, automaton* nfa, const std::string);
int writeDFAToFile(std::string pgmname, automaton* dfa);

void simplifyCFG(demand_grammar* gram);

demand_grammar * sanitize(demand_grammar *);
regular_demand_grammar * regularize(const demand_grammar *); //Converts arbit CFG into a strongly regular grammar using Mohri-Nederhoff transformation
demand_grammar * solve_functions_and_combine(expr_demand_grammars *);
expr_demand_grammars * merge(expr_demand_grammars *, expr_demand_grammars *);
void minimizeDFA(automaton* dfa, std::unordered_set<std::string> nt);

std::pair<rule,rule> splitLF(path p);
rule expandProd(path p);
std::pair<rule,rule> solveLF(std::string lf, rule demands);



}

}

#endif
