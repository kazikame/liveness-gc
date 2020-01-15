#include "SchemeAST.h"
#include "DemandStructure.h"
#include <iostream>
#include <typeinfo>
#include <set>

using namespace Scheme::AST;
using namespace Scheme::Demands;

ProgramLiveness progLiveness;
ProgramLiveness functionCallDemands;
ArgumentsOrder argumentsOrder;
ProgramLiveness2 progLiveness2;
unordered_map<string, EdgeSet> Scheme::AST::revCallGraph;

void Scheme::AST::printRevCallGraph()
{
    cout<<"\n\nPrinting the reverse call graph:-\n\n";
    for (auto i : Scheme::AST::revCallGraph)
    {
        cout<<i.first<<": ";
        for (auto j : i.second)
        {
            cout<<j<<", ";
        }
        cout<<'\n';
    }
    cout<<"\n\n";
}

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
        // auto i = returnValue.begin();
        returnValue[pArg->getIDStr()].catOne();
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
    { 

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
    LivenessInformation bodyLiveness = pBody->transformDemand();
    LivenessInformation temp = bodyLiveness;
    //std::cout<<"Variable Checking:"<<pID->getIDStr()<<endl;
    LivenessInformation exprLiveness = pExpr->transformDemand();
    LivenessTable varLiveness = bodyLiveness[pID->getIDStr()];
    LivenessInformation mappedLiveness = mapLiveness(varLiveness,exprLiveness);
    doUnion(temp, mappedLiveness);
    if (pID->getIDStr() == "_var14")
    {
        cout<<bodyLiveness<<endl;
        cout<<mappedLiveness<<endl;
    }
    if(pExpr->isConsExpression())
    {  
        string label = "L/" + pExpr->getLabel();
        progLiveness[label] = temp;
    }
    if(pExpr->isFunctionCallExpression())
    {  
        string label = "L/" + pBody->getLabel();
        progLiveness[label] = bodyLiveness;
        progLiveness[label][""] = progLiveness[label][pID->getIDStr()];
        // progLiveness[label].erase(pID->getIDStr());
    }
    temp.erase(pID->getIDStr());

    // cout<<getLabel()<<' '<<pID->getIDStr()<<endl<<temp<<endl;
    return temp;
}


/* D( (f x1 x2 ... xn), s, T) = T_1(x1, s) + T_2(x2, s) + ... + T_n(xn, s) */
LivenessInformation FuncExprNode::transformDemand() const
{
    LivenessInformation demandTransform;
    LivenessInformation temp = functionCallDemands[pID->getIDStr()];// Works correctly
    const auto& variableOrder = argumentsOrder[pID->getIDStr()];
    // cout<<demandTransform<<"<---demand transformer"<<endl;
    auto iter = pListArgs->begin();
    for (const auto& i: variableOrder)
    {
        IdExprNode* ie = (IdExprNode*) *iter;
        demandTransform[ie->getIDStr()] = temp[i];
        demandTransform[ie->getIDStr()].varName = ie->getIDStr();
        iter++;

        // cout<<ie->getIDStr()<<"<---actual varname"<<endl;
    }

    return demandTransform;
}


LivenessInformation DefineNode::transformDemand() const {
    // Check if totalLiveness contains only the demands of formal parameters    -   ANF?
    LivenessInformation totalLiveness = pExpr->transformDemand();
    vector<string> order;
    LivenessInformation livenessInOrder;
    for (auto i : *pListArgs)
    {
       // std::cout<<"Variable checking in define:"<<i->getIDStr()<<endl;
        auto iter = totalLiveness.find(i->getIDStr());// Works fine
        order.push_back(i->getIDStr());
        if (iter != totalLiveness.end())
        {
            livenessInOrder.insert(*iter);
        }
    }
    functionCallDemands[pID->getIDStr()] = livenessInOrder;
    argumentsOrder[pID->getIDStr()] = order;
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

    //INITIALIZATION OF FUNCTION CALL DEMANDS DONE?
    // revCallGraph = makeRevCallGraph();
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

        workList.erase(func);

        LivenessInformation oldInfo = functionCallDemands[func->getFunctionName()];
        LivenessInformation newInfo = func->transformDemand();
        if(!(oldInfo == newInfo))
        {
            auto i=revCallGraph[func->getFunctionName()].begin();
            for(;i!=revCallGraph[func->getFunctionName()].end();i++)
            {
                // cout<<"Inside graph loop"<<endl;
                workList.insert(funcNametoNode[*i]);
            }
        }
        

    }  




    // For testing only
    // std::cout<<"\n\nPrinting functionCallDemands...\n\n";
    // std::cout<<functionCallDemands;
    // std::cout<<"Transformations OK-----"<<'\n';
    
    LivenessInformation mainBodyLiveness = pExpr->transformDemand();

    // std::cout<<"\n\nPrinting progLiveness...\n";
    // std::cout<<progLiveness<<'\n';
    // std::cout<<"progLiveness OK-----\n\n\n";

    for (auto i : progLiveness)
    {
        for (auto j : i.second)
        {
            progLiveness2[i.first][j.first] = j.second;
        }
    }
    return mainBodyLiveness;

}
