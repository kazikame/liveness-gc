#include "SchemeAST.h"
#include <map>
#include <string>
using namespace Scheme::AST;
using namespace Scheme::Demands;
using namespace std;


demand_grammar function_call_demands;
demand_grammar gLivenessData;
unordered_map<string, expr_demand_grammars*> gLivenessMap;
unordered_map<string, demand_grammar*> localLivenessMap;
unordered_map<string, const Node*> prog_pt_map;
unordered_map<string, unordered_set<string>> liveness_set_map;



std::string current_label;



/////////////////////////////////////////////////////////
// the following variables are used to compute the liveness for user defined functions
unordered_map<string,  const Node*> gfunc_prog_pts;

unordered_map<string, unsigned int> func_heap_cell_reqd;
bool in_function_define = false;





void print_label_set(unordered_set<string> s)
{
	cout << "Printing label set " << endl;
	for(auto l : s)
	  std::cout << l << std::endl;
}

/* D( (return x), s, T ) = x.s  */
unordered_map<string, expr_demand_grammars*> ReturnExprNode::transformDemand(const rule & demand)
{

     //Create a new label corresponding to the return statement
	if (in_function_define)
		gfunc_prog_pts[getLabel()] = this;
	else
		prog_pt_map[getLabel()] = this;

	gLivenessMap[label] =  pID->transformDemandRef(demand);
	
	localLivenessMap[getLabel()] = new demand_grammar({{pID->getIDStr(), demand }});


	label_set.insert(label);
	pID->label_set.insert(label);

	heap_cells_required = 0;

	current_label = label;

	live_var_set.insert(pID->getIDStr());

	liveness_set_map[label] = live_var_set;

	return gLivenessMap;
}


/* ref( (id x), s, T ) = x.s  */
expr_demand_grammars * IdExprNode::transformDemandRef(const rule & demand)
{

 	auto retval = new expr_demand_grammars({ new demand_grammar({{ label, demand }}),
        new demand_grammar({{ *pID, demand }})
    });

 	heap_cells_required = 1;
	return retval;
}


/* ref( k, s, T ) = {k.s}  */
expr_demand_grammars * ConstExprNode::transformDemandRef(const rule & demand)
{

	heap_cells_required = 1;
	return new expr_demand_grammars({ new demand_grammar({{ label, demand }}), new demand_grammar({{ }}) }); //TODO: verify this rule

}


bool UnaryPrimExprNode::isExpressionRecursive(const std::string var) const
{
	bool isRecursive = false;
	std::string argString = ((IdExprNode*)pArg)->getIDStr();
	if (0==argString.compare(var))
		isRecursive = true;


	return isRecursive;
}


/* ref( (car x), s, T) = x.(0 s) U {x.eps}
   ref( (cdr x), s, T) = x.(1 s)
   ref( (null? x), s, T) = x.s     This should have been x.eps. Why are we putting the demand s on x?        */
expr_demand_grammars * UnaryPrimExprNode::transformDemandRef(const rule & demand)
{
    rule arg_demand;

    if(node_name == "car")
    { /* x.(0 s) */
        for(auto p : demand)
        {
            p.push_front(T0);
            arg_demand.insert(p);
        }
        //Add Xb demand
        path p;
        p.push_front(TXb);
        arg_demand.insert(p);

    }
    else if(node_name == "cdr")
    { /* x.(1 s) */
        for(auto p : demand) {
            p.push_front(T1);
            arg_demand.insert(p);
        }
        //Add Xb demand
        path p;
        p.push_front(TXb);
        arg_demand.insert(p);
    }
    else
    {
        /* x.s */  /* should be x.eps */
    	//TODO : Should we add a epsilon demand or a Xb demand here?
    	path p;
    	p.push_front(TXb);
    	arg_demand.insert(p);
    }

    expr_demand_grammars * result = pArg->transformDemandRef(arg_demand);
    result->first->emplace(label, demand);

    //TODO: This can be simplified to a map<label, rule>
    localLivenessMap[getLabel() + "/" + current_label ] = new demand_grammar({{ "1", arg_demand }});
    cout << "Added liveness for argument " << getLabel() + "/" + current_label + "/"  + to_string(1) << endl;
    // demand_grammar* var_grammar = result->second;

    label_set.insert(pArg->label_set.begin(), pArg->label_set.end());

    live_var_set.insert(((IdExprNode*)pArg)->getLabel());


    heap_cells_required = 1;
    return result;
}

bool BinaryPrimExprNode::isExpressionRecursive(const std::string var) const
{
	bool isRecursive = false;
	std::string arg1String = ((IdExprNode*)pArg1)->getIDStr();
	if (0==arg1String.compare(var))
		isRecursive = true;
	else
	{
		std::string arg2String = ((IdExprNode*)pArg2)->getIDStr();
		if (0==arg2String.compare(var))
			isRecursive = true;
	}
	return isRecursive;
}


/* ref( (cons x y), s, T) = x.(0b s) + y.(1b s)
   ref( (eq? x y), s, T) = x.s + y.s            */
expr_demand_grammars * BinaryPrimExprNode::transformDemandRef(const rule & demand)
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
    	//Inserting epsilon demands for arguments for primitive functions other than cons
    	path p;
    	p.push_front(E);
    	arg_1_demand.insert(p);
    	arg_2_demand.insert(p);
    }
    
    expr_demand_grammars * result =  merge(pArg1->transformDemandRef(arg_1_demand),
                                           pArg2->transformDemandRef(arg_2_demand));
    result->first->emplace(label, demand);
    

    localLivenessMap[getLabel() + "/" + current_label ] = new demand_grammar({{ "1", arg_1_demand }});
    cout << "Added liveness for argument " << getLabel() + "/" + current_label + "/"  + to_string(1) << endl;
    localLivenessMap[getLabel() + "/" + current_label ] = new demand_grammar({{ "2", arg_2_demand }});
    cout << "Added liveness for argument " << getLabel() + "/" + current_label + "/"  + to_string(2) << endl;


    cout << "Inserting to liveness set " << (((IdExprNode*)pArg1)->getLabel()) << endl;
    live_var_set.insert(((IdExprNode*)pArg1)->getLabel());
    live_var_set.insert(((IdExprNode*)pArg2)->getLabel());

    heap_cells_required = 2;
    return result;
 
}


//TODO : Return type of these functions will change from expr_demand_grammars to a map :: label -> expr_demand_grammars

/* D( (if x e1 e2), s, T) = D(e1, s, T) + D(e2, s, T) + x.(Xb s) */
unordered_map<string, expr_demand_grammars*> IfExprNode::transformDemand(const rule & demand)
{
    rule cond_demand;

    if (in_function_define)
    	gfunc_prog_pts[getLabel()] = this;
    else
    	prog_pt_map[getLabel()] = this;
    
    for(auto p : demand)
    {
    	p.push_front(TXb);
    	cond_demand.insert(p);
    }


    //Create a new label and insert it in the map, along with the liveness for the condition variable
    string cond_variable = ((IdExprNode*)pCond)->getIDStr();

    pThen->transformDemand(demand);
    pElse->transformDemand(demand);


   auto result = new expr_demand_grammars({ new demand_grammar({{ label, rule({path({TXb})})}}),
            new demand_grammar({{ cond_variable, rule({path({TXb})})}})});


    //Update the liveness map to hold the liveness corresponding to the condition variable
   	gLivenessMap[label] = result;


    label_set.insert(label);
    label_set.insert(pThen->label_set.begin(), pThen->label_set.end());
    label_set.insert(pElse->label_set.begin(), pElse->label_set.end());

    current_label = label;
    //TODO: How do I insert the liveness of variables at this point?
    //Merge the liveness of then, else and condition and add it to liveness map



    live_var_set.insert(pThen->live_var_set.begin(), pThen->live_var_set.end());
    live_var_set.insert(pElse->live_var_set.begin(), pElse->live_var_set.end());
    live_var_set.insert(cond_variable);

    liveness_set_map[label] = live_var_set;

    heap_cells_required = (pThen->heap_cells_required > pElse->heap_cells_required) ?
    		                        pThen->heap_cells_required
								  : pElse->heap_cells_required;

    return gLivenessMap;
}



//This needs to change to handle multiple labels. How are the labels returned?
//Are they stored at each program point?
//Let, If and Return nodes will change, rest remain unchanged.


/* D( (let x <- st in e), s, T) = E - E(x) + ref(st, E(x), T) where E = D(e, s, T) */
unordered_map<string, expr_demand_grammars*> LetExprNode::transformDemand(const rule & demand)
{
	auto result = pBody->transformDemand(demand);

	/*if (in_function_define)
		gfunc_prog_pts[getLabel()] = this;
	else
		prog_pt_map[getLabel()] = this;
*/
	
	//Check if the let definition is recursive and if true create a function "f" and
	//get its transformer
	std::string let_var = pID->getIDStr();

	bool isLetExprRecursive = pExpr->isExpressionRecursive(let_var);
	if (isLetExprRecursive)
	{
		//Create required function and LFs corresponding to the let expr
		cout << "Recursive expression at " << getLabel() << endl;
	}


	for (auto l: pBody->label_set)
	{

		if (isLetExprRecursive)
		{
			//Use the LF of the function created to get the liveness of the variables in the expr.
		}
		else
		{
			//Call the transformDemandRef function to get the liveness
			//auto let_var_demand_it = gLivenessMap[l]->second->find(let_var);
			current_label = l;
			auto curr_expr_gram = gLivenessMap[l];

			//Process demand only if the let variable has a non-null demand
			auto dem_gram = *(curr_expr_gram->second);
			if (dem_gram.find(let_var) != dem_gram.end())
			{
				auto let_var_demand_it = dem_gram[let_var];
				//auto let_var_demand_it = rule({path({TXb})});
				auto let_expr_demand = pExpr->transformDemandRef(let_var_demand_it);

				//Merge the new demands with the existing demands
				expr_demand_grammars* res = merge(gLivenessMap[l], let_expr_demand);
				gLivenessMap[l] = res;


			}

		}

	}
	//Kill the liveness of the variable after calculating liveness
	live_var_set.insert(pExpr->live_var_set.begin(), pExpr->live_var_set.end());
	live_var_set.insert(pBody->live_var_set.begin(), pBody->live_var_set.end());

	cout << "Deleting from liveness set "<< getVar() << endl;
	live_var_set.erase(getVar());
	cout << "Deleted from liveness set "<< getVar() << endl;

	cout << "Set of live variables at pgm_pt " << let_var << endl;
	for (auto live_var:live_var_set)
	{
		cout << live_var << ",";
	}
	cout << endl;


	//Copy the label set
	label_set.insert(pBody->label_set.begin(), pBody->label_set.end());

	heap_cells_required = pBody->heap_cells_required + pExpr->heap_cells_required;

	return gLivenessMap;
}


bool FuncExprNode::isExpressionRecursive(const std::string var) const
{
	bool isRecursive = false;
	if(pListArgs->size())
	{

		 auto iter = pListArgs->begin();
		 while(++iter != pListArgs->end())
		 {
			 std::string argName = ((IdExprNode*)*iter)->getIDStr();
			 if (0==argName.compare(var))
			 {
				 isRecursive = true;
				 break;
			 }
		 }
	}
	return isRecursive;
}


/* D( (f x1 x2 ... xn), s, T) = T_1(x1, s) + T_2(x2, s) + ... + T_n(xn, s) */
expr_demand_grammars * FuncExprNode::transformDemandRef(const rule & demand)
{
   	rule temp_demand = demand;

    if(function_call_demands.find(pID->getIDStr()) != function_call_demands.end())
        function_call_demands[pID->getIDStr()].insert(temp_demand.begin(), temp_demand.end());
    else
        function_call_demands[pID->getIDStr()] =  demand;

    if(pListArgs->size())
    {
        unsigned index = 1;
        std::string prefix = PREFIX_TRANSFORMER + SEPARATOR + pID->getIDStr() + SEPARATOR;
        //std::string prefix = "D" + SEPARATOR + pID->getIDStr() + SEPARATOR;
        rule arg_demand;
        //This should be done for the DF's only
        for(auto p : demand)
        {
            p.push_front(prefix + std::to_string(index));
            arg_demand.insert(p);
        }

        auto iter = pListArgs->begin();
        auto result = (*iter)->transformDemandRef(arg_demand);


        //TODO: How do we disambiguate the labels for each argument?
        localLivenessMap[getLabel() + "/" + current_label] = new demand_grammar({{ to_string(index), arg_demand }});
        cout << "Added liveness for argument " << getLabel() + "/" + current_label + "/"  + to_string(index) << endl;
        live_var_set.insert(((IdExprNode*)(*iter))->getLabel());


        while(++iter != pListArgs->end())
        {
            ++index;
            arg_demand.clear();
            for(auto p : demand) {
                p.push_front(prefix + std::to_string(index));
                arg_demand.insert(p);
            }
            localLivenessMap[getLabel() + "/" + current_label ] = new demand_grammar({{ to_string(index), arg_demand }});
            result = merge(result, (*iter)->transformDemandRef(arg_demand));
            live_var_set.insert(((IdExprNode*)(*iter))->getLabel());
        }

        result->first->emplace(label, demand);

        heap_cells_required = pListArgs->size();

        return result;
    }
    else
    {
    	heap_cells_required = 1;
    	return new expr_demand_grammars({new demand_grammar({{label, demand}}), new demand_grammar});
    }
}




unordered_map<string, expr_demand_grammars*> DefineNode::transformDemand(const rule & demand)
{
    // The demand argument is unnecessary for this function.

	gfunc_prog_pts.clear();
	in_function_define = true;
    std::string func_demand_prefix = PREFIX_DEMAND + SEPARATOR + pID->getIDStr();

    expr_demand_grammars * result = new expr_demand_grammars({ new Scheme::Demands::demand_grammar, new Scheme::Demands::demand_grammar});
    unordered_map<string, expr_demand_grammars *> liveness_map;
    
    auto expr_map = pExpr->transformDemand(rule({{ func_demand_prefix }}));
    

    //cout << "Inserting prog pts for function " << pID->getIDStr() << endl;
    prog_pt_map.insert(gfunc_prog_pts.begin(), gfunc_prog_pts.end());
    //cout << "Number of prog pts in " << pID->getIDStr() << " = " << gfunc_prog_pts.size() << endl;

    //For each function point, get liveness for function variables and store it in a map
    for (auto p : gfunc_prog_pts)
    {
    	auto labels = p.second->label_set;
    	liveness_map[p.first] = new expr_demand_grammars({ new Scheme::Demands::demand_grammar, new Scheme::Demands::demand_grammar});
    	for(auto l : labels)
    	{
    		liveness_map[p.first] = merge(liveness_map[p.first], expr_map[l]);
    	}
    }

    
    auto &dem_grams = result->second; 
    //Process each variable
    for (auto p: liveness_map)
    {

    	Scheme::Demands::demand_grammar* var_gram = p.second->second;
    	for (auto g : (*var_gram))
    	{
    		std::string liveness_label = "L/" + p.first + "/" + g.first; //change it to LF
    		((*dem_grams)[liveness_label]).insert(g.second.begin(), g.second.end());
    	}
    
    
    	int index = 0;
    	demand_grammar * arg_demands = new demand_grammar;

    	for(auto & arg : *pListArgs)
    	{

    		auto arg_demand_pair = var_gram->find(arg->getIDStr());
    		if(arg_demand_pair != var_gram->end())
    		{

    			arg_demands->emplace(func_demand_prefix + SEPARATOR + std::to_string(++index),
    					arg_demand_pair->second);

    			(gLivenessData["T" + SEPARATOR + pID->getIDStr() + SEPARATOR + std::to_string(index)]).insert(arg_demand_pair->second.begin(), arg_demand_pair->second.end());

    		}
    		else
    		{
    			//This can happen when an argument to the function is not used in one of the branch.
    			//In such a case a null demand will be added.
    			//TODO : is there a better way?

    			rule null_demand;
    			((*arg_demands)[func_demand_prefix + SEPARATOR + std::to_string(++index)]).insert(null_demand.begin(), null_demand.end());

    		}

    	}
    }
    
//    for (auto &var : (*dem_grams))
//    {
//    	string nt = var.first;
//    	gLivenessData[nt].insert(var.second.begin(), var.second.end());
//
//
//    }

    in_function_define = false;

    heap_cells_required = this->pExpr->heap_cells_required;
    func_heap_cell_reqd[pID->getIDStr()] = this->pExpr->heap_cells_required;
    cout << "The number of heap cells required by " << pID->getIDStr() << " " << this->pExpr->heap_cells_required <<endl;

    return gLivenessMap;
}




unordered_map<string, expr_demand_grammars*> ProgramNode::transformDemand(const rule & demand)
{
    // The demand argument is unnecessary for this function.
	prog_pt_map[getLabel()] = this;
    function_call_demands.clear();

    rule symbolic_demand = rule({{ PREFIX_DEMAND + SEPARATOR + "all" }});



    expr_demand_grammars * result = new expr_demand_grammars({ new Scheme::Demands::demand_grammar({{ }}), new Scheme::Demands::demand_grammar({{ }})});
    auto main_expr_gram = pExpr->transformDemand(symbolic_demand);
    gLivenessMap.insert(main_expr_gram.begin(), main_expr_gram.end());

    for(auto & def : *pListDefines)
    {
    	auto define_map = def->transformDemand(symbolic_demand);
    }

    for(auto & func : function_call_demands)
    {
        result->first->emplace(PREFIX_DEMAND + SEPARATOR + func.first, func.second);
        gLivenessData[PREFIX_DEMAND + SEPARATOR + func.first].insert(func.second.begin(), func.second.end());
    }



    this->label_set.insert(pExpr->label_set.begin(), pExpr->label_set.end());
    this->progpt_map = &prog_pt_map;
    this->liveness_data = gLivenessData;


    return gLivenessMap;
}
