#include "SchemeAST_new.h"
#include "DemandStructure.h"
#include<iostream>
#include <typeinfo>
#include <set>

using namespace Scheme::AST;
using namespace Scheme::Demands;

ProgramLiveness progLiveness;
ProgramLiveness functionCallDemands;
//demand_grammar function_call_demands;
//demand_grammar gLivenessData; //Global map which stores the liveness at cons points and the next statement after a function call only.


/* D( (return x), s, T ) = x.s  */
LivenessInformation ReturnExprNode::transformDemand() const
{
	LivenessInformation returnValue;
    LivenessTable l= LivenessTable(pID->getIDStr(), true);
    returnValue[pID->getIDStr()] = l;
    return returnValue;
}


/* ref( (id x), s, T ) = x.s  */
// Function calls?
LivenessInformation IdExprNode::transformDemand() const
{

	LivenessInformation returnValue;
    LivenessTable l= LivenessTable(getIDStr(), true);
    returnValue[getIDStr()] = l;
    return returnValue;
}


/* ref( k, s, T ) = {k.s}  */
LivenessInformation ConstExprNode::transformDemand() const
{
    LivenessInformation returnValue;
    return returnValue;

}


/* ref( (car x), s, T) = x.(0 s) U {x.eps}
   ref( (cdr x), s, T) = x.(1 s)
   ref( (null? x), s, T) = x.s     This should have been x.eps. Why are we putting the demand s on x?        */
LivenessInformation UnaryPrimExprNode::transformDemand() const
{

    if(node_name == "car")
    { 
        LivenessInformation returnValue = pArg->transformDemand();
        auto i=returnValue.begin();
		returnValue[i->first].catZero();
        //std::cout<<"Variable checking"<<i->first<<endl;
        return returnValue;
    }

    else if(node_name == "cdr")
    { 
        LivenessInformation returnValue = pArg->transformDemand();
        auto i=returnValue.begin();
		returnValue[i->first].catOne();
        //std::cout<<"Variable checking"<<i->first<<endl;
        return returnValue;
    }
    else
    {
        LivenessInformation returnValue = LivenessInformation();
        returnValue[pArg->getIDStr()] = LivenessTable(pArg->getIDStr(), false);
        auto i=returnValue.begin();
        returnValue[i->first] = returnValue[i->first] + EPSILON;
        //std::cout<<"Variable checking"<<i->first<<endl;
        return returnValue; 

    }
}


/* ref( (cons x y), s, T) = x.(0b s) + y.(1b s)
   ref( (eq? x y), s, T) = x.s + y.s            */
LivenessInformation BinaryPrimExprNode::transformDemand() const
{

    if(node_name == "cons")
    { /* x.(0b s) + y.(1b s) */
    	// for(auto p : demand)
     //    {
     //        p.push_front(T0b);
     //        arg_1_demand.insert(p);
     //    }
     //    for(auto p : demand) {
     //        p.push_front(T1b);
     //        arg_2_demand.insert(p);
     //    }

    	LivenessInformation arg1Liveness = pArg1->transformDemand();
        LivenessInformation arg2Liveness = pArg2->transformDemand();

        if (!arg1Liveness.empty())
        {
            auto i = arg1Liveness.begin();
			arg1Liveness[i->first].stripZero();
        //     std::cout<<"Checking..1"<<endl;
        // std::cout<<arg1Liveness<<endl;;
        }

        if (!arg2Liveness.empty())
        {
            auto i = arg2Liveness.begin();
			arg2Liveness[i->first].stripOne();
        //     std::cout<<"Checking..2"<<endl;
        // std::cout<<arg1Liveness<<endl;;
        }
        //  std::cout<<"Before union"<<endl;
        // std::cout<<arg1Liveness<<endl;;
        // std::cout<<arg2Liveness;
        
        doUnion(arg1Liveness, arg2Liveness);// Union updates first argument
        // std::cout<<"After union"<<endl;
        // std::cout<<arg1Liveness;
        return arg1Liveness;////CHECK IN CASE OF ERRORS
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
    	// path p;
    	// p.push_front(E);
    	// arg_1_demand.insert(p);
    	// arg_2_demand.insert(p);

        LivenessInformation arg1Liveness =  LivenessInformation();
    	arg1Liveness[pArg1->getIDStr()] = LivenessTable(pArg1->getIDStr(), false);
        LivenessInformation arg2Liveness =  LivenessInformation();
    	arg2Liveness[pArg2->getIDStr()] = LivenessTable(pArg2->getIDStr(), false);
        if (!arg1Liveness.empty())
        {
            auto i = arg1Liveness.begin();
            arg1Liveness[i->first]= arg1Liveness[i->first] + EPSILON;
        }

        if (!arg2Liveness.empty())
        {
            auto i = arg2Liveness.begin();
            arg2Liveness[i->first] = arg2Liveness[i->first] + EPSILON;
        }
        // std::cout<<"Before union"<<endl;
        // std::cout<<arg1Liveness<<endl;;
        // std::cout<<arg2Liveness;
        doUnion(arg1Liveness, arg2Liveness);// Union updates first argument
        // std::cout<<"After union"<<endl;
        // std::cout<<arg1Liveness;
        return arg1Liveness;

    }

    // LivenessInformation result =  merge(pArg1->transformDemand(arg_1_demand),
    //                                        pArg2->transformDemand(arg_2_demand));
    // result->first->emplace(label, demand);

    // return result;
}


/* D( (if x e1 e2), s, T) = D(e1, s, T) + D(e2, s, T) + x.(Xb s) */
LivenessInformation IfExprNode::transformDemand() const
{
    //rule cond_demand; // TODO Add an epsilon demand instead of Xb demand.
     //We don't need the Xb addition for liveness GC for strict languages
//    for(auto p : demand) {
//        p.push_front(TXb);
//        cond_demand.insert(p);
//    }
    // path p;
    // p.push_front(E);
    // cond_demand.insert(p);
    // LivenessInformation result =  merge(pCond->transformDemand(cond_demand),
    //                                        merge(pThen->transformDemand(demand),
    //                                              pElse->transformDemand(demand)));
    // result->first->emplace(label, demand);

    LivenessInformation returnValue = pCond->transformDemand();
    //cout<<"Inside IfExprNode transformDemand:\n\n"<<"pCond:\n"<<returnValue<<'\n';
    LivenessInformation temp = pThen->transformDemand();
    //cout<<"pThen:\n"<<temp<<'\n';
    LivenessInformation elseExpr = pElse->transformDemand();
    //cout<<"pElse:\n"<<elseExpr<<'\n';
    doUnion(temp, elseExpr);
    //cout<<"Union:\n"<<temp;
    doUnion(returnValue,temp);
    //cout<<"Union with cond:\n"<<returnValue;
    return returnValue;
}


/* D( (let x <- st in e), s, T) = E - E(x) + ref(st, E(x), T) where E = D(e, s, T) */
LivenessInformation LetExprNode::transformDemand() const
{
	// LivenessInformation result = pBody->transformDemand(demand);

	// if (pExpr->isFunctionCallExpression())
	// {
	// 	demand_grammar* var_grammar = result->second;
	// 	for (auto &var : (*var_grammar))
	// 	{
	// 		string nt = "L/" + pBody->getLabel() +  "/" + var.first;
	// 		gLivenessData[nt] = var.second;
	// 		//((FuncExprNode*)pExpr)->setNextExpr(pBody->getLabel());
	// 	}
	// }

 //    std::string let_var = pID->getIDStr();
 //    auto let_var_demand_it = result->second->find(let_var);

 //    if(let_var_demand_it != result->second->end())
 //    {
 //        rule let_var_demand = let_var_demand_it->second;
 //        result->second->erase(let_var_demand_it);
 //        expr_demand_grammars* let_body = pExpr->transformDemand(let_var_demand);

 //        result = merge(result, let_body);
 //        if (pExpr->isConsExpression())
 //        {
 //        	demand_grammar* var_grammar = result->second;
 //        	for (auto &var : (*var_grammar))
 //        	{
 //        		string nt = "L/" + pExpr->getLabel() +  "/" + var.first;
 //        		gLivenessData[nt] = var.second;
 //        	}
 //        }
 //    }

 //    result->first->emplace(label, demand);


 //    return result;
    LivenessInformation bodyLiveness = pBody->transformDemand();
    //std::cout<<"Variable Checking:"<<pID->getIDStr()<<endl;
    if (bodyLiveness.find(pID->getIDStr()) != bodyLiveness.end())
    {
        LivenessInformation exprLiveness = pExpr->transformDemand();
        LivenessTable varLiveness = bodyLiveness[pID->getIDStr()];
        LivenessInformation mappedLiveness = mapLiveness(varLiveness,exprLiveness);
        bodyLiveness.erase(pID->getIDStr());
        doUnion(bodyLiveness, mappedLiveness);
        if(pExpr->isConsExpression() || pExpr->isFunctionCallExpression())
        {  
            string label = "L/" + pExpr->getLabel();
            progLiveness[label] = bodyLiveness;
        }
        return bodyLiveness;

    }

    else
        return bodyLiveness;


}


/* D( (f x1 x2 ... xn), s, T) = T_1(x1, s) + T_2(x2, s) + ... + T_n(xn, s) */
LivenessInformation FuncExprNode::transformDemand() const
{
   // // TODO :Have to change it to have both IF and DF part
	// rule temp_demand = demand;
 //    if(function_call_demands.find(pID->getIDStr()) != function_call_demands.end())
 //        function_call_demands[pID->getIDStr()].insert(temp_demand.begin(), temp_demand.end());
 //    else
 //        function_call_demands[pID->getIDStr()] =  demand;

 //    if(pListArgs->size())
 //    {
 //        unsigned index = 1;
 //        std::string prefix = PREFIX_TRANSFORMER + SEPARATOR + pID->getIDStr() + SEPARATOR;

 //        rule arg_demand;
 //        //This should be done for the DF's only
 //        for(auto p : demand)
 //        {
 //            p.push_front(prefix + std::to_string(index));
 //            arg_demand.insert(p);
 //        }

 //        auto iter = pListArgs->begin();
 //        auto result = (*iter)->transformDemand(arg_demand);

 //        while(++iter != pListArgs->end())
 //        {
 //            ++index;
 //            arg_demand.clear();
 //            for(auto p : demand) {
 //                p.push_front(prefix + std::to_string(index));
 //                arg_demand.insert(p);
 //            }
 //            result = merge(result, (*iter)->transformDemand(arg_demand));
 //        }

 //        result->first->emplace(label, demand);

 //        return result;
 //    }
 //    else
 //    {

 //    	return new expr_demand_grammars({new demand_grammar({{label, demand}}), new demand_grammar});
 //    }
   // std::cout<<"Variable checking on function call demand:"<<pID->getIDStr()<<endl;
    LivenessInformation demandTransform = functionCallDemands[pID->getIDStr()];// Works correctly

    auto iter = pListArgs->begin();
    for (auto& i: demandTransform)
    {
        IdExprNode* ie = *iter;

        i.first = ie->getIDStr();//Does not work
        iter++;
    }

    return demandTransform;
}


LivenessInformation DefineNode::transformDemand() const {
    // Check if totalLiveness contains only the demands of formal parameters    -   ANF?
    LivenessInformation totalLiveness = pExpr->transformDemand();

    LivenessInformation livenessInOrder;
    for (auto i : *pListArgs)
    {
       // std::cout<<"Variable checking in define:"<<i->getIDStr()<<endl;
        auto iter = totalLiveness.find(i->getIDStr());// Works fine

        if (iter != totalLiveness.end())
        {
            livenessInOrder.insert(*iter);
        }
    }
    functionCallDemands[pID->getIDStr()] = livenessInOrder;

    return livenessInOrder;


}



void DefineNode::init() {
    // Check if totalLiveness contains only the demands of formal parameters    -   ANF?
    //LivenessInformation totalLiveness = pExpr->transformDemand();

   // LivenessInformation livenessInOrder;
    for (auto i : *pListArgs)
    {
       // LivenessTable LivenessTable(i->getIDStr())
       // std::cout<<"Variable checking in define:"<<i->getIDStr()<<endl;
        functionCallDemands[pID->getIDStr()][i->getIDStr()] = LivenessTable(i->getIDStr(),false);
    }
    


}


LivenessInformation ProgramNode::transformDemand() const {
    // // The demand argument is unnecessary for this function.

    // function_call_demands.clear();

    // rule symbolic_demand = rule({{ PREFIX_DEMAND + SEPARATOR + "all" }});
    // LivenessInformation result = pExpr->transformDemand(symbolic_demand);
    // gLivenessData[PREFIX_DEMAND + SEPARATOR + "all" ] = rule({{"0", PREFIX_DEMAND + SEPARATOR + "all" }, {"1", PREFIX_DEMAND + SEPARATOR + "all" },{E}});

    // delete result->second;
    // result->second = new demand_grammar;

    // for(auto & def : *pListDefines)
    //     result = merge(result, def->transformDemand(symbolic_demand));

    // for(auto & func : function_call_demands)
    // {
    //     result->first->emplace(PREFIX_DEMAND + SEPARATOR + func.first, func.second);
    //     gLivenessData[PREFIX_DEMAND + SEPARATOR + func.first] = func.second;
    // }

    // return result;

    //INITIALIZATION OF FUNCTION CALL DEMANDS DONE?
    revCallGraph = this->makeRevCallGraph();
    map<std::string, DefineNode*> funcNametoNode;
    for (auto& def : *pListDefines) 
     {  
        funcNametoNode[def->getFunctionName()]= def;
         def->init();
     }

    

    set<DefineNode*> workList;
    for (auto& def : *pListDefines)
    {
        workList.insert(def);
    }  

    while(!workList.empty())
    {
        DefineNode* func = *workList.begin();
        workList.remove(func);
        LivenessInformation oldInfo = functionCallDemands[func->getFunctionName()];
        LivenessInformation newInfo = func->transformDemand();
        if(!(oldInfo == newInfo))
        {
            auto i=revCallGraph[func->getFunctionName()].begin();
            for(;i!=revCallGraph[func->getFunctionName()].end();i++)
            {
                workList.insert(funcNametoNode[*i]);
            }
        }

    }  




    // For testing only
    std::cout<<"\n\nPrinting functionCallDemands...\n\n";
    std::cout<<functionCallDemands;
    std::cout<<"Transformations OK-----"<<'\n';
    
    LivenessInformation mainBodyLiveness = pExpr->transformDemand();

    std::cout<<"\n\nPrinting progLiveness...\n";
    std::cout<<progLiveness<<'\n';
    std::cout<<"progLiveness OK-----\n\n\n";

    return mainBodyLiveness;

}
