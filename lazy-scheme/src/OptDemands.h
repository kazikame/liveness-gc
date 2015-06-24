#ifndef __OPT_DEMANDS_H__
#define __OPT_DEMANDS_H__ 1

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/unordered_set.hpp>

namespace Scheme {

namespace OptDemands {

//#define __DEBUG__GC
//#undef DBG
#ifdef __DEBUG__GC
#define DBG(stmt) stmt
#else
#define DBG(stmt) (void)0
#endif

typedef std::unordered_map<int, std::unordered_set<int>> transitions;
typedef std::unordered_map<int, transitions> state_transitions;
typedef std::pair<
		std::unordered_set<int>,     /* final  states */
		state_transitions
		> automaton;

/* Terminal symbols in the grammar*/
const int E = 0x0;
const int T0 = 0x1;
const int T1 = 0x2;
const int T0b = 0x4;
const int T1b = 0x8;
const int TXb = 0x10;
const std::unordered_set<int> Terminals({E, T0, T1, T0b, T1b, TXb});
automaton* simplifyNFA(std::unordered_set<int> start_states, automaton *nfa);
} // namespace OptDemands

} // namespace Scheme

#endif
