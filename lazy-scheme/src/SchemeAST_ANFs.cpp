#include "Utils.hpp"
#include "SchemeAST.h"

#include <sstream>
#include <cassert>

using namespace Scheme::AST;


bool isStatement(const ExprNode * pExpr)
{
    return  isOfClass<Node, ConstExprNode>(pExpr)   ||
            isOfClass<Node, PrimExprNode>(pExpr)        ||
            isOfClass<Node, FuncExprNode>(pExpr);
}


bool isExpression(const ExprNode * pExpr)
{
    return isOfClass<Node, ReturnExprNode>(pExpr)   ||
           isOfClass<Node, LetExprNode>(pExpr)         ||
           isOfClass<Node, IfExprNode>(pExpr);
}


unsigned long var_count = 0;

std::string getFreeVarName() {
    // TODO @ HIGH : Check for existing names in program
    std::stringstream sstream;
    sstream << "_var" << var_count++;
    return sstream.str();
}


ExprNode * Scheme::AST::pushDown(ExprNode * pHoleExpr, ExprNode * pANFExpr) {
    assert(isExpression(pANFExpr));
    if(isOfClass<Node, ReturnExprNode>(pANFExpr)) {
        ExprNode * resNode = NULL;
        IdExprNode * idNode = ((ReturnExprNode *)pANFExpr)->pID->clone();
        delete pANFExpr;
        if(isStatement(pHoleExpr)) {
            std::string newVar = getFreeVarName();
            if(isOfClass<Node, UnaryPrimExprNode>(pHoleExpr))
                resNode = new LetExprNode(new IdExprNode(new std::string(newVar)),
                                          ((UnaryPrimExprNode *) pHoleExpr)->fillHoleWith(idNode),
                                          new ReturnExprNode(new IdExprNode(new std::string(newVar))));
            else if(isOfClass<Node, BinaryPrimExprNode>(pHoleExpr))
                resNode = new LetExprNode(new IdExprNode(new std::string(newVar)),
                                          ((BinaryPrimExprNode *) pHoleExpr)->fillHoleWith(idNode),
                                          new ReturnExprNode(new IdExprNode(new std::string(newVar))));
            else if(isOfClass<Node, FuncExprNode>(pHoleExpr))
                resNode = new LetExprNode(new IdExprNode(new std::string(newVar)),
                                          ((FuncExprNode *) pHoleExpr)->fillHoleWith(idNode),
                                          new ReturnExprNode(new IdExprNode(new std::string(newVar))));
            else {
                assert(false);
                return NULL;
            }
        } else {
            if(isOfClass<Node, LetExprNode>(pHoleExpr))
                resNode = ((LetExprNode *) pHoleExpr)->fillHoleWith(idNode);
            else if(isOfClass<Node, IfExprNode>(pHoleExpr))
                resNode = ((IfExprNode *) pHoleExpr)->fillHoleWith(idNode);
            else {
                assert(false);
                return NULL;
            }
        }
        return resNode;
    } else if(isOfClass<Node, LetExprNode>(pANFExpr)) {
        LetExprNode * pLetANFExpr = (LetExprNode *) pANFExpr;

        if(isOfClass<Node, LetExprNode>(pHoleExpr) &&
                isOfClass<Node, ReturnExprNode>(pLetANFExpr->pBody)) {
            ExprNode * resNode = ((LetExprNode *) pHoleExpr)->fillHoleWith(pLetANFExpr->pExpr->clone());
            delete pLetANFExpr;
            return resNode;
        }

        pLetANFExpr->pBody = pushDown(pHoleExpr, pLetANFExpr->pBody);
        return pLetANFExpr;
    } else if(isOfClass<Node, IfExprNode>(pANFExpr)) {
        IfExprNode * pIfANFExpr = (IfExprNode *) pANFExpr;
        pIfANFExpr->pThen = pushDown(pHoleExpr->clone(), pIfANFExpr->pThen);
        pIfANFExpr->pElse = pushDown(pHoleExpr, pIfANFExpr->pElse);
        return pIfANFExpr;
    }
    assert(false); // not expected to reach here!
    return NULL;
}


ExprNode * IdExprNode::getANF() const {
    return new ReturnExprNode(this->clone());
}

ReturnExprNode * ReturnExprNode::getANF() const {
    return this->clone();
}



LetExprNode * ConstExprNode::getANF() const {
    std::string newVar = getFreeVarName();
    return new LetExprNode(new IdExprNode(new std::string(newVar)),
                           this->clone(),
                           new ReturnExprNode(new IdExprNode(new std::string(newVar))));
}


ExprNode * IfExprNode::getANF() const {
    if(isOfClass<Node, IdExprNode>(pCond))
        return new IfExprNode(pCond->clone(), pThen->getANF(), pElse->getANF());

    cout << "The type of condition is " << pCond->type << " at " << pCond->getLabel() << endl;
    assert(false);
//    return pushDown(new IfExprNode(new HoleConstExprNode(),
//                                   pThen->getANF(),
//                                   pElse->getANF()),
//                    pCond->getANF());
}

IfExprNode * IfExprNode::fillHoleWith(IdExprNode * pSubExpr) {
    delete pCond;
    pCond = pSubExpr;
    return this;
}


ExprNode * LetExprNode::getANF() const {
    bool alreadyANFExpr = false;

    if(isOfClass<Node, ConstExprNode>(pExpr))
        alreadyANFExpr = true;
    else if(isOfClass<Node, UnaryPrimExprNode>(pExpr)
            && isOfClass<Node, IdExprNode>(((UnaryPrimExprNode *) pExpr)->pArg))
        alreadyANFExpr = true;
    else if(isOfClass<Node, BinaryPrimExprNode>(pExpr) &&
            isOfClass<Node, IdExprNode>(((BinaryPrimExprNode *) pExpr)->pArg1) &&
            isOfClass<Node, IdExprNode>(((BinaryPrimExprNode *) pExpr)->pArg2))
        alreadyANFExpr = true;
    else if(isOfClass<Node, FuncExprNode>(pExpr)) {
        FuncExprNode * pFuncExpr = (FuncExprNode *) pExpr;
        alreadyANFExpr = true;
        for(std::list<ExprNode *>::const_iterator i = pFuncExpr->pListArgs->begin();
                i != pFuncExpr->pListArgs->end(); ++i)
            alreadyANFExpr &= isOfClass<Node, IdExprNode>(* i);
    }

    if(alreadyANFExpr)
        return new LetExprNode(pID->clone(), pExpr->clone(), pBody->getANF());
//    return pushDown(new LetExprNode(pID->clone(), new HoleConstExprNode(), pBody->getANF()),
//                    pExpr->getANF());
    else //TODO:Added it to get infinte lazy list programs to work. Need to check if this is the correct thing to do
    	return new LetExprNode(pID->clone(), pExpr->clone(), pBody->getANF());
}

LetExprNode * LetExprNode::fillHoleWith(ExprNode * pSubExpr) {
    delete pExpr;
    pExpr = pSubExpr;
    return this;
}


ExprNode * UnaryPrimExprNode::getANF() const {
    if(isOfClass<Node, IdExprNode>(pArg)) {
        std::string newVar = getFreeVarName();
        return new LetExprNode(new IdExprNode(new std::string(newVar)), this->clone(),
                               new ReturnExprNode(new IdExprNode(new std::string(newVar))));
    }
    assert(false);
//    else
//        return pushDown(new UnaryPrimExprNode(node_name, new HoleConstExprNode()), pArg->getANF());
}

UnaryPrimExprNode * UnaryPrimExprNode::fillHoleWith(IdExprNode * id) {
    delete pArg;
    pArg = id;
    return this;
}


ExprNode * BinaryPrimExprNode::getANF() const {
    if(isOfClass<Node, IdExprNode>(pArg1)) {
        if(isOfClass<Node, IdExprNode>(pArg2)) {
            std::string newVar = getFreeVarName();
            return new LetExprNode(new IdExprNode(new std::string(newVar)), this->clone(),
                                   new ReturnExprNode(new IdExprNode(new std::string(newVar))));
        } else {
        	assert(false);
//            return pushDown(new BinaryPrimExprNode(node_name, pArg1->clone(), new HoleConstExprNode()),
//                            pArg2->getANF());
        }
    }
    assert(false);
//    else {
//        ExprNode * intermediateNode = pushDown(new BinaryPrimExprNode(node_name,
//                                               new HoleConstExprNode(), pArg2->clone()), pArg1->getANF());
//        ExprNode * resNode = intermediateNode->getANF();
//        delete intermediateNode;
//        return resNode;
//    }
}

BinaryPrimExprNode * BinaryPrimExprNode::fillHoleWith(IdExprNode * id) {
//    if(isOfClass<Node, HoleConstExprNode>(pArg1)) {
//        delete pArg1;
//        pArg1 = id;
//        return this;
//    } else
	{
        delete pArg2;
        pArg2 = id;
        return this;
    }
}


ExprNode * FuncExprNode::getANF() const {
    ExprNode * foundExpr = NULL;
    std::list<ExprNode *> * newArgs = new std::list<ExprNode *>();
    for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i) {
        if(foundExpr || isOfClass<Node, IdExprNode>(* i))
            newArgs->push_back((* i)->clone());
        else {
        	assert(false);
//            foundExpr = * i;
//            newArgs->push_back(new HoleConstExprNode());
        }
    }
    if(foundExpr) {
        ExprNode * intermediateNode = pushDown(new FuncExprNode(pID->clone(), newArgs),
                                               foundExpr->getANF());
        ExprNode * resNode = intermediateNode->getANF();
        delete intermediateNode;
        return resNode;
    } else {
        std::string newVar = getFreeVarName();
        return new LetExprNode(new IdExprNode(new std::string(newVar)), new FuncExprNode(pID->clone(),
                               newArgs),
                               new ReturnExprNode(new IdExprNode(new std::string(newVar))));

    }
}

FuncExprNode * FuncExprNode::fillHoleWith(IdExprNode * pSubExpr) {
    for(std::list<ExprNode *>::iterator i = pListArgs->begin(); i != pListArgs->end(); ++i) {
//        if(isOfClass<Node, HoleConstExprNode>(* i))
        {
            delete * i;
            * i = pSubExpr;
            return this;
        }
    }
    assert(false); // not expected to reach here!
    return NULL;
}


DefineNode * DefineNode::getANF() const
{
    std::list<IdExprNode *> * newArgs = new std::list<IdExprNode *>();
    for(std::list<IdExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
        newArgs->push_back((* i)->clone());
    return new DefineNode(pID->clone(), newArgs, pExpr->getANF());
}


ProgramNode * ProgramNode::getANF() const
{
    std::list<DefineNode *> * newDefines = new std::list<DefineNode *>();
    for(std::list<DefineNode *>::const_iterator i = pListDefines->begin(); i != pListDefines->end();
            ++i)
        newDefines->push_back((* i)->getANF());
    return new ProgramNode(newDefines, pExpr->getANF());
}




