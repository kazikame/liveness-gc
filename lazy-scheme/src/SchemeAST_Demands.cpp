#include "SchemeAST.h"

using namespace Scheme::AST;
using namespace Scheme::Demands;


demand_grammar function_call_demands;
demand_grammar gLivenessData; //Global map which stores the liveness at cons points and the next statement after a function call only.


/* D( (return x), s, T ) = x.s  */
expr_demand_grammars * ReturnExprNode::transformDemand(const rule & demand) const
{
	return pID->transformDemand(demand);
}


/* ref( (id x), s, T ) = x.s  */
expr_demand_grammars * IdExprNode::transformDemand(const rule & demand) const
{

	return new expr_demand_grammars({ new demand_grammar({{ label, demand }}),
        new demand_grammar({{ *pID, demand }})
    });
}


/* ref( k, s, T ) = {k.s}  */
expr_demand_grammars * ConstExprNode::transformDemand(const rule & demand) const
{
    return new expr_demand_grammars({ new demand_grammar({{ label, demand }}), new demand_grammar({{ }}) }); //TODO: verify this rule

}


/* ref( (car x), s, T) = x.(0 s) U {x.eps}
   ref( (cdr x), s, T) = x.(1 s)
   ref( (null? x), s, T) = x.s     This should have been x.eps. Why are we putting the demand s on x?        */
expr_demand_grammars * UnaryPrimExprNode::transformDemand(const rule & demand) const
{
    rule arg_demand;

    if(node_name == "car")
    { /* x.(0 s) */
        for(auto p : demand)
        {
            p.push_front(T0);
            arg_demand.insert(p);
        }
        //Add epsilon demand
        path p;
        p.push_front(E);
        arg_demand.insert(p);

    }
    else if(node_name == "cdr")
    { /* x.(1 s) */
        for(auto p : demand) {
            p.push_front(T1);
            arg_demand.insert(p);
        }
        //Add epsilon demand
        path p;
        p.push_front(E);
        arg_demand.insert(p);
    }
    else
    {
        /* x.s */  /* should be x.eps */
//        for(auto p : demand)
//            arg_demand.insert(p);
    	path p;
    	p.push_front(E);
    	arg_demand.insert(p);
    }

    expr_demand_grammars * result = pArg->transformDemand(arg_demand);
    result->first->emplace(label, demand);

    demand_grammar* var_grammar = result->second;


    return result;
}


/* ref( (cons x y), s, T) = x.(0b s) + y.(1b s)
   ref( (eq? x y), s, T) = x.s + y.s            */
expr_demand_grammars * BinaryPrimExprNode::transformDemand(const rule & demand) const
{
    rule arg_1_demand, arg_2_demand;

    if(node_name == "cons")
    { /* x.(0b s) + y.(1b s) */
    	for(auto p : demand)
        {
            p.push_front(T0b);
            arg_1_demand.insert(p);
        }
        for(auto p : demand) {
            p.push_front(T1b);
            arg_2_demand.insert(p);
        }

    }
    else
    {
    	//Shouldn't this be epsilon demand for all the cases?
        /* x.s + y .s */
//        for(auto p : demand)
//            arg_1_demand.insert(p);
//        for(auto p : demand)
//            arg_2_demand.insert(p);
    	//Inserting epsilon demands for arguments for primitive functions other than cons
    	path p;
    	p.push_front(E);
    	arg_1_demand.insert(p);
    	arg_2_demand.insert(p);
    }

    expr_demand_grammars * result =  merge(pArg1->transformDemand(arg_1_demand),
                                           pArg2->transformDemand(arg_2_demand));
    result->first->emplace(label, demand);

    return result;
}


/* D( (if x e1 e2), s, T) = D(e1, s, T) + D(e2, s, T) + x.(Xb s) */
expr_demand_grammars * IfExprNode::transformDemand(const rule & demand) const
{
    rule cond_demand; // TODO Add an epsilon demand instead of Xb demand.
     //We don't need the Xb addition for liveness GC for strict languages
//    for(auto p : demand) {
//        p.push_front(TXb);
//        cond_demand.insert(p);
//    }
    path p;
    p.push_front(E);
    cond_demand.insert(p);
    expr_demand_grammars * result =  merge(pCond->transformDemand(cond_demand),
                                           merge(pThen->transformDemand(demand),
                                                 pElse->transformDemand(demand)));
    result->first->emplace(label, demand);

    return result;
}


/* D( (let x <- st in e), s, T) = E - E(x) + ref(st, E(x), T) where E = D(e, s, T) */
expr_demand_grammars * LetExprNode::transformDemand(const rule & demand) const
{
	expr_demand_grammars * result = pBody->transformDemand(demand);

	if (pExpr->isFunctionCallExpression())
	{
		demand_grammar* var_grammar = result->second;
		for (auto &var : (*var_grammar))
		{
			string nt = "L/" + pBody->getLabel() +  "/" + var.first;
			gLivenessData[nt] = var.second;
			//((FuncExprNode*)pExpr)->setNextExpr(pBody->getLabel());
		}
	}

    std::string let_var = pID->getIDStr();
    auto let_var_demand_it = result->second->find(let_var);

    if(let_var_demand_it != result->second->end())
    {
        rule let_var_demand = let_var_demand_it->second;
        result->second->erase(let_var_demand_it);
        expr_demand_grammars* let_body = pExpr->transformDemand(let_var_demand);

        result = merge(result, let_body);
        if (pExpr->isConsExpression())
        {
        	demand_grammar* var_grammar = result->second;
        	for (auto &var : (*var_grammar))
        	{
        		string nt = "L/" + pExpr->getLabel() +  "/" + var.first;
        		gLivenessData[nt] = var.second;
        	}
        }
    }

    result->first->emplace(label, demand);


    return result;
}


/* D( (f x1 x2 ... xn), s, T) = T_1(x1, s) + T_2(x2, s) + ... + T_n(xn, s) */
expr_demand_grammars * FuncExprNode::transformDemand(const rule & demand) const
{
   // TODO :Have to change it to have both IF and DF part
	rule temp_demand = demand;
    if(function_call_demands.find(pID->getIDStr()) != function_call_demands.end())
        function_call_demands[pID->getIDStr()].insert(temp_demand.begin(), temp_demand.end());
    else
        function_call_demands[pID->getIDStr()] =  demand;

    if(pListArgs->size())
    {
        unsigned index = 1;
        std::string prefix = PREFIX_TRANSFORMER + SEPARATOR + pID->getIDStr() + SEPARATOR;

        rule arg_demand;
        //This should be done for the DF's only
        for(auto p : demand)
        {
            p.push_front(prefix + std::to_string(index));
            arg_demand.insert(p);
        }

        auto iter = pListArgs->begin();
        auto result = (*iter)->transformDemand(arg_demand);

        while(++iter != pListArgs->end())
        {
            ++index;
            arg_demand.clear();
            for(auto p : demand) {
                p.push_front(prefix + std::to_string(index));
                arg_demand.insert(p);
            }
            result = merge(result, (*iter)->transformDemand(arg_demand));
        }

        result->first->emplace(label, demand);

        return result;
    }
    else
    {

    	return new expr_demand_grammars({new demand_grammar({{label, demand}}), new demand_grammar});
    }
}


expr_demand_grammars * DefineNode::transformDemand(const rule & demand) const {
    // The demand argument is unnecessary for this function.

    std::string func_demand_prefix = PREFIX_DEMAND + SEPARATOR + pID->getIDStr();
    expr_demand_grammars * result = pExpr->transformDemand(rule({{ func_demand_prefix }}));

    int index = 0;
    demand_grammar * arg_demands = new demand_grammar;
    for(auto & arg : *pListArgs) {
        auto arg_demand_pair = result->second->find(arg->getIDStr());
        if(arg_demand_pair != result->second->end())
            arg_demands->emplace(func_demand_prefix + SEPARATOR + std::to_string(++index),
                                 arg_demand_pair->second);
    }
//The map containing the variable -> demand mapping is being deleted and is overwritten by symbolic demand on arguments.
//TODO Have to see how this has to be updated to handle liveness.
//    demand_grammar* var_grammar = result->second;
//    for (auto &var : (*var_grammar))
//    {
//    	string nt = "L/" + getLabel() +  "/" + var.first;
//    	gLivenessData[nt] = var.second;
//    }

    delete result->second;
    result->second = arg_demands;

    return result;
}




expr_demand_grammars * ProgramNode::transformDemand(const rule & demand) const {
    // The demand argument is unnecessary for this function.

    function_call_demands.clear();

    rule symbolic_demand = rule({{ PREFIX_DEMAND + SEPARATOR + "all" }});
    expr_demand_grammars * result = pExpr->transformDemand(symbolic_demand);
    gLivenessData[PREFIX_DEMAND + SEPARATOR + "all" ] = rule({{"0", PREFIX_DEMAND + SEPARATOR + "all" }, {"1", PREFIX_DEMAND + SEPARATOR + "all" },{E}});

    delete result->second;
    result->second = new demand_grammar;

    for(auto & def : *pListDefines)
        result = merge(result, def->transformDemand(symbolic_demand));

    for(auto & func : function_call_demands)
    {
        result->first->emplace(PREFIX_DEMAND + SEPARATOR + func.first, func.second);
        gLivenessData[PREFIX_DEMAND + SEPARATOR + func.first] = func.second;
    }

    return result;
}
