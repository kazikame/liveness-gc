#include <fstream>
#include <cassert>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include<boost/tokenizer.hpp>
#include <ctime>
//#include <boost/filesystem.hpp>

#include "Demands.h"
#include "OptDemands.h"

using namespace Scheme::Demands;


//#define __DEBUG__GC
#undef __DEBUG__GC
#ifdef __DEBUG__GC
#define DBG(stmt) stmt
#else
#define DBG(stmt) (void)0
#endif

extern demand_grammar gLivenessData;
extern std::string outdir;

static unsigned long state_counter = 0;

std::map<std::string, bool> nfa_changed;
std::map<std::string, std::unordered_set<std::string> > eps_closure_map;

static std::unordered_set<std::string> marked_states;
static std::unordered_map<std::string, automaton> nfa_map;
typedef std::unordered_set<std::string> partition;

partition reachable_states(std::string nt, automaton* nfa);
void print_path(path);
void print_fa_to_graphviz_file(std::string pgmname, std::string non_terminal, automaton* nfa);
void print_split_lf(std::pair<rule,rule> res, std::string trans_demand_key);



std::string createNewState(const std::string & old, const std::string & alpha = "")
{

    return old + (alpha.length() ? STATE_SEPARATOR + alpha : "") + "("
           + std::to_string(state_counter++) + ")";
}


automaton * determinize(const automaton *)

{
	//TODO : Implement this method
    auto dfa = new automaton;
    return dfa;
}


void make_fa(const std::string & start,
		const path & alpha,
		const std::string & end,
		demand_grammar * gram,
		partitions * parts,
		std::unordered_map<std::string, unsigned> * rev_parts,
		state_transitions * trans)
{
    assert(alpha.size());
    if(alpha.size() > 1)
    {
        auto temp_state = createNewState(start, alpha.front());
        auto temp_alpha = path({{alpha.front()}});
        (*trans)[temp_state];
        make_fa(start, temp_alpha, temp_state, gram, parts, rev_parts, trans);

        temp_alpha = alpha;
        temp_alpha.pop_front();
        make_fa(temp_state, temp_alpha, end, gram, parts, rev_parts, trans);
        return;
    }

    auto symbol = alpha.front();
    if(symbol == E || gram->find(symbol) == gram->end())
    {
        trans->at(start)[symbol].insert(end);
        return;
    }

    auto nt_part = rev_parts->at(symbol);
    auto part = parts->at(nt_part);

    if(part.first == NONE)
    {
        for(auto & prod_path : gram->at(*(part.second.begin())))
            make_fa(start, prod_path, end, gram, parts, rev_parts, trans);
        return;
    }

    std::unordered_map<std::string, std::string> nt_map;
    for(auto & nt : part.second)
    {
        auto nt_state = createNewState(nt);
        (*trans)[nt_state];
        state_counter--;
    }
    state_counter++;

    switch(part.first)
    {
        case CYCLE:
            for(auto & non_terminal : part.second)
                for(auto & prod_path : gram->at(non_terminal))
                    if(part.second.find(prod_path.front()) == part.second.end())
                        make_fa(start, prod_path, end, gram, parts, rev_parts, trans);
            break;

        case RIGHT:
            for(auto & non_terminal : part.second)
            {
                for(auto & prod_path : gram->at(non_terminal)) {
                    auto & right_symbol = prod_path.back();
                    if(part.second.find(right_symbol) != part.second.end())
                    {
                        auto temp_alpha = prod_path;
                        temp_alpha.pop_back();
                        if(temp_alpha.size())
                            make_fa(nt_map.at(non_terminal), temp_alpha, nt_map.at(right_symbol),
                                    gram, parts, rev_parts, trans);
                    }
                    else
                        make_fa(nt_map.at(non_terminal), prod_path, end,
                                gram, parts, rev_parts, trans);
                }
            }
            trans->at(start)[E].insert(nt_map.at(symbol));
            break;

        case LEFT:
            for(auto & non_terminal : part.second)
            {
                for(auto & prod_path : gram->at(non_terminal)) {
                    auto & left_symbol = prod_path.front();
                    if(part.second.find(left_symbol) != part.second.end()) {
                        auto temp_alpha = prod_path;
                        temp_alpha.pop_front();
                        if(temp_alpha.size())
                            make_fa(nt_map.at(left_symbol), temp_alpha, nt_map.at(non_terminal),
                                    gram, parts, rev_parts, trans);
                    }
                    else
                        make_fa(start, prod_path, nt_map.at(non_terminal), gram, parts, rev_parts, trans);
                }
            }
            trans->at(nt_map.at(symbol))[E].insert(end);
            break;
        case NONE:
        	break;
    }
}

void combineEpsilonEdgesFrom(const std::string & start, automaton * nfa)
{
    if(marked_states.find(start) != marked_states.end())    return;
    marked_states.insert(start);

    auto & finals = nfa->first;
    auto & trans = nfa->second;

    if(trans.find(start) == trans.end())    return;

    if(trans.at(start).find(E) != trans.at(start).end()) {
        std::list<std::string> epsilon_closure;
        std::unordered_set<std::string> local_marked_states;

        for(auto & token : trans.at(start).at(E))
            epsilon_closure.push_back(token);

        while(!epsilon_closure.empty()) {
            auto & token = epsilon_closure.front();

            if(local_marked_states.find(token) == local_marked_states.end()) {
                local_marked_states.insert(token);

                if(trans.find(token) != trans.end()) {
                    for(auto & tran : trans.at(token)) {
                        if(tran.first == E)
                            for(auto & next : tran.second)
                                epsilon_closure.push_back(next);
                        else
                            for(auto & next : tran.second)
                                trans.at(start)[tran.first].insert(next);
                    }
                }

                if(finals.find(token) != finals.end())
                    finals.insert(start);
            }

            epsilon_closure.pop_front();
        }

        trans.at(start).erase(E);
    }

    for(auto & tran : trans.at(start))
        if(tran.first != E)
            for(auto & next : tran.second)
                combineEpsilonEdgesFrom(next, nfa);
}

void markStatesNotReachingFinalFrom(const std::string & start, automaton * nfa,
                                    std::unordered_set<std::string> & marked,
                                    std::unordered_set<std::string> & visited)
{

}

void removeDeadStatesFrom(const std::string & start, automaton * nfa)
{
    std::unordered_map<std::string, std::unordered_set<std::string>> reverseEdges;
    for(auto & state : nfa->first)
        reverseEdges[state];
    for(auto & state : nfa->second)
        reverseEdges[state.first];

    std::list<std::tuple<std::string, std::string>> check_list;
    std::unordered_set<std::string> start_reachable_states;
    std::unordered_set<std::string> final_reachable_states;
    std::unordered_set<std::string> visited_states;

    std::string state, parent;
    check_list.push_back(make_pair(start, ""));
    while(!check_list.empty()) {
        std::tie(state, parent) = check_list.back();

        start_reachable_states.insert(state);
        if(nfa->first.find(state) != nfa->first.end())
            final_reachable_states.insert(state);
        if(final_reachable_states.find(state) != final_reachable_states.end())
            final_reachable_states.insert(parent);

        if(visited_states.find(state) == visited_states.end()) {
            visited_states.insert(state);

            if(nfa->second.find(state) != nfa->second.end())
                for(auto & tran : nfa->second.at(state))
                    for(auto & next : tran.second)
                        check_list.push_back(make_pair(next, state));
            else
                check_list.pop_back();
        } else
            check_list.pop_back();
    }

    for(auto iter = start_reachable_states.begin(); iter != start_reachable_states.end(); )
        if(final_reachable_states.find(*iter) == final_reachable_states.end())
            iter = start_reachable_states.erase(iter);
        else ++iter;

    for(auto iter = nfa->second.begin(); iter != nfa->second.end(); ) {
        if(start_reachable_states.find(iter->first) == start_reachable_states.end()) {
            iter = nfa->second.erase(iter);
        } else {
            for(auto tran_iter = iter->second.begin(); tran_iter != iter->second.end(); ) {
                for(auto next_iter = tran_iter->second.begin(); next_iter != tran_iter->second.end() ; ) {
                    if(start_reachable_states.find(*next_iter) == start_reachable_states.end())
                        next_iter = tran_iter->second.erase(next_iter);
                    else ++next_iter;
                }
                if(tran_iter->second.size())    ++tran_iter;
                else                            tran_iter = iter->second.erase(tran_iter);
            }
            if(iter->second.size())             ++iter;
            else                                iter = nfa->second.erase(iter);
        }
    }

    for(auto iter = nfa->first.begin(); iter != nfa->first.end(); )
        if(start_reachable_states.find(*iter) == start_reachable_states.end())
            iter = nfa->first.erase(iter);
        else ++iter;
}

void expand_eps_closure(std::unordered_set<std::string> & eps_closure, const automaton * nfa)
{
    std::unordered_set<std::string> visited;
    std::list<std::string> check_list;

    for(auto & state : eps_closure) check_list.push_back(state);

    while(!check_list.empty()) {
        auto & state = check_list.front();

        if(visited.find(state) == visited.end()) {
            visited.insert(state);

            if(nfa->second.find(state) == nfa->second.end()
               || nfa->second.at(state).find(E) == nfa->second.at(state).end()) continue;

            for(auto & next : nfa->second.at(state).at(E)) {
                check_list.push_back(next);
                eps_closure.insert(next);
            }
        }

        check_list.pop_front();
    }
}

void simplifyWithoutEpsilonEdges(const std::string & start, automaton * nfa) {
    auto & trans = nfa->second;
    bool change = true;

    while(change) {
        change = false;

        std::list<std::string> bag({start});
        std::unordered_set<std::string> marked;
        std::unordered_set<std::string> visited;

        while(!bag.empty()) {
            auto & state = bag.front();

            if(visited.find(state) == visited.end()) {
                visited.insert(state);

                if(trans.find(state) == trans.end()) continue;

                for(auto & sym_trans : trans.at(state)) {
                    if(sym_trans.first == E && marked.find(state) != marked.end())
                        continue;

                    for(auto & state : sym_trans.second)
                        bag.push_back(state);

                    if(sym_trans.first != T0b && sym_trans.first != T1b)
                        continue;

                    std::unordered_set<std::string> bar_eps_closure;
                    bar_eps_closure.insert(sym_trans.second.begin(), sym_trans.second.end());
                    expand_eps_closure(bar_eps_closure, nfa);
                    marked.insert(bar_eps_closure.begin(), bar_eps_closure.end());

                    auto next_sym = (sym_trans.first == T0b ? T0 : T1);
                    std::unordered_set<std::string> targets;
                    for(auto & next : bar_eps_closure)
                        if(trans.find(next) != trans.end())
                            if(trans.at(next).find(next_sym) != trans.at(next).end())
                                targets.insert(trans.at(next).at(next_sym).begin(), trans.at(next).at(next_sym).end());

                    auto size = trans.at(state)["e"].size();
                    trans.at(state).at("e").insert(targets.begin(), targets.end());
                    change = change || (size != trans.at(state).at(E).size());
                }
            }

            bag.pop_front();
        }
    }

    for(auto & non_terminal : nfa->second) {
        non_terminal.second.erase(T0b);
        non_terminal.second.erase(T1b);
    }
}

void removeXbEdgesFrom(automaton * nfa)
{

	//This is not correct. Need to re-write.
	std::string prev_nt = E;
	for(auto & non_terminal : nfa->second)
    {
		auto Xb_transition = non_terminal.second.find(TXb);
//
//        if(Xb_transition != non_terminal.second.end())
//        {
//        	//This should handle cases like 0b2 or 1b2 is encountered.
//        	//In such cases there can be no path hence do not add an epsilon edge
//            if (prev_nt == T0b || prev_nt == T1b)
//            {
//            	non_terminal.second.erase(TXb);
//            }
//            else
//            {
//            	//For all other cases add an epsilon edge instead of the the 2 edge
//
//            	auto dest_states = Xb_transition->second;
//            	non_terminal.second.erase(TXb);
//            	(non_terminal.second[E]).insert(dest_states.begin(), dest_states.end());
//            	//If the epsilon edge takes it to a final state, mark the current state as final
//            	nfa->first.insert(non_terminal.first);
//            }
//
//        }
		//Currently add an epsilon edge unconditionally, and make the current state a final state
		  if(Xb_transition != non_terminal.second.end())
		  {
			  DBG(std::cerr << "Replacing Xb by epsilon edge for " << non_terminal.first << std::endl);
			  auto dest_states = Xb_transition->second;
			  (non_terminal.second[E]).insert(dest_states.begin(), dest_states.end());
			  non_terminal.second.erase(TXb);
			  nfa->first.insert(non_terminal.first);
		  }
    }

}

bool isTerminal(std::string sym)
{
	//See if we can use the set Terminals to return true in case of a Terminal symbol
	return (sym == E) ||
		   (sym == "0") ||
		   (sym == "1") ||
		   (sym == "0b")||
		   (sym == "1b") ||
		   (sym == "Xb");

}

automaton* Scheme::Demands::getNFAsFromRegularGrammar(const demand_grammar* gram, const std::string pgmname)
{
	auto nfa = new automaton();
	state_transitions &trans = nfa->second;
	std::unordered_set<std::string> start_states;

	for(auto &nt:(*gram))
	{
		std::string startstate = nt.first;
		start_states.insert(startstate);
		std::string finalstate = nt.first + "_f";
	}

	for(auto &nt:(*gram)) //For each non-terminal
	{
		std::string startstate = nt.first;
		std::string finalstate = nt.first + "_f";
		nfa->first.insert(nt.first + "_f");

		for (auto p:nt.second)//For each production for the non-terminal
		{
			std::string current_state = startstate;
			std::string previousSymbol=E;
			bool ispreviousSymbolTerminal=false;
			for (auto sym:p) //For each symbol in p
			{

				if (isTerminal(sym))
				{
					if (ispreviousSymbolTerminal)
					{
						std::string newstate = createNewState(nt.first);
						trans[current_state];
						trans.at(current_state)[previousSymbol].insert(newstate);
						current_state = newstate;
					}
					ispreviousSymbolTerminal=true;
					previousSymbol=sym;

				}
				else
				{

					trans[current_state];
					trans.at(current_state)[previousSymbol].insert(sym);
					trans[sym + "_f"];
					current_state = sym + "_f";
					ispreviousSymbolTerminal=false;
					previousSymbol=E;
				}
			}
			trans[current_state];
			trans.at(current_state)[previousSymbol].insert(finalstate);
		}
	}

    std::cout << "Completed creating NFAs"<<std::endl;
    removeXbEdgesFrom(nfa);

	return nfa;
}



void Scheme::Demands::printNFAToFile(automaton *nfa, std::string filename)
{
	std::ofstream outfile(filename);
	state_transitions &trans = nfa->second;
	
	
	for(auto s: trans)
	{
		std::string sym1;
		if (nfa->first.find(s.first) == nfa->first.end())
			sym1 = s.first + "--";
		else
			sym1 = s.first + "$--";
	  for(auto sym: s.second)
		{
		  auto symbol = sym1;
		  symbol = symbol + sym.first + "-->";
//		  std::string symbol;
//		  if (nfa->first.find(s.first) == nfa->first.end())
//			  symbol = s.first + "--" + sym.first + "-->";
//		  else
//			  symbol = s.first + "$--" + sym.first + "-->";

			for(auto des:sym.second)
			{
				if (nfa->first.find(des) == nfa->first.end())
					outfile << symbol << des;
				else
					outfile << symbol << des << "$";
				outfile << std::endl; //sym.second << std::endl;
			}
		}
	}
}


std::unordered_set<std::string> epsilonClosure(std::string state, automaton *nfa);
automaton* Scheme::Demands::convertNFAtoDFA(std::unordered_set<std::string> start_states, automaton* nfa, const std::string pgmname)
{
	automaton *dfa = new automaton();
	std::map<std::string, std::unordered_set<std::string> > new_state_map;
	for(auto st_tuple:nfa->second)
	{
		std::string st;

		if (start_states.find(st_tuple.first) == start_states.end())
			continue;
		else
			st = st_tuple.first;


		auto new_s = epsilonClosure(st, nfa);


		std::vector<std::unordered_set<std::string> > processed;
		std::string s = st;
		new_state_map[s] = new_s;
		std::queue<std::string> q;
		q.push(s);
		processed.push_back(new_state_map.at(s));
		while(!q.empty())
		{
			auto curr_state = q.front();
			q.pop();
			//std::cout << "Processing state " << curr_state << std::endl;
			//for(auto term:{T0, T1, T0b, T1b})

			//TODO: What to do if the start state is the final state and has no other transitions?
			//Insert the state in the final state list of the dfa and insert a dummy transition
			if (nfa->first.find(curr_state) != nfa->first.end())
			{
				dfa->first.insert(curr_state);
				dfa->second[curr_state];
			}


			for(auto term:{T0, T1})//Remove the 0b and 1b edges
			{
				std::unordered_set<std::string> nexts;
				auto states = new_state_map[curr_state];
				for(auto next_state:states)
				{
					if(nfa->second.find(next_state) != nfa->second.end())
					{
						auto temp = nfa->second.at(next_state)[term];
						nexts.insert(temp.begin(), temp.end());
					}
					if(nfa->first.find(next_state) != nfa->first.end())
					{

						dfa->first.insert(s);
					}

				}

				dfa->second[curr_state];
				if (!nexts.empty())
				{
					if (std::find(processed.begin(), processed.end(), nexts) == processed.end())
					{
						auto new_name = createNewState(st);
						new_state_map[new_name] = nexts;
						processed.push_back(new_state_map.at(new_name));


						dfa->second.at(curr_state)[term] = {new_name};
						q.push(new_name);
					}
					else
					{
						std::string new_name;
						for(auto p:new_state_map)
						{
							if (p.second == nexts)
							{
								new_name = p.first;
								break;
							}
						}
						dfa->second.at(curr_state)[term] = {new_name};

					}
				}
			}

		}

	}
//Removing useless states. There might be a bug so commenting it for now
//	std::vector<std::string> useless_states;
//	for (auto nt:dfa->second)
//	{
//		if(nt.second.size() <= 0)
//		{
//			useless_states.push_back(nt.first);
//
//		}
//	}
//	for (auto state:useless_states)
//		dfa->second.erase(state);

	return dfa;
}
//TODO : Try to move make_key function to a common file so that it can be used in both Demands.cpp & SchemeDriver.cpp.
std::string make_key1(std::unordered_set<std::string>& st)
{
	assert(!st.empty());
	std::string key(*(st.begin()));
	auto st_temp = st.begin()++;
	for(auto s = st_temp; s != st.end(); s++)
	{
		key.append("#");
		key.append(*s);
	}
	return key;
}

std::vector<std::string> splitLivenessString(std::string ls)
{
	std::vector<std::string> v;
	std::string label_set_str = ls.substr(ls.find_first_of("/") + 1, ls.find_last_of("/") - ls.find_first_of("/") - 1);
	std::string var_name = ls.substr(ls.find_last_of("/") + 1,  std::string::npos);
	v.push_back(label_set_str);
	v.push_back(var_name);
	return v;
}

int Scheme::Demands::writeDFAToFile(std::string pgmname, automaton* dfa, std::map<std::string, std::unordered_set<std::string>> prog_pt_map)
{
	int numkeys = 1;
	std::ofstream state_map(outdir + pgmname + "/fsmdump-" + pgmname + "-state-map");
	std::map<std::string, int> st_map;
	for(auto st_tuple : dfa->second)
	{
	  	//std::cerr << "Adding " << st_tuple.first << " to state map" << std::endl;
		state_map << st_tuple.first << ":" << std::to_string(numkeys) <<";"<<std::endl;
		st_map[st_tuple.first] = numkeys;
		auto v = splitLivenessString(st_tuple.first);


		if (st_tuple.second.size() == 0 &&
			dfa->first.find(st_tuple.first) == dfa->first.end())
		{
			//If there are no transitions and it is not a final state then skip that state
//			std::cerr << "Skipping " << st_tuple.first << std::endl;
			//std::cerr << st_tuple.first << std::endl;
			continue;
		}

		//	std::cerr << "Process liveness string " << st_tuple.first << std::endl;
		for (auto p : prog_pt_map)
		{
		  //	std::cerr << "p.first = " << p.first << std::endl;
		  //	std::cerr << "p.second is " << make_key1(p.second) << " && v[1] = " << v[1] << std::endl;

			if (make_key1(p.second) == v[0] &&
			    v[1].find("(") == std::string::npos)
			{

				std::string state_name = "L/" + p.first + "/" + v[1];
				state_map << state_name << ":" << std::to_string(numkeys) <<";"<<std::endl;
//				std::cerr << "Adding " << state_name << " to state map" << std::endl;
				st_map[state_name] = numkeys;
			}
		}
		++numkeys;
	}
	state_map.close();


	std::ofstream transition_table(outdir + pgmname + "/fsmdump-" + pgmname + "-state-transition-table");
	for(auto st_tuple:dfa->second)
	{
		int src_state = st_map[st_tuple.first];
		for(auto tr_tuple:st_tuple.second)
		{
			std::string term = tr_tuple.first;
			assert(tr_tuple.second.size() == 1);
			int dest_state = st_map[*tr_tuple.second.begin()];
			transition_table << std::to_string(src_state)<<":"<<term<<":"<<std::to_string(dest_state)<<";"<<std::endl;
		}
	}
	transition_table.close();
	return numkeys;
}


std::unordered_set<std::string> epsilonClosure(std::string state, automaton *nfa)
{
	if (eps_closure_map.find(state) != eps_closure_map.end())
	{
		//std::cout << "Returning cached value for "<< state << std::endl;
		return eps_closure_map[state];
	}

	clock_t start, end;
	double cpuTime;
	start = clock();

	std::unordered_set<std::string> eps_closure;
	eps_closure.insert(state);
	std::queue<std::string> state_queue;
	state_queue.push(state);


	while(!state_queue.empty())
	{
		std::unordered_set<std::string> nexts;
		while(!state_queue.empty())
		{
			auto node = state_queue.front();
			state_queue.pop();
			if(nfa->second.find(node) != nfa->second.end())
			{
				std::unordered_set<std::string> temp = nfa->second.at(node)[E];
				nexts.insert(temp.begin(), temp.end());
			}
		}

		std::queue<std::string> temp_queue;
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

	eps_closure_map[state] = eps_closure;
	end = clock();
	cpuTime= (end - start) / (CLOCKS_PER_SEC);
	DBG(std::cout << "Time taken to calculate epsilon closure for state " << state << " is " << cpuTime << std::endl);
	//std::cout << "Finished epsilon closure for " << state << std::endl;
	return eps_closure;
}


void removeUnreachableStates(std::unordered_set<std::string> reachable_states,  automaton* nfa)
{
	std::vector<std::string> delete_list;
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





void Scheme::Demands::printSetofStates(std::unordered_set<std::string> states, std::ostream& out)
{
	out<<"{";
	for(auto state:states)
		out << state << ",";
	out<<"}"<<std::endl;
}


bool addTransitionsToNode(std::string src, std::string des, automaton *nfa)
{
	bool changed = false;
	//Add all transitions other than epsilon transitions from the des node to src node
	for(auto t:nfa->second.at(des))
	{
		if (E != t.first)
		{
			for(auto s:t.second)
			{
				if (nfa->second.at(src)[t.first].find(s) == nfa->second.at(src)[t.first].end())
				{
					//std::cout << "Adding transition " << src << " --" << t.first << "-> " << s << std::endl;
					nfa->second.at(src)[t.first].insert(s);
					changed = true;
				}
			}
		}

	}
	return changed;
}


bool replaceEdgesWithEpsilonEdge(automaton* nfa, std::pair<std::string, transitions> ts, state_transitions &trans, const std::string sym1, const std::string sym2)
{
	bool changed = false;
	auto &src = ts.first;
	auto &tmap = ts.second;

	if (tmap.find(sym1) == tmap.end() || tmap[sym1].empty())
		return changed;

	std::unordered_set<std::string> next = tmap[sym1];
	for(auto s:next)
	{
		if (!(trans.find(s) != trans.end() && trans.at(s).find(sym2) != trans.at(s).end()))
			continue;

		std::unordered_set<std::string> des = trans.at(s)[sym2];
		for(auto d:des)
		{
			if(trans.at(src)[E].find(d) != trans.at(src)[E].end())
				continue;

			//Add only if the edge is not present
			trans.at(src)[E].insert(d);
			//							std::cerr << "Adding an epsilon edge between " << src << " & " << d << std::endl;

			//TODO: SHOULD I REMOVE THIS CODE TO ADD src TO FINAL STATE LIST
			if (nfa->first.find(d) != nfa->first.end() &&
					nfa->first.find(src) == nfa->first.end())
			{
				//Insert the src as a final state if d is a final state.
				//								std::cerr << "Adding as final state " << src << std::endl;
				nfa->first.insert(src);
			}
			//std::cout << "Processed bar edge for " << src << std::endl;
			changed = true;

			//This is not correct. It should be the start state of the automaton being processed
			nfa_changed[src] = true;

		}

	}


	return changed;
}



bool barEdgeSimplification(automaton *nfa)
{
	bool changed = false;
	state_transitions &trans = nfa->second;
	std::cout << "Bar edge simplification started"<< std::endl;
	for(auto ts : trans)
	{
//		std::cerr << "Processing " << ts.first << std::endl;
//		if (nfa_changed[ts.first])
		{
			DBG(std::cerr << "Simplifying " << ts.first << std::endl);
			bool t1 = replaceEdgesWithEpsilonEdge(nfa, ts, trans, T0b, T0);
			bool t2 = replaceEdgesWithEpsilonEdge(nfa, ts, trans, T1b, T1);
			changed = t1 || t2 || changed;
		}
	}
	std::cout << "Bar edge simplification completed"<< std::endl;
	return changed;
}

bool removeEpsilonEdges(std::unordered_set<std::string> start_states, automaton *nfa)
{

	bool changed = false;
	std::unordered_set<std::string> reachable_states;
	int i = 0;
	std::cout << "starting removal of epsilon edges" << std::endl;
	std::cout << "Number of states to process " << start_states.size() << std::endl;
	eps_closure_map.clear();

	for(auto non_terminal: start_states)
	{
		clock_t start, end;
		double cpuTime;
		start = clock();

		++i;
		DBG(std::cout << "Processing non terminal " << non_terminal << std::endl);
		DBG(std::cout << "Processing terminal#" << i << std::endl);
		std::stack<std::string> states;
		std::unordered_set<std::string> processed;
		states.push(non_terminal);
		std::ostream &out = std::cerr;
		//std::cerr << "Processing non_terminal " << non_terminal << std::endl;

		while(!states.empty())
		{
			std::string curr_state = states.top();
			states.pop();//Remove it from the stack

			processed.insert(curr_state); //Add it to the list of states already processed.
			reachable_states.insert(curr_state);
			auto eps_closure = epsilonClosure(curr_state, nfa);

			for (auto s:eps_closure)
			{
				if (nfa->second.find(s) != nfa->second.end())
				{
					DBG(out << "Processing epsilon edge for " << s << std::endl);
					bool changed1 = addTransitionsToNode(curr_state, s, nfa);
					changed = changed1 || changed;
				}
				//If any of the states in the epsilon closure is a final state. Set the current state also as a final state
				if (nfa->first.find(s) != nfa->first.end() &&
						nfa->first.find(curr_state) == nfa->first.end())
				{
					//std::cerr << "Setting " << curr_state << " as final" <<std::endl;
					nfa->first.insert(curr_state);
					changed = true;
				}
			}
			//remove all epsilon transitions from the non_terminal
			//This check is required to see if the current state has any transitions otherwise there won't be an entry in the map.

			if (nfa->second.find(curr_state) != nfa->second.end())
			{
				auto &t = nfa->second.at(curr_state);

				//t.erase(E); //DO NOT ERASE THE epsilon EDGE AS THE BAR EDGE REMOVAL CODE WILL KEEP ADDING IT BACK
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
		end = clock();
	    cpuTime= (end - start) / (CLOCKS_PER_SEC);
	    DBG(std::cout << "Time taken for epsilon removal of terminal " << non_terminal << " is " << cpuTime << std::endl);
		DBG(std::cout << "Completed epsilon removal for " << non_terminal << std::endl);
		//std::cerr << "The set of reachable states from " << non_terminal << std::endl;
		//printSetofStates(reachable_states);
	}
	std::cout << "Completed processing all non_terminals" << std::endl;
	removeUnreachableStates(reachable_states, nfa);
	std::cout << "Removed unreachable states" << std::endl;



	return changed;
}

static std::unordered_map<std::string, int> st2idx;
static std::vector<std::string> idx2st;
int getLabel(std::string slbl) 
{
    if (slbl == E  ) return Scheme::OptDemands::E;
    if (slbl == T0 ) return Scheme::OptDemands::T0;
    if (slbl == T1 ) return Scheme::OptDemands::T1;
    if (slbl == T0b) return Scheme::OptDemands::T0b;
    if (slbl == T1b) return Scheme::OptDemands::T1b;
    if (slbl == TXb) return Scheme::OptDemands::TXb;

    return -1; // error
}
std::string getLabelStr(int lbl) 
{
    if (lbl == Scheme::OptDemands::E  ) return E;
    if (lbl == Scheme::OptDemands::T0 ) return T0;
    if (lbl == Scheme::OptDemands::T1 ) return T1;
    if (lbl == Scheme::OptDemands::T0b) return T0b;
    if (lbl == Scheme::OptDemands::T1b) return T1b;
    if (lbl == Scheme::OptDemands::TXb) return TXb;

    return "???"; // error
}

Scheme::OptDemands::automaton* toINFA(const automaton *nfa,
                                      const std::unordered_set<std::string> &start_states,
                                      std::unordered_set<int>& istart_states) 
{
    std::cerr << "In to INFA\n";
    int cstate = 0; // current state
    
    for(auto xs:nfa->second)
	{
        auto s = xs.first;
		if (st2idx.find(s) != st2idx.end())
            continue;
        idx2st.push_back(s);
        st2idx[s] = cstate++;
	}
    
	for(auto s:start_states) {
		if (st2idx.find(s) != st2idx.end())
            continue;
        idx2st.push_back(s);
        st2idx[s] = cstate++;
        
        istart_states.insert(st2idx.at(s));
    }
    
    std::unordered_set<int> final_states;
	for(auto s:nfa->first) {
		if (st2idx.find(s) != st2idx.end())
            continue;
        idx2st.push_back(s);
        st2idx[s] = cstate++;
        
        final_states.insert(st2idx.at(s));
    }
    
    Scheme::OptDemands::state_transitions strans;
    for (auto xs:nfa->second)
	{
        Scheme::OptDemands::transitions itx;
        std::unordered_set<int> idst;
        for (auto tx:xs.second) {
            auto label = getLabel(tx.first);
            for(auto dst:tx.second)
                idst.insert(st2idx.at(dst));
            itx[label] = idst;
        }
        auto s = st2idx.at(xs.first);
        strans[s] = itx;
    }
    
	Scheme::OptDemands::automaton* infa= new Scheme::OptDemands::automaton(final_states, strans);
    return infa;
}

automaton *fromINFA(Scheme::OptDemands::automaton* infa)
{
    std::cerr << "In from INFA\n";
    
    std::unordered_set<std::string> final_states;
	for(auto s:infa->first)
        final_states.insert(idx2st[s]);

    state_transitions strans;
    for (auto xs:infa->second)
	{
        transitions itx;
        std::unordered_set<std::string> idst;
        for (auto tx:xs.second) {
            auto label = getLabelStr(tx.first);
            for(auto dst:tx.second)
                idst.insert(idx2st[dst]);
            itx[label] = idst;
        }
        auto s = idx2st[xs.first];
        strans[s] = itx;
    }
    
	automaton* nfa= new automaton(final_states, strans);
    return nfa;
}

automaton* Scheme::Demands::simplifyNFA(std::unordered_set<std::string> start_states, automaton *nfa)
{
	bool changed = true;
	int i = 1;
#define TRYNEW 0
#if TRYNEW
    std::unordered_set<int> istart_states;
    Scheme::OptDemands::automaton* infa = toINFA(nfa, start_states, istart_states);
    infa = Scheme::OptDemands::simplifyNFA(istart_states, infa);
    return fromINFA(infa);
#else
	while(changed)
	{
	  changed = true;
	  std::cerr<< "Starting " << i <<"th round of simplification"<<std::endl;
        
	  bool changed1 = removeEpsilonEdges(start_states, nfa);
	  bool changed2 = barEdgeSimplification(nfa);
        
	  DBG(std::cerr << "Things changed in baredgesimplification "<<changed2<<std::endl);
	  DBG(std::cerr << "Things changed in epsilonedgeremoval "<<changed1<<std::endl);
	  changed = changed1 || changed2;
	  std::cout << "Completed " << i++ << " rounds of simplification" <<std::endl;
	}

	return nfa;
#endif
}



//This is the function which converts an arbitrary CFG into a regular CFG using the Mohri-Nederhoff transformation
regular_demand_grammar * Scheme::Demands::regularize(const demand_grammar * gram)
{
    using namespace boost;
    typedef adjacency_list<vecS, vecS, directedS, std::string> string_vertex_graph_t;

    unsigned i = 0;
    string_vertex_graph_t graph(gram->size());
    unordered_map<std::string, unsigned> vertex_number;
    for(auto & non_terminal : *gram)
    {
        vertex_number[non_terminal.first] = i;
        graph[i++] = non_terminal.first;
    }

    for(auto & non_terminal : *gram) {
        for(auto & prod_path : non_terminal.second) {
            for(auto & token : prod_path) {
                if(gram->find(token) != gram->end()) {
                    const int & from = vertex_number.at(non_terminal.first);
                    const int & to = vertex_number.at(token);
                    add_edge(from, to, graph);
                }
            }
        }
    }

    std::vector<unsigned> vertex_part(gram->size());
    unsigned comp_cnt = strong_components(graph,
                                          make_iterator_property_map(vertex_part.begin(),
                                                  get(vertex_index, graph),
                                                  vertex_part[0]));

    auto parts = new partitions(comp_cnt);
    for(auto & vert : vertex_number)
    {
        parts->at(vertex_part.at(vert.second)).first = NONE;
        parts->at(vertex_part.at(vert.second)).second.insert(vert.first);
    }

    auto new_gram = new demand_grammar;
    for(auto & mr_set : *parts)
    {
        // May be we can optimize which mr_set can be skipped
        if(mr_set.second.size() == 1)
        {
            auto self_loop = false;

            auto & non_terminal = *(mr_set.second.begin());

            for(auto & prod_path : gram->at(non_terminal))
            {
                if(prod_path.size() == 1)
                    continue;
                else if(prod_path.size() == 2) {
                    if(*prod_path.begin() == non_terminal
                       && *std::prev(prod_path.end()) == non_terminal) {
                        self_loop = true;
                        break;
                    }
                } else if(std::find(std::next(prod_path.begin()),
                                    std::prev(prod_path.end()),
                                    non_terminal) != std::prev(prod_path.end())) {
                    self_loop = true;
                    break;
                }
            }

            if(!self_loop)
            {
                new_gram->emplace(non_terminal, gram->at(non_terminal));
                continue;
            }
        }

        for(auto & non_terminal : mr_set.second)
        {
            new_gram->emplace(non_terminal, rule());
            new_gram->emplace(non_terminal + SUFFIX_APPROX, rule({{E}}));

        }

        for(auto & non_terminal : mr_set.second)
        {
            for(auto & prod_path : gram->at(non_terminal))
            {
                auto current_nt = non_terminal;
                path new_path;

                for(auto & token : prod_path) {
                    // If the token is not an NT from current MR set, retain it
                    if(mr_set.second.find(token) == mr_set.second.end()) {
                        new_path.push_back(token);
                        continue;
                    }

                    // Else, add the token to the new_path
                    new_path.push_back(token);
                    // Add the new_path and clear it
                    new_gram->at(current_nt).insert(new_path);
                    new_path.clear();
                    // Now work on the generated nt
                    current_nt = token + SUFFIX_APPROX;
                }

                new_path.push_back(non_terminal + SUFFIX_APPROX);
                new_gram->at(current_nt).insert(new_path);
            }
        }
    }

    i = 0;
    graph.clear();
    vertex_part.clear();
    vertex_number.clear();

    string_vertex_graph_t new_graph(new_gram->size());
    for(auto & non_terminal : *new_gram)
    {
        vertex_number[non_terminal.first] = i;
        new_graph[i++] = non_terminal.first;
    }

    for(auto & non_terminal : vertex_number)
    {
        for(auto & prod_path : new_gram->at(non_terminal.first))
        {
            for(auto & token : prod_path)
            {
                if(vertex_number.find(token) != vertex_number.end())
                {
                    const int & from = vertex_number.at(non_terminal.first);
                    const int & to = vertex_number.at(token);
                    add_edge(from, to, new_graph);
                }
            }
        }
    }

    vertex_part.resize(vertex_number.size());
    unsigned new_comp_cnt = strong_components(new_graph,
                                              make_iterator_property_map(vertex_part.begin(),
                                                      get(vertex_index, new_graph),
                                                      vertex_part[0]));

    parts->clear();
    parts->resize(new_comp_cnt);
    for(auto & vert : vertex_number)
    {
        auto & part = parts->at(vertex_part.at(vert.second));
        part.second.insert(vert.first);
        part.first = NONE;
    }

    for(auto & mr_set : *parts)
    {
        bool nt_found = false, left = false, right = false;
        for(auto & non_terminal : mr_set.second)
        {
            for(auto & prod_path : new_gram->at(non_terminal))
            {
                int pos_nt = -1;
                for(auto i = prod_path.begin(); i != prod_path.end(); ++i)
                {
                    if(mr_set.second.find(*i) != mr_set.second.end())
                    {
                        pos_nt = std::distance(prod_path.begin(), i);
                        break;
                    }
                }
                
                if (pos_nt == -1)
                    continue;

                auto prod_path_size = (int)prod_path.size(); // to avoid compiler warning!!
                if(pos_nt > 0)
                    left = true;
                if(pos_nt < prod_path_size -1 )
                    right = true;
                if(pos_nt >=0 && pos_nt <= prod_path_size - 1)
                    nt_found = true;
            }
        }
        
        assert(!(left && right));
        if(nt_found)
        {
            if(!left && !right)
                mr_set.first = CYCLE;
            else if(!right)
                mr_set.first = RIGHT;
            else if(!left)
                mr_set.first = LEFT;
        }
    }

    return new regular_demand_grammar(new_gram, parts);
}




void print_path (path prod_path)
{

	std::stringstream ss;
	for_each(prod_path.begin(), prod_path.end(), [&ss] (const std::string& s) { ss << "." << s; });
	if (ss.str().size() > 1)
	{
		std::string a = ss.str().substr(1);
	}
}



//This should be called store LF in global var or something like that
void print_split_lf(std::pair<rule,rule> res, std::string trans_demand_key)
{

	trans_demand_key.replace(0, 1, "IF");
	gLivenessData[trans_demand_key] = res.first;

    trans_demand_key.replace(0, 2, "DF");
    gLivenessData[trans_demand_key] = res.second;

}

demand_grammar * Scheme::Demands::solve_functions_and_combine(expr_demand_grammars * grams)
{
    for(auto & non_terminal : *(grams->second))
    {
    	auto function_demand_key = non_terminal.first.substr(0,
                                                            non_terminal.first.find_last_of(SEPARATOR));
        auto trans_demand_key = non_terminal.first;

        // The demand transformer would be same as the demand on the argument.
        grams->first->emplace(trans_demand_key, non_terminal.second);
        // Remove the demand on the function which was appended at the end.
        std::pair<rule,rule> res = solveLF(trans_demand_key, grams->first->at(trans_demand_key));
        print_split_lf(res, trans_demand_key);
    }

    auto final_gram = new demand_grammar(*(grams->first));

    return sanitize(final_gram);
}



std::pair<rule,rule> Scheme::Demands::splitLF(path p)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("/");
	tokenizer tok(p.front(), sep);
	tokenizer::iterator titer = tok.begin();
	++titer;
	std::string funcname = *titer;
	++titer;
	std::string argnum = *titer;

	//convert the rest of the string as a demand
	p.pop_front();
	rule dem;
	dem.insert(p);

	rule ifpart;
	std::stringstream ss;
	ss << "IF/" << funcname << "/" << argnum;
	path ptemp;
	ptemp.push_back(ss.str());
	ifpart.insert(ptemp);


	rule dfpart;
	rule exp = expandProd(p);
	for(auto d: dem)
		for(auto extended_dem: expandProd(d))
		{
			std::stringstream ss;
			ss << "DF/" << funcname << "/" << argnum;
			path ps;
			ps.push_back(ss.str());
			ps.insert(ps.end(), extended_dem.begin(),extended_dem.end());
			dfpart.insert(ps);
		}

	return std::pair<rule,rule>(ifpart, dfpart);

}

rule Scheme::Demands::expandProd(path p)
{

	if (p.size() == 0)
	{
		rule r;
		r.insert(path()); //Insert an empty path
		return r;
	}

	std::string p1 = p.front();
	if(p1[0] == 'T') //if it is an LF //TODO Change this condition to check whether the rule begins with "LF"
	{
		//std::cout << "Processing " << p1 << std::endl;
		std::pair<rule,rule> res = splitLF(p);
		rule r = res.first;
		r.insert(res.second.begin(), res.second.end());

		return r;
	}
	else
	{
		rule r;
		path temp_path;
		temp_path.push_back(p1);
		r.insert(temp_path);
		if (p.size() == 1)
		{
			return r;
		}

		path::iterator it = p.begin();
		++it; //move it to the next element
		rule r2 = expandProd(path(it, p.end()));

		rule result;

		for(auto qpath: r2)
		{
			//for each path in r2 prefix p1[0].
			path temp;
			temp.push_back(p1);
			temp.insert(temp.end(), qpath.begin(), qpath.end());
			result.insert(temp);
		}

		return result;
	}
}

std::pair<rule,rule> Scheme::Demands::solveLF(std::string lf, rule demands)
{
	rule ifprods;
	rule dfprods;
	for (auto prod: demands)
	{
		rule exp = expandProd(prod);
		for (auto p: exp)
		{
			if (p.size() > 0 && p.back()[0] == 'D')
			{
				path pnew = p;
				pnew.pop_back();
				//Add an epsilon demand if only the sigma part exists.
				if (pnew.size() == 0)
					pnew.push_front(E);

				dfprods.insert(pnew);
			}
			else
			{
				ifprods.insert(p);
			}
		}
	}
	return std::pair<rule, rule>(ifprods, dfprods);
}



demand_grammar * Scheme::Demands::sanitize(demand_grammar * gram)
{
    std::unordered_set<std::string> empty_non_terminals;

    // Fixed point iteration -- removal of all empty_non_terminals
    do
    {
        std::unordered_set<std::string> new_empty_non_terminals;

        for(auto non_terminal = gram->begin(); non_terminal != gram->end(); )
        {
            // For the remaining nts, remove paths which contain empty-marked nts.
            for(auto i = non_terminal->second.begin(); i != non_terminal->second.end(); )
            {
                auto flag = false;
                for(auto & token : *i)
                    if(empty_non_terminals.find(token) != empty_non_terminals.end())
                    {
                        flag = true;
                        break;
                    }
                    else if(token != "D/all"
                              && Terminals.find(token) == Terminals.end()
                              && gram->find(token) == gram->end())
                    {
                        flag = true;
                        break;
                    }

                flag |= (!i->size() || (i->size() == 1 && i->front() == non_terminal->first));

                if(flag)    non_terminal->second.erase(i++);
                else        ++i;
            }

            // If a non-marked nt becomes empty, mark it.
            if(!non_terminal->second.size())
            {
                new_empty_non_terminals.insert(non_terminal->first);
//                std::cerr << "Erasing " << non_terminal->first << std::endl;
                gram->erase(non_terminal++);
            }
            else
            	++non_terminal;
        }

        // Continue iteration with the new set of marked nts.
        empty_non_terminals = new_empty_non_terminals;
    } while(empty_non_terminals.size());

    return gram;
}


expr_demand_grammars * Scheme::Demands::merge(expr_demand_grammars * grams_to,
                                              expr_demand_grammars * grams_from)
{
	// Merge the program point demands. There should be no intersection within
	// the argument grammars.

	//Do we need this merging?
	
	expr_demand_grammars *res =  new expr_demand_grammars;
	*res = *grams_to;

    for(auto & non_terminal : *(grams_from->first))
    //assert(grams_to->first->emplace(non_terminal.first, non_terminal.second).second);
    {
    	res->first->emplace(non_terminal.first, non_terminal.second);
    }

    // Merge the variable name demands. There might be intersections here. In
    // that case, we take the union of the demands.
    for(auto & non_terminal : *(grams_from->second))
    {
    	//This condition was added to prevent null constants from being propagated during liveness propagation
    	if(non_terminal.first.size() > 0) //merge only if the non_terminal is a variable
    	{
    		auto result = res->second->emplace(non_terminal.first, non_terminal.second);
			if(!result.second)
				result.first->second.insert(non_terminal.second.begin(), non_terminal.second.end());
    	}
    }

    return res;
}

/////////////////////////////////////////////DFA Minimization code/////////////////////////////////////////////////////////
//typedef std::unordered_set<std::string> partition;
partition reachable_states(std::string nt, automaton* nfa)
{
	partition p;
	std::stack<std::string> state_stack;
	std::deque<std::string> visited;
	state_transitions trans = nfa->second;
	state_stack.push(nt);
	//std::cout <<"processing non-terminal "<< nt<<std::endl;
	while(!state_stack.empty())
	{
		std::string curr_state = state_stack.top();
		state_stack.pop();
		visited.push_back(curr_state);
		p.insert(curr_state);
		//std::cout << "Current state " << curr_state << std::endl;
		if (trans.find(curr_state) != trans.end())
		{
			transitions ts = trans.at(curr_state);
			if (ts.find(T0) != ts.end())
			{
				for (auto dest: ts.at(T0))
					if (std::find(visited.begin(), visited.end(), dest) == visited.end())
						state_stack.push(dest);
			}
			if (ts.find(T1) != ts.end())
			{
				for (auto dest: ts.at(T1))
					if (std::find(visited.begin(), visited.end(), dest) == visited.end())
						state_stack.push(dest);
			}
			if (ts.find(E) != ts.end())
			{
				for (auto dest: ts.at(E))
					if (std::find(visited.begin(), visited.end(), dest) == visited.end())
						state_stack.push(dest);

			}
			if (ts.find(T1b) != ts.end())
			{
				for (auto dest: ts.at(T1b))
					if (std::find(visited.begin(), visited.end(), dest) == visited.end())
						state_stack.push(dest);
			}
			if (ts.find(T0b) != ts.end())
			{
				for (auto dest: ts.at(T0b))
					if (std::find(visited.begin(), visited.end(), dest) == visited.end())
						state_stack.push(dest);
			}

		}
	}

	return p;
}

partition initPartition(std::string nt, automaton* dfa)
{
	return reachable_states(nt, dfa);
}

std::pair<partition,partition> split_partition(partition &p, std::unordered_set<std::string> &final_states)
{
	partition terminal_states;
	partition non_terminal_states;
	for(auto state: p)
	{
		if (final_states.find(state) != final_states.end())
			terminal_states.insert(state);
		else
			non_terminal_states.insert(state);
	}
	return std::pair<partition, partition>(non_terminal_states, terminal_states);
}

partition getSubsetWithSimilarTransitions(partition curr_partition, automaton* dfa, bool goesToFinalOn0, bool goesToFinalOn1)
{
	partition p;
	state_transitions trans = dfa->second;
	std::unordered_set<std::string> final_states = dfa->first;
	for(auto state:p)
	{
		if (trans.find(state) != trans.end())
		{
			transitions ts = trans.at(state);
			if (ts.find(T0) != ts.end())
			{
				if (goesToFinalOn0 && final_states.find(*(ts.at(T0).begin())) != final_states.end())
					p.insert(state);
				if (!goesToFinalOn0 && final_states.find(*(ts.at(T0).begin())) == final_states.end())
									p.insert(state);
			}
			if (ts.find(T1) != ts.end())
			{
				if (goesToFinalOn1 && final_states.find(*(ts.at(T1).begin())) != final_states.end())
					p.insert(state);
				if (!goesToFinalOn1 && final_states.find(*(ts.at(T1).begin())) == final_states.end())
					p.insert(state);
			}

		}
		//TODO : What happens if it has no transition on some symbol or both symbols? Is it handled, test to make sure.
	}
	return p;
}

void print_minimized_dfa(automaton *dfa)
{
	for(auto trans : dfa->second)
		for (auto ts : trans.second)
			{
				std::cout << trans.first << " on " << ts.first << " goes to ";
				printSetofStates(ts.second);
			}
}

void get_minimized_dfa(automaton *dfa, std::deque<partition> minimized_dfa, automaton *min_dfa)
{
	auto trans = dfa->second;
	for(auto ps: minimized_dfa)
	{
		auto state_iter = ps.begin();

		if (state_iter != ps.end())
		{
			auto state = *state_iter;
			if(trans.find(state) != trans.end())
			{
				auto ts = trans.at(state);
				if(ts.find(T0) != ts.end())
				{
					auto dest = *(ts[T0].begin());
					partition dest_partition;
					for(auto partitions : minimized_dfa)
					{
						auto temp = partitions.find(dest);
						if (temp != partitions.end())
						{
							min_dfa->second[state][T0].insert(partitions.begin(), partitions.end());
							break;
						}
					}

				}
				if(ts.find(T1) != ts.end())
				{
					auto dest = *(ts[T1].begin());
					partition dest_partition;
					for(auto partitions : minimized_dfa)
					{
						auto temp = partitions.find(dest);
						if ( temp != partitions.end())
						{
							min_dfa->second[state][T1].insert(partitions.begin(), partitions.end());
							break;
						}
					}
				}
			}
		}
	}
}

//This minimization process is very specific to our requirement. It handles only two alphabets 0 & 1.
//TODO should we write a more general determinization function?
void Scheme::Demands::minimizeDFA(automaton* dfa, std::unordered_set<std::string> nt)
{
	std::unordered_set<std::string> processed_states;
	std::unordered_map<std::string, std::deque<partition>> dfa_map;
	automaton* min_dfa = new automaton();
	for(auto start_state:nt)
	{
		if (processed_states.find(start_state) == processed_states.end())
		{
			//Create partitions
			//while no more partitions possible make partitions for each input symbol for each state
			partition p = initPartition(start_state, dfa);
			processed_states.insert(p.begin(), p.end());
			std::pair<partition, partition> ps = split_partition(p, dfa->first);
			std::deque<partition> partition_stack;
			std::deque<partition> minimized_dfa;
			partition_stack.push_front(ps.first);
			partition_stack.push_front(ps.second);
			while(!partition_stack.empty())
			{
				auto curr_partition = partition_stack.front();
				partition_stack.pop_front();
				partition p00 = getSubsetWithSimilarTransitions(curr_partition, dfa, false, false);
				partition p01 = getSubsetWithSimilarTransitions(curr_partition, dfa, false, true);
				partition p10 = getSubsetWithSimilarTransitions(curr_partition, dfa, true, false);
				partition p11 = getSubsetWithSimilarTransitions(curr_partition, dfa, true, true);
				if (!p00.empty())
					partition_stack.push_front(p00);
				if (!p01.empty())
					partition_stack.push_front(p01);
				if (!p10.empty())
					partition_stack.push_front(p10);
				if (!p11.empty())
					partition_stack.push_front(p11);
				if (p00.empty() && p01.empty() && p10.empty() && p11.empty())
					minimized_dfa.push_front(curr_partition);
			}
			for(auto state:p)
			{
				dfa_map[state] = minimized_dfa;
			}
			get_minimized_dfa(dfa, minimized_dfa, min_dfa);
		}


	}
	print_minimized_dfa(min_dfa);

}

void print_fa_to_graphviz_file(std::string pgmname, std::string non_terminal, automaton* nfa)
{
//	digraph finite_state_machine {
//		rankdir=LR;
//		size="8,5"
//		node [shape = doublecircle]; LR_0 LR_3 LR_4 LR_8;
//		node [shape = circle];
//		LR_0 -> LR_2 [ label = "SS(B)" ];
//		LR_0 -> LR_1 [ label = "SS(S)" ];
//	}
	char cCurrentPath[FILENAME_MAX];
	if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		exit(-1);
	}
	std::string filename = non_terminal;
	std::replace(filename.begin(), filename.end(), '/', '_');
	filename = filename + ".gv";

	std::ofstream gvizfile(std::string(cCurrentPath) + "/output/" + pgmname + "/" + filename);
	gvizfile << "digraph finite_state_machine {\n";
	gvizfile << "rankdir=LR;\n";
	gvizfile <<	"size=\"8,5\"\n";

	auto final_states = nfa->first;
	partition rchable_states = reachable_states(non_terminal, nfa);
	std::string final_state_string;

	for(auto st:rchable_states)
	{

		if (final_states.find(st) != final_states.end())
		{
			std::string fst = st;
			std::replace(fst.begin(), fst.end(), '/', '_');
			std::replace(fst.begin(), fst.end(), '*', 'B');
			std::replace(fst.begin(), fst.end(), '(', '_');
			std::replace(fst.begin(), fst.end(), ')', '_');
			std::replace(fst.begin(), fst.end(), '-', '_');

			final_state_string = final_state_string + fst + " ";

		}
	}

	if(final_state_string.length() > 0)
	{
		gvizfile << "node [shape = doublecircle]; ";
		gvizfile << final_state_string;
		gvizfile << ";\n" ;
	}
	gvizfile << "node [shape = circle];\n";
	auto trans = nfa->second;
	std::queue<std::string> state_queue;
	std::unordered_set<std::string> processed_set;
	state_queue.push(non_terminal);
	while(!state_queue.empty())
	{
		auto curr_state = state_queue.front();
		state_queue.pop();
		processed_set.insert(curr_state);
		if (trans.find(curr_state) != trans.end())
		{
			std::string src_state = curr_state;
			std::replace(src_state.begin(), src_state.end(), '/', '_');
			std::replace(src_state.begin(), src_state.end(), '*', 'B');
			std::replace(src_state.begin(), src_state.end(), '(', '_');
			std::replace(src_state.begin(), src_state.end(), ')', '_');
			std::replace(src_state.begin(), src_state.end(), '-', '_');
			for(auto ts:trans.at(curr_state))
			{
				for (auto s: ts.second)
				{
					std::string dest_state = s;
					std::replace(dest_state.begin(), dest_state.end(), '/', '_');
					std::replace(dest_state.begin(), dest_state.end(), '*', 'B');
					std::replace(dest_state.begin(), dest_state.end(), '(', '_');
					std::replace(dest_state.begin(), dest_state.end(), ')', '_');
					std::replace(dest_state.begin(), dest_state.end(), '-', '_');
					gvizfile << src_state << " -> " << dest_state << " [ label = \"" << ts.first << "\" ]; \n";
					if(processed_set.find(s) == processed_set.end())
					{
						state_queue.push(s);
					}
				}
			}
		}
	}
	gvizfile << "}\n";
	gvizfile.close();
}

#define print std::cout
#define endline std::endl

void Scheme::Demands::simplifyCFG(demand_grammar* gram)
{
	//Remove empty productions and hence any other productions which contain that as the right hand side
	bool changed = true;
	while(changed)
	{
		changed = false;
		std::vector<std::string> empty_nts;
		for(auto &prod : (*gram))
		{
			if (prod.second.size() == 0)
			{
				//print << "Processing empty production " << prod.first << endline;
				empty_nts.push_back(prod.first);
				changed = true;
			}
			else if (prod.second.size() == 1)
			{
				rule &prod_rules = prod.second;
				auto p = prod_rules.begin();
				if (std::find(p->begin(), p->end(), prod.first) != p->end())
				{
					//print << "non-terminating production " << prod.first << endline;
					empty_nts.push_back(prod.first);
					changed = true;
				}
			}
		}
		for(auto nt : empty_nts)
		{
			gram->erase(nt);
			for (auto &prod : (*gram))
			{
				rule &prod_rules = prod.second;
				//path p;
				//p.begin()
				for (auto r : prod_rules)
				{
					if (std::find(r.begin(), r.end(), nt) != r.end())
					{
						//print << "deleting rule for " << prod.first << " which contains empty non-terminal " << nt << endline;
						prod_rules.erase(r);
					}
				}

			}
		}
	}
	return;
}
