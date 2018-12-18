#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include<boost/tokenizer.hpp>
//#include <boost/filesystem.hpp>

#include "OptDemands.h"
//#define __DEBUG__GC
#undef __DEBUG__GC
#ifdef __DEBUG__GC
#define DBG(stmt) stmt
#else
#define DBG(stmt) (void)0
#endif

using namespace Scheme::OptDemands;

static bool removeEpsilonEdges(std::unordered_set<int> start_states,
                               automaton *nfa);
static bool replaceEdgesWithEpsilonEdge(automaton* nfa,
                                        std::pair<int, transitions> ts,
                                        state_transitions &trans,
                                        const int sym1, const int sym2);

static void removeUnreachableStates(std::unordered_set<int> reachable_states,
                                    automaton* nfa);
static bool barEdgeSimplification(automaton *nfa);
static std::unordered_set<int> epsilonClosure(int state, automaton *nfa);
static bool addTransitionsToNode(int src, int des, automaton *nfa);

automaton* Scheme::OptDemands::simplifyNFA(
    std::unordered_set<int> start_states,
    automaton *nfa)
{
	bool changed = true;
	int i = 1;
	while(changed)
	{
		DBG(std::cerr << "Starting " << i <<"th round of simplification\n");
		changed = removeEpsilonEdges(start_states, nfa);
		changed |= barEdgeSimplification(nfa);
        i++;
		DBG(std::cerr << "Completed " << i << " rounds of simplification\n");
        //if (i == 3) exit(0);
	}
	return nfa;
}

static
bool removeEpsilonEdges(std::unordered_set<int> start_states,
                        automaton *nfa)
{
	bool changed = false;
	std::unordered_set<int> reachable_states;
	int i = 0;
    std::ostream &dout = std::cerr;
	DBG(dout << "starting removal of epsilon edges" << std::endl);
	for(auto non_terminal: start_states)
	{
		++i;
		DBG(dout << "Processing NT " << non_terminal << std::endl);
		std::stack<int> states;
		std::unordered_set<int> processed;
		states.push(non_terminal);

		while(!states.empty())
		{
			int curr_state = states.top();
			states.pop();//Remove it from the stack
            //Add it to the list of states already processed.
			processed.insert(curr_state);
			reachable_states.insert(curr_state);
			auto eps_closure = epsilonClosure(curr_state, nfa);
			for (auto s:eps_closure)
			{
				if (nfa->second.find(s) != nfa->second.end())
				{
					DBG(dout << "Processing EPS-edge for " << s << std::endl);
					changed |= addTransitionsToNode(curr_state, s, nfa);
				}
				//If any of the states in the epsilon closure
				//is a final state. Set the current state
				//also as a final state
				if (nfa->first.find(s) != nfa->first.end() &&
                    nfa->first.find(curr_state) == nfa->first.end())
				{
					DBG(dout << "Setting " << curr_state << " as final\n");
					nfa->first.insert(curr_state);
					changed = true;
				}
			}
			//remove all epsilon transitions from the
			//non_terminal This check is required to see if
			//the current state has any transitions otherwise
			//there won't be an entry in the map.
            
			if (nfa->second.find(curr_state) != nfa->second.end())
			{
				auto &t = nfa->second.at(curr_state);
                //DO NOT ERASE THE epsilon EDGE AS THE BAR
				// EDGE REMOVAL CODE WILL KEEP ADDING IT BACK
				// t.erase(E);
				for(auto ts:nfa->second.at(curr_state))
				{
					if(ts.first != E)
						for(auto st:ts.second)
							if(processed.find(st) == processed.end())
							{
								states.push(st);
								reachable_states.insert(st);
							}
				}
			}
		}
		DBG(dout << "Completed epsilon removal for " << non_terminal
            << std::endl);
	}
	removeUnreachableStates(reachable_states, nfa);
	return changed;
}

static
void removeUnreachableStates(std::unordered_set<int> reachable_states,
                             automaton* nfa)
{
	std::vector<int> delete_list;
	for(auto s:nfa->second)
	{
		if (reachable_states.find(s.first) == reachable_states.end())
		{
			delete_list.push_back(s.first);
		}
	}
    
	for(auto s:delete_list)
		nfa->second.erase(s);
}

static
bool barEdgeSimplification(automaton *nfa)
{
	bool changed = false;
	state_transitions &trans = nfa->second;
	DBG(std::cerr << "Bar edge simplification started"<< std::endl);
	for(auto ts : trans)
	{
        changed |= replaceEdgesWithEpsilonEdge(nfa, ts, trans, T0b, T0);
        changed |= replaceEdgesWithEpsilonEdge(nfa, ts, trans, T1b, T1);
	}
	DBG(std::cerr << "Bar edge simplification completed"<< std::endl);
	return changed;
}

bool replaceEdgesWithEpsilonEdge(automaton* nfa,
                                 std::pair<int, transitions> ts,
                                 state_transitions &trans,
                                 const int sym1, const int sym2)
{
	bool changed = false;
	auto &src = ts.first;
	auto &tmap = ts.second;

	if (tmap.find(sym1) == tmap.end() || tmap[sym1].empty())
		return changed;

	std::unordered_set<int> next = tmap[sym1];
	for(auto s:next)
	{
		if ((trans.find(s) == trans.end()) ||
            (trans.at(s).find(sym2) == trans.at(s).end()))
			continue;

		std::unordered_set<int> des = trans.at(s)[sym2];
		for(auto d:des)
		{
			if(trans.at(src)[E].find(d) != trans.at(src)[E].end())
				continue;

			//Add only if the edge is not present
			trans.at(src)[E].insert(d);

			//TODO: SHOULD I REMOVE THIS CODE TO ADD src TO FINAL STATE LIST
			if (nfa->first.find(d) != nfa->first.end() &&
                nfa->first.find(src) == nfa->first.end())
			{
				//Insert the src as a final state if d is a final state.
				nfa->first.insert(src);
			}
			changed = true;
		}
	}
	return changed;
}

static std::unordered_set<int> epsilonClosure(int state, automaton *nfa)
{
	std::unordered_set<int> eps_closure;
	eps_closure.insert(state);
	std::queue<int> state_queue;
	state_queue.push(state);

	while(!state_queue.empty())
	{
		std::unordered_set<int> nexts;
		while(!state_queue.empty())
		{
			auto node = state_queue.front();
			state_queue.pop();
			if(nfa->second.find(node) != nfa->second.end())
			{
				std::unordered_set<int> temp = nfa->second.at(node)[E];
				nexts.insert(temp.begin(), temp.end());
			}
		}

		std::queue<int> temp_queue;
		for(auto st:nexts)
		{
			if(eps_closure.find(st) == eps_closure.end())
				temp_queue.push(st);
		}
		state_queue.swap(temp_queue);
		eps_closure.insert(nexts.begin(), nexts.end());
	}

	//If a final state is part of an epsilon closure, then all states in the
	//epsilon closure path should be marked final state
	bool hasfinalState = false;
	for(auto st:eps_closure)
		if(nfa->first.find(st) != nfa->first.end())
		{
			hasfinalState = true;
			break;
		}
	if (hasfinalState)
	{
		nfa->first.insert(state);
	}

	return eps_closure;
}

static
bool addTransitionsToNode(int src, int des, automaton *nfa)
{
	bool changed = false;
	//Add all transitions other than epsilon transitions from the des node to src node
	for(auto t:nfa->second.at(des))
	{
		if (E == t.first)
            continue;
        
        for(auto s:t.second)
        {
            if (nfa->second.at(src)[t.first].find(s) != nfa->second.at(src)[t.first].end())
                continue;
            nfa->second.at(src)[t.first].insert(s);
            changed = true;
        }
    }
	return changed;
}
    
