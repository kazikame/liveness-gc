#include "Output.h"

using namespace Scheme::output;

std::ostream & Scheme::output::indentTill(std::ostream & stream, unsigned depth) {
    if(depth)       stream.put('\n');
    while(depth--)  stream.put(' ');
    return stream;
}

void Scheme::output::dumpGrammar(std::ostream & out,
                                 Scheme::Demands::regular_demand_grammar * rgram) {
    out << std::left;
    auto gram = rgram->first;
    for(auto ipart = 0; ipart < rgram->second->size(); ++ipart)
    {
        auto type = "NONE";
        if(rgram->second->at(ipart).first == Scheme::Demands::LEFT)         type = "LEFT";
        else if(rgram->second->at(ipart).first == Scheme::Demands::RIGHT)   type = "RIGHT";
        else if(rgram->second->at(ipart).first == Scheme::Demands::CYCLE)   type = "CYCLE";

        out << "Partition #" << ipart
            << " | SIZE = " << rgram->second->at(ipart).second.size()
            << " | TYPE = " << type << "\n";
        for(auto & non_terminal : rgram->second->at(ipart).second) {
            out << std::setw(16) << non_terminal << arrow;

            if(!gram->at(non_terminal).size())
            {
                out << empty << "\n";
                continue;
            }

            auto path_iter = gram->at(non_terminal).begin();

            if(path_iter->size())
            {
                auto token_iter = path_iter->begin();
                out << *token_iter;
                while(++token_iter != path_iter->end())
                    out << cdot << *token_iter;
            } else
                out << empty;

            while(++path_iter != gram->at(non_terminal).end()) {
                out << vpipe;
                if(path_iter->size())
                {
                    auto token_iter = path_iter->begin();
                    out << *token_iter;
                    while(++token_iter != path_iter->end())
                        out << cdot << *token_iter;
                } else
                    out << empty;
            }

            out << "\n";
        }
        out << "\n";
    }
}

void Scheme::output::dumpGrammar(std::ostream & out, Scheme::Demands::demand_grammar * gram) {
    out << std::left;
    for(auto & non_terminal : *gram) {
        out << std::setw(16) << non_terminal.first << arrow;

        if(!non_terminal.second.size())
        {
            out << empty << std::endl;
            continue;
        }

        auto path_iter = non_terminal.second.begin();

        if(path_iter->size())
        {
            auto token_iter = path_iter->begin();
            out << *token_iter;
            while(++token_iter != path_iter->end())
                out << cdot << *token_iter;
        }
        else
            out << empty;

        while(++path_iter != non_terminal.second.end())
        {
            out << vpipe;
            if(path_iter->size()) {
                auto token_iter = path_iter->begin();
                out << *token_iter;
                while(++token_iter != path_iter->end())
                    out << cdot << *token_iter;
            }
            else
                out << empty;
        }

        out << std::endl;
    }
}

void Scheme::output::dumpGrammar(std::ostream & out,
                                 Scheme::Demands::expr_demand_grammars * grammars) {
    for(auto & gram : {
                grammars->first, grammars->second
            })
        dumpGrammar(out, gram);
}
