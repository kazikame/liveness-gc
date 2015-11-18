//#include "SchemeAST.h"
#include <list>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <cstring>
#include <ctime>
#include <unordered_map>
#include <boost/variant.hpp>
#include<stack>
#include<fstream>
#include "Simulator.h"
#include "gc.h"
using namespace std;
using namespace Scheme::AST;

extern stack<cons*> update_heap_refs;
extern deque<actRec> actRecStack;
extern GCStatus gc_status;
vector<double> gc_times;
int conscount;
//extern int gcinvoke;
int maxcount;
//extern double gctime;
ProgramNode* pgm;
unsigned long lbl_count = 0;
map< string, vector<ExprNode*>> func_call_points;
unsigned int closure_count = 0;
unsigned int reduction_count = 0;
//extern unsigned int num_of_allocations;//
//extern map<cons*, int> heap_map;
//extern map<int, string> root_var_map;
extern unordered_map<string, unsigned int> func_heap_cell_reqd;
std::string curr_return_addr;


std::string getNextLabel()
{
	std::stringstream sstream;
	//sstream <<  "p#" << lbl_count++;
	sstream <<  lbl_count++;
	return sstream.str();
}

cons* reduceParamToWHNF(cons* cell)
{
	cons* heap_cell = update_heap_refs.top();

	if (!is_valid_address(heap_cell))
		cout << "Address invalid for " << heap_cell << endl;

	assert(is_valid_address(heap_cell));
	if (heap_cell->inWHNF)
	{
//		cout << "Heap cell " << heap_cell - getbufferlive() << " is in whnf" << endl;
//		cout << "Its type is " << heap_cell->typecell << endl;
		return heap_cell;
	}
	else
	{
		cons* retval = heap_cell;
		cons *temp = (retval->val.closure.expr->evaluate());
		retval=temp;
		//cout << "Type of heap_cell after reduction is " << temp->typecell << endl;
		assert( temp->typecell == consExprClosure ||
				temp->typecell == constIntExprClosure ||
				temp->typecell == constBoolExprClosure ||
				temp->typecell == constStringExprClosure ||
				temp->typecell == nilExprClosure);
		assert(retval->inWHNF);
		assert(is_valid_address(retval));

		heap_cell = update_heap_refs.top();
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = retval->inWHNF;

		return heap_cell;
	}
}



void updateEvaluationPoint(Node* node)
{
	//For each of the variables in the program stack top
		//if variable points to a closure, update closure
		//arguments to point to current prgrm point
		for(vector<var_heap>::iterator vhit = actRecStack.begin()->heapRefs.begin(); vhit != actRecStack.begin()->heapRefs.end(); ++vhit)
		{

			cons* heap_cell = (cons*)vhit->ref;
	//		DBG(cout << "Processing variable " << vhit->varname <<  " is_param " << vhit-> is_param << endl);
			if (vhit->is_param)
			{
	//			DBG(cout << "Processing a parameter, not updating prog point" << endl);
				continue;
			}

			if (heap_cell && !heap_cell->inWHNF)
			{
				switch(heap_cell->typecell)
				{
				case constIntExprClosure:
				case constBoolExprClosure:
				case constStringExprClosure:
				case nilExprClosure:
					break;
				case unaryprimopExprClosure:
				{
	//				cout << "Unaryprimop" << endl;
					auto arg_num = heap_cell->val.closure.arg1_name->substr(heap_cell->val.closure.arg1_name->rfind("/")+1);
					heap_cell->val.closure.arg1_name = new string(node->getLabel() + "/" + arg_num);
	//				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
					break;
				}
				case binaryprimopExprClosure:
				{
					assert(!heap_cell->inWHNF);
	//				cout << "Binaryprimop" << endl;
					auto arg_num = heap_cell->val.closure.arg1_name->substr(heap_cell->val.closure.arg1_name->rfind("/")+1);
					heap_cell->val.closure.arg1_name = new string(node->getLabel() + "/" + arg_num);
	//				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
					arg_num = heap_cell->val.closure.arg2_name->substr(heap_cell->val.closure.arg2_name->rfind("/")+1);
					heap_cell->val.closure.arg2_name = new string(node->getLabel() + "/" + arg_num);
	//				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
					break;
				}
				case funcApplicationExprClosure:
				{
	//				cout << "FuncApp" << endl;
					auto funcCallExpr = (FuncExprNode*)heap_cell->val.closure.expr;
					auto num_args = funcCallExpr->getNumArgs();
					if (num_args > 0)
					{
						auto curr_cell = heap_cell;
						while (num_args > 0)
						{
	//						cout << "Processing arg" << (funcCallExpr->getArgs().size() - num_args + 1) << endl;
							auto arg_num = curr_cell->val.closure.arg2_name->substr(curr_cell->val.closure.arg2_name->rfind("/")+1);
							curr_cell->val.closure.arg2_name = new string(node->getLabel() + "/" + arg_num);
	//						cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
							--num_args;
							curr_cell = curr_cell->val.closure.arg1;
						}
					}
					break;
				}

				break;
				default : cout << "Should not have come to this point"<<endl;
				cout << "Processing " << heap_cell << " with type " << heap_cell->typecell << endl;
				break;
				}
			}
		}

}



Node::~Node() {}

std::string Node::getLabel() const
{
	return label;
}

std::string IdExprNode::getLabel() const
{
	return label;
}

std::string ReturnExprNode::getLabel() const
{
	return this->pID->getLabel();
}

Node::Node(const std::string name) : type(defaultType), label(""), node_name(name) {}


ExprNode::~ExprNode() {}

ExprNode::ExprNode(const std::string name) : Node(name) {}


IdExprNode::~IdExprNode() {
	delete pID;
}

std::string IdExprNode::getIDStr() const {
	return *pID;
}

IdExprNode::IdExprNode(std::string * id) : ExprNode("ID"), pID(id) {}

IdExprNode * IdExprNode::clone() const
{
	return new IdExprNode(new std::string(*pID));
}

void IdExprNode::doLabel(bool shouldAddLabel)
{
	string l = getNextLabel();
	label = (shouldAddLabel ? l : "");
}

cons* IdExprNode::evaluate()
{
	std::string varName = *(this->pID);
	cons* retval = lookup_addr(this->pID->c_str());
			
	if (retval->inWHNF)
	{
		return retval;
	}
	else
	{

		update_heap_refs.push(retval);
		cons* temp = retval->val.closure.expr->evaluate();

		cons * heap_cell = update_heap_refs.top();
		update_heap_refs.pop();
		assert(is_valid_address(temp) && is_valid_address(heap_cell));
		assert(temp->inWHNF);

		heap_cell->inWHNF = true;
		heap_cell->typecell = temp->typecell;
		heap_cell->val = temp->val;

		return heap_cell;
	}
}

cons* IdExprNode::make_closure()
{
	return (cons*)lookup_addr(this->pID->c_str());
}


ReturnExprNode::~ReturnExprNode()
{
	delete pID;
}

ReturnExprNode::ReturnExprNode(IdExprNode * pId) : ExprNode("RETURN"), pID(pId) {}

void ReturnExprNode::doLabel(bool shouldAddLabel)
{
	pID->doLabel(shouldAddLabel);
	idVarLabel = pID->getLabel();
	label = pID->getLabel();
}

ReturnExprNode * ReturnExprNode::clone() const
{
	return new ReturnExprNode(pID->clone());
}

cons* ReturnExprNode::evaluate()
{

	curr_return_addr = getLabel();
//	DBG(cout << "At return updating curr_return_address to " << curr_return_addr << endl);
	std::string varName = this->pID->getName();
	cons* retval = (cons*)lookup_addr(varName.c_str());


	if (gc_status == gc_live)
	{
		updateEvaluationPoint(this);
	}


//
//
//	//For each of the variables in the program stack top
//	//if variable points to a closure, update closure
//	//arguments to point to current prgrm point
//	for(vector<var_heap>::iterator vhit = actRecStack.begin()->heapRefs.begin(); vhit != actRecStack.begin()->heapRefs.end(); ++vhit)
//	{
//
//		cons* heap_cell = (cons*)vhit->ref;
////		DBG(cout << "Processing variable " << vhit->varname <<  " is_param " << vhit-> is_param << endl);
//		if (vhit->is_param)
//		{
////			DBG(cout << "Processing a parameter, not updating prog point" << endl);
//			continue;
//		}
//
//		if (heap_cell && !heap_cell->inWHNF)
//		{
//			switch(heap_cell->typecell)
//			{
//			case constIntExprClosure:
//			case constBoolExprClosure:
//			case constStringExprClosure:
//			case nilExprClosure:
//				break;
//			case unaryprimopExprClosure:
//			{
////				cout << "Unaryprimop" << endl;
//				auto arg_num = heap_cell->val.closure.arg1_name->substr(heap_cell->val.closure.arg1_name->rfind("/")+1);
//				heap_cell->val.closure.arg1_name = new string(this->getLabel() + "/" + arg_num);
////				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//				break;
//			}
//			case binaryprimopExprClosure:
//			{
//				assert(!heap_cell->inWHNF);
////				cout << "Binaryprimop" << endl;
//				auto arg_num = heap_cell->val.closure.arg1_name->substr(heap_cell->val.closure.arg1_name->rfind("/")+1);
//				heap_cell->val.closure.arg1_name = new string(this->getLabel() + "/" + arg_num);
////				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//				arg_num = heap_cell->val.closure.arg2_name->substr(heap_cell->val.closure.arg2_name->rfind("/")+1);
//				heap_cell->val.closure.arg2_name = new string(this->getLabel() + "/" + arg_num);
////				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//				break;
//			}
//			case funcApplicationExprClosure:
//			{
////				cout << "FuncApp" << endl;
//				auto funcCallExpr = (FuncExprNode*)heap_cell->val.closure.expr;
//				auto num_args = funcCallExpr->getNumArgs();
//				if (num_args > 0)
//				{
//					auto curr_cell = heap_cell;
//					while (num_args > 0)
//					{
////						cout << "Processing arg" << (funcCallExpr->getArgs().size() - num_args + 1) << endl;
//						auto arg_num = curr_cell->val.closure.arg2_name->substr(curr_cell->val.closure.arg2_name->rfind("/")+1);
//						curr_cell->val.closure.arg2_name = new string(this->getLabel() + "/" + arg_num);
////						cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//						--num_args;
//						curr_cell = curr_cell->val.closure.arg1;
//					}
//				}
//				break;
//			}
//
//			break;
//			default : cout << "Should not have come to this point"<<endl;
//			cout << "Processing " << heap_cell << " with type " << heap_cell->typecell << endl;
//			break;
//			}
//		}
//	}

	if (!retval->inWHNF)
	{
		actRecStack.front().return_point = this->getLabel();
		update_heap_refs.push(retval);
		cons* temp = retval->val.closure.expr->evaluate();
		retval = update_heap_refs.top();

		assert(temp->inWHNF && retval->inWHNF);

		retval->inWHNF = true;
		retval->typecell = temp->typecell;
		retval->val = temp->val;

		update_heap_refs.pop();

	}
	
	return retval;
}
cons* ReturnExprNode::make_closure()
{
	cout << "Control should not have come here !!!!"<<endl;
	return pID->make_closure();//TODO: This should probably just evaluate
}

/*  Definitions for CONSTANT nodes */

ConstExprNode::~ConstExprNode() {}

ConstExprNode::ConstExprNode(const std::string name) : ExprNode(name) {}

void ConstExprNode::doLabel(bool shouldAddLabel) {
	label = (shouldAddLabel ? getNextLabel() : "");
}

NilConstExprNode::NilConstExprNode() : ConstExprNode("NIL") {}

NilConstExprNode * NilConstExprNode::clone() const
{
	return new NilConstExprNode();
}

cons* NilConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = nilExprClosure;
	retval->inWHNF = true;
	retval->closure_id = ++closure_count;
	return retval;
}

cons* NilConstExprNode::evaluate()
{
    //cons *heap_cell = NULL;
	assert(false && "TO BE FIXED -- Amey");
    cons *heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		if (!heap_cell->inWHNF)
			*heap_cell = *(heap_cell->val.closure.expr->evaluate());
		heap_cell->inWHNF = true;
		heap_cell->typecell = nilExprClosure;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
}



IntConstExprNode::~IntConstExprNode()
{
	delete pIntVal;
}

IntConstExprNode::IntConstExprNode(int * ival) : ConstExprNode("INT"), pIntVal(ival) {}

IntConstExprNode * IntConstExprNode::clone() const {
	return new IntConstExprNode(new int(*pIntVal));
}

cons* IntConstExprNode::evaluate()
{
	cons *heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
	{
        GC_STAT_UPDATE_LAST_USE(heap_cell);
		return heap_cell;
	}

	cons* retval = heap_cell;
	retval->typecell = constIntExprClosure;
	retval->val.intVal = *pIntVal;
	retval->inWHNF = true;
	retval->reduction_id = ++reduction_count;
    GC_STAT_UPDATE_LAST_USE(retval);
	return retval;
}

int IntConstExprNode::getVal()
{
	return *(this->pIntVal);

}
cons* IntConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = constIntExprClosure;
	retval->val.intVal = *pIntVal;
	retval->inWHNF = true;
	retval->closure_id = ++closure_count;
	return retval;
}

StrConstExprNode::~StrConstExprNode()
{
	delete pStrVal;
}

StrConstExprNode::StrConstExprNode(std::string * sval) : ConstExprNode("STR"), pStrVal(sval) {}

StrConstExprNode * StrConstExprNode::clone() const
{
	return new StrConstExprNode(new std::string(*pStrVal));
}

cons* StrConstExprNode::evaluate()
{
    cons* heap_cell = NULL;
    assert(0 && "ToBeDone-- Amey");
	if (heap_cell->inWHNF)
	{
        GC_STAT_UPDATE_LAST_USE(heap_cell);
		return heap_cell;
	}

	cons* retval = heap_cell;
	retval->typecell = constStringExprClosure;
	retval->val.stringVal = pStrVal;
	retval->inWHNF = true;
	retval->reduction_id = ++reduction_count;
    GC_STAT_UPDATE_LAST_USE(retval);
	return retval;
}

string* StrConstExprNode::getVal()
{
	return (this->pStrVal);
}

cons* StrConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = constStringExprClosure ;
	retval->val.stringVal = pStrVal;
    retval->inWHNF = true;
    retval->closure_id = ++closure_count;

    return retval;
}

BoolConstExprNode::~BoolConstExprNode()
{
	delete pBoolVal;
}

BoolConstExprNode::BoolConstExprNode(bool * bval) : ConstExprNode("BOOL"), pBoolVal(bval) {}

cons* BoolConstExprNode::evaluate()
{
    cons* heap_cell = NULL;
    assert(0 && "ToBeDone-- Amey");
	if (heap_cell->inWHNF)
	{
        GC_STAT_UPDATE_LAST_USE(heap_cell);
		return heap_cell;
	}

	cons* retval = heap_cell;
	retval->typecell = constBoolExprClosure;
	retval->val.boolval = *pBoolVal;
	retval->inWHNF = true;
	retval->reduction_id = ++reduction_count;
    GC_STAT_UPDATE_LAST_USE(retval);
	return retval;
}

cons* BoolConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = constBoolExprClosure;
	retval->val.boolval = *pBoolVal;
	retval->inWHNF = true;
	retval->closure_id = ++closure_count;
	return retval;
}

BoolConstExprNode * BoolConstExprNode::clone() const
{
	return new BoolConstExprNode(new bool(*pBoolVal));
}

bool BoolConstExprNode::getVal()
{
	return *(this->pBoolVal);
}

/*  Definitions for IF node */

IfExprNode::~IfExprNode() {
	delete pCond;
	delete pThen;
	delete pElse;
}

IfExprNode::IfExprNode(ExprNode * cond, ExprNode * then_expr, ExprNode * else_expr)
: ExprNode("IF"), pCond(cond), pThen(then_expr), pElse(else_expr)
{

}

void IfExprNode::doLabel(bool shouldAddLabel)
{
	pCond->doLabel(shouldAddLabel);
	pThen->doLabel(shouldAddLabel);
	pElse->doLabel(shouldAddLabel);
	label = (shouldAddLabel ? getNextLabel() : "");
}

IfExprNode * IfExprNode::clone() const
{
	return new IfExprNode(pCond->clone(), pThen->clone(), pElse->clone());

}

cons* IfExprNode::evaluate()
{

	curr_return_addr = getLabel();
//	DBG(cout << "At If updating curr_return_address to " << curr_return_addr << endl);

	if (gc_status == gc_live)
	{
		updateEvaluationPoint(this);
	}

//	for(vector<var_heap>::iterator vhit = actRecStack.begin()->heapRefs.begin(); vhit != actRecStack.begin()->heapRefs.end(); ++vhit)
//	{
//
//		cons* heap_cell = (cons*)vhit->ref;
////		DBG(cout << "Processing variable " << vhit->varname <<  " is_param " << vhit-> is_param << endl);
//		if (vhit->is_param)
//		{
////			DBG(cout << "Processing a parameter, not updating prog point" << endl);
//			continue;
//		}
//
//		if (heap_cell && !heap_cell->inWHNF)
//		{
//			switch(heap_cell->typecell)
//			{
//			case constIntExprClosure:
//			case constBoolExprClosure:
//			case constStringExprClosure:
//			case nilExprClosure:
//				break;
//			case unaryprimopExprClosure:
//			{
////				cout << "Unaryprimop" << endl;
//				auto arg_num = heap_cell->val.closure.arg1_name->substr(heap_cell->val.closure.arg1_name->rfind("/")+1);
////				cout << "Updating argument prg_pt " << *(heap_cell->val.closure.arg1_name)  << " to " << this->getLabel() + "/" + arg_num <<
////						" for " << vhit->varname << endl;
//				heap_cell->val.closure.arg1_name = new string(this->getLabel() + "/" + arg_num);
//				break;
//			}
//			case binaryprimopExprClosure:
//			{
//				assert(!heap_cell->inWHNF);
////				cout << "Binaryprimop" << endl;
//				auto arg_num = heap_cell->val.closure.arg1_name->substr(heap_cell->val.closure.arg1_name->rfind("/")+1);
//				heap_cell->val.closure.arg1_name = new string(this->getLabel() + "/" + arg_num);
////				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//				arg_num = heap_cell->val.closure.arg2_name->substr(heap_cell->val.closure.arg2_name->rfind("/")+1);
//				heap_cell->val.closure.arg2_name = new string(this->getLabel() + "/" + arg_num);
////				cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//				break;
//			}
//			case funcApplicationExprClosure:
//			{
////				cout << "FuncApp" << endl;
//				auto funcCallExpr = (FuncExprNode*)heap_cell->val.closure.expr;
//				auto num_args = funcCallExpr->getNumArgs();
//				if (num_args > 0)
//				{
//					auto curr_cell = heap_cell;
//					while (num_args > 0)
//					{
////						cout << "Processing arg" << (funcCallExpr->getArgs().size() - num_args + 1) << endl;
//						auto arg_num = curr_cell->val.closure.arg2_name->substr(curr_cell->val.closure.arg2_name->rfind("/")+1);
//						curr_cell->val.closure.arg2_name = new string(this->getLabel() + "/" + arg_num);
////						cout << "Updating argument prg_pt to " << this->getLabel() + "/" + arg_num << endl;
//						--num_args;
//						curr_cell = curr_cell->val.closure.arg1;
//					}
//				}
//				break;
//			}
//
//			break;
//			default : cout << "Should not have come to this point"<<endl;
//			cout << "Processing " << heap_cell << " with type " << heap_cell->typecell << endl;
//			break;
//			}
//		}
//	}

	IdExprNode* i = (IdExprNode*)this->pCond;
	cons* cond_heap_ref = (cons*)lookup_addr(i->getIDStr().c_str());


	//cout << "In if setting return address to " << curr_return_addr << endl;
	actRecStack.front().return_point = curr_return_addr;

	update_heap_refs.push(cond_heap_ref);

	cons* cond_resultValue = this->pCond->evaluate();
    GC_STAT_UPDATE_LAST_USE(cond_resultValue);

	cons* temp = update_heap_refs.top();
	temp->inWHNF = cond_resultValue->inWHNF;
	temp->typecell = cond_resultValue->typecell;
	temp->val = cond_resultValue->val;

	update_heap_refs.pop();

	assert(cond_resultValue->typecell == constBoolExprClosure);
	cons* retval;

    auto cond_val = cond_resultValue->val.boolval;

    if (gc_status != gc_disable && (gc_status == gc_live || gc_status == gc_freq))
    {

    	auto num_cells_required = cond_val ? func_heap_cell_reqd[pThen->getLabel()]:func_heap_cell_reqd[pElse->getLabel()];
//    	DBG(cout << "Num heap cells required for the rest of the branch is " << num_cells_required << endl);

    	if (current_heap() < num_cells_required)
    	{
    		liveness_gc();
    		detail_gc();
    		GC_STAT_DUMP_GARBAGE_STATS();

    		if (current_heap() < num_cells_required)
    		{

    			cout << "heap cells required " << num_cells_required << endl;
    			cout << "current heap size " << current_heap() << endl;

    			fprintf(stderr,"No Sufficient Memory - cons\n");
    			throw bad_alloc();
    		}

    	}
    }

//	if (cond_resultValue->val.boolval)
//	{
//		retval = this->pThen->evaluate();
//	}
//	else
//	{
//		retval = this->pElse->evaluate();
//	}

	auto resExpr = cond_val ? pThen : pElse;
	retval = resExpr->evaluate();

	assert(retval->inWHNF);
	return retval;

}

cons* IfExprNode::make_closure()
{
	cout << "Should never be executed as ANF does not allow if as right hand side for a let var"<<endl;
	return NULL;
}

ExprNode* IfExprNode::getCond()
{
	return this->pCond;
}

ExprNode* IfExprNode::getThenPart()
{
	return this->pThen;
}

ExprNode* IfExprNode::getElsePart()
{
	return this->pElse;
}

/*  Definitions for LET node */

LetExprNode::~LetExprNode() {
	delete pID;
	delete pExpr;
	delete pBody;
}

LetExprNode::LetExprNode(IdExprNode * id, ExprNode * expr, ExprNode * body)
: ExprNode("LET"), pID(id) , pExpr(expr), pBody(body) {}

void LetExprNode::doLabel(bool shouldAddLabel)
{

	pExpr->doLabel(shouldAddLabel);
	pBody->doLabel(shouldAddLabel);
	label = (shouldAddLabel ? getNextLabel() : "");
	//Set the nextExpr label for a function call expression.
	if (pExpr->isFunctionCallExpression())
		((FuncExprNode*)pExpr)->setNextExpr(pBody->getLabel());
}

LetExprNode * LetExprNode::clone() const
{
	return new LetExprNode(pID->clone(), pExpr->clone(), pBody->clone());
}

#ifndef __MYDEBUG
#define __MYDEBUG
#undef __MYDEBUG
#endif

cons* LetExprNode::evaluate()
{

	//Now we update the return address only at if/return, not at all let points
	//curr_return_addr = getLabel();


	//	DBG(cerr << "Processing let variable " << this->pID->getIDStr() << " at label" << getLabel() <<  endl);
//	cout << "Pointing to " << static_cast<cons*>(getfree()) << endl;
#ifdef __MYDEBUG 
//	cout << "Processing let variable " << this->pID->getIDStr() << " at label " << getLabel() <<  endl;
//	cout << "Allocation #" << num_of_allocations << endl;
//	cout << "Current heap = " << current_heap() << endl;

#endif
	bool isFunctionCall = getVarExpr()->isFunctionCallExpression();
     //-------------------------------------------------------------------------------------------------------------
     //This is where we do the GC. Can this be made independent of ExprNode class?
    static clock_tick last_gc_clock = 0;

	if (gc_status != gc_disable && (gc_status == gc_plain || gc_status == gc_freq))
    {

		if (GC_STAT_GET_CLOCK() - last_gc_clock > GC_FREQ_THRESHOLD()) {
			reachability_gc();
			//return_stack().return_point = curr_let_pgmpt;
			GC_STAT_DUMP_GARBAGE_STATS();
			last_gc_clock = GC_STAT_GET_CLOCK();
		}

        if ((!isFunctionCall && (current_heap() < 1))
                 || (isFunctionCall && (current_heap() < (0 + ((FuncExprNode*)(getVarExpr()))->pListArgs->size())))) 
        {

        	// cerr << "DOING RGC"<<endl;
        	//TODO : Add #define for the following code, they are not needed for RGC. Added only to dump graphviz files
        	//std::string curr_let_pgmpt = return_stack().return_point;
        	//return_stack().return_point = getLabel();
        	reachability_gc();
        	detail_gc();
        	//return_stack().return_point = curr_let_pgmpt;
        	GC_STAT_DUMP_GARBAGE_STATS();


            //We have to check for this condition here for lazy languages
            int num_cells_reqd = 0;
            if (!isFunctionCall )
                num_cells_reqd = 1;
            else // (isFunctionCall)
            {
                num_cells_reqd = ((FuncExprNode*)(getVarExpr()))->pListArgs->size();
                //We need to handle 0-ary functions. We need at least one cons cell even for a 0-ary function call.
                num_cells_reqd = (num_cells_reqd == 0) ? 1 : num_cells_reqd;
            }
            //		cerr << "Num of cons cells required is " << num_cells_reqd<<endl;
            if (check_space(num_cells_reqd * sizeof(cons)) == 0)
            {
                fprintf(stderr,"No Sufficient Memory - cons\n");
                throw bad_alloc();
            }
        }
    }
    /* End of GC related stuff */
    /* -------------------------------------------------------------------------------------------------------------*/


	//Create an entry for the variable where it will be allocated on the heap
	make_reference_addr(this->getVar().c_str(), getfree(), false);
	//ensure that the pointer does not get forwarded unnecessarily.
	cons* temp = static_cast<cons*>(getfree());
	temp->forward=NULL;

	//cout << "Created " << this->pID->getIDStr() << " at " << (temp - getbufferlive()) << endl;

	//If VarExpr is a function call, store the pgmpt of the let as the return point for liveness based GC 

	DBG(cout<<"In let expr " << getLabel() << endl);

	//We need the prgm pt of the enclosing let expr while creating liveness automata
	this->getVarExpr()->parent_let_pgmpt = getLabel();
	if (getVarExpr()->isFunctionCallExpression())
	{
		FuncExprNode* funExpr = (FuncExprNode*)getVarExpr();
		funExpr->parent_let_pgmpt = getLabel();

	}
	

	cons* var_res = this->getVarExpr()->make_closure();

	DBG(cout << "Created let expr at " << var_res << endl);

	cons* retval = this->getBody()->evaluate();
	assert(retval->inWHNF && is_valid_address(retval));

	return retval;

}

cons* LetExprNode::make_closure()
{
	cout << "This method should never be called. ANF cannot have a let as a right hand side for a let variable"<<endl;
	return NULL;
}

std::string LetExprNode::getVar()
{
	return this->pID->getIDStr();
}

ExprNode* LetExprNode::getVarExpr()
{
	return this->pExpr;
}

ExprNode* LetExprNode::getBody()
{
	return this->pBody;
}

/*  Definitions for abstract PRIMITIVE nodes */

PrimExprNode::~PrimExprNode() {}

PrimExprNode::PrimExprNode(const std::string name) : ExprNode(name) {}




UnaryPrimExprNode::~UnaryPrimExprNode() {
	delete pArg;
}

void UnaryPrimExprNode::doLabel(bool shouldAddLabel)
{
	pArg->doLabel(shouldAddLabel);
	label = (shouldAddLabel ? getNextLabel() : "");
}

UnaryPrimExprNode::UnaryPrimExprNode(const std::string name,
		ExprNode * arg) : PrimExprNode(name), pArg(arg)
{
	const char* primop = name.c_str();

	if (strcmp(primop, "car") == 0) this->type = carExpr;
	else if (strcmp(primop, "cdr") == 0) this->type = cdrExpr;
	else if (strcmp(primop, "null?") == 0) this->type = nullqExpr;
	else if (strcmp(primop, "pair?") == 0) this->type = pairqExpr;

}

UnaryPrimExprNode * UnaryPrimExprNode::clone() const
{
	return new UnaryPrimExprNode(node_name, pArg->clone());
}

cons* UnaryPrimExprNode::evaluate()
{
	cons* retval = NULL;
	switch(this->type)
	{
	case carExpr : retval = evaluateCarExpr();
	break;
	case cdrExpr : retval = evaluateCdrExpr();
	break;
	case nullqExpr : retval = evaluateNullqExpr();
	break;
	case pairqExpr : retval = evaluatePairqExpr();
	break;
	default : retval = NULL;
	}

	return retval;
}

cons* UnaryPrimExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = unaryprimopExprClosure;
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = pArg->make_closure();
	retval->val.closure.arg2 = NULL;
	retval->inWHNF = false;
	retval->val.closure.prog_pt = new string(getLabel());
	retval->val.closure.arg1_name = new string(curr_return_addr + "/1");
	retval->closure_id = ++closure_count;
	return retval;
}


cons* UnaryPrimExprNode::evaluateCarExpr()
{

	cons* heap_cell = update_heap_refs.top();
//	DBG(cerr << "Evaluating car expression for " << heap_cell << endl);
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		update_heap_refs.push(heap_cell->val.closure.arg1);
//		DBG(cerr << "Evaluating argument 1 for " << heap_cell->val.closure.arg1 << endl);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);

		assert(temp->inWHNF);
		cons* arg1 = update_heap_refs.top();

		assert(is_valid_address(arg1));

		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		arg1->val = temp->val;
		update_heap_refs.pop();


		assert(arg1->typecell == consExprClosure);
        GC_STAT_UPDATE_LAST_USE(arg1);

		assert(is_valid_address(arg1->val.cell.car));


		update_heap_refs.push(arg1->val.cell.car);
		cons* retval = reduceParamToWHNF(arg1->val.cell.car);
		update_heap_refs.pop();

		cons* heap_cell = update_heap_refs.top();
		assert(retval->inWHNF);



		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;

		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
    return heap_cell;
}

cons* UnaryPrimExprNode::evaluateCdrExpr()
{

	cons* heap_cell = update_heap_refs.top();

	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{

		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);

		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure);
        GC_STAT_UPDATE_LAST_USE(arg1);

		update_heap_refs.push(arg1->val.cell.cdr);
		cons* retval = reduceParamToWHNF(arg1->val.cell.cdr);

		update_heap_refs.pop();

		cons* heap_cell = update_heap_refs.top();
		assert(retval->inWHNF);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;

		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
	return heap_cell;
}

//TODO : Evaluation should modify the cons cell and update it. Need to have a handle for the cons cell
// that the expression is representing
cons* UnaryPrimExprNode::evaluateNullqExpr()
{
//	DBG(cerr << "Evaluating nullq expression"<<endl);
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{

		update_heap_refs.push(heap_cell->val.closure.arg1);
		assert(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);

		cons* arg1 = update_heap_refs.top();
		arg1->val = temp->val;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;

		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
        GC_STAT_UPDATE_LAST_USE(arg1);

		cons* heap_cell = update_heap_refs.top();
		heap_cell->inWHNF = true;
		heap_cell->val.boolval = (arg1->typecell == nilExprClosure)? true:false;
		heap_cell->typecell = constBoolExprClosure;

		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
	return heap_cell;
}

cons* UnaryPrimExprNode::evaluatePairqExpr()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{

		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);

		cons* arg1 = update_heap_refs.top();
		arg1->val = temp->val;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;

		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
        GC_STAT_UPDATE_LAST_USE(arg1);
		cons* heap_cell = update_heap_refs.top();
		heap_cell->inWHNF = true;

		heap_cell->val.boolval = (arg1->typecell == consExprClosure);
		heap_cell->typecell = constBoolExprClosure;

		heap_cell->reduction_id = ++reduction_count;

		return heap_cell;
	}
	return heap_cell;
}

BinaryPrimExprNode::~BinaryPrimExprNode()
{
	delete pArg1;
	delete pArg2;
}

void BinaryPrimExprNode::doLabel(bool shouldAddLabel)
{
	pArg1->doLabel(shouldAddLabel);
	pArg2->doLabel(shouldAddLabel);
	label = (shouldAddLabel ? getNextLabel() : "");
}

BinaryPrimExprNode::BinaryPrimExprNode(const std::string name, ExprNode * arg1, ExprNode * arg2)
: PrimExprNode(name), pArg1(arg1), pArg2(arg2)
{
	const char* primop = name.c_str();

	if (strcmp(primop, "+") == 0) this->type = addExpr;
	else if (strcmp(primop, "-") == 0) this->type = subExpr;
	else if (strcmp(primop, "*") == 0) this->type = mulExpr;
	else if (strcmp(primop, "/") == 0) this->type = divExpr;
	else if (strcmp(primop, "%") == 0) this->type = modExpr;
	else if (strcmp(primop, "<") == 0) this->type = ltExpr;
	else if (strcmp(primop, ">") == 0) this->type = gtExpr;
	else if ((strcmp(primop, "=") == 0) || (strcmp(primop, "eq?") == 0)) this->type = eqExpr;
	//else if (strcmp(primop, "cons") == 0) this->type = consExpr;
	else if (strcmp(primop, "cons") == 0) this->type = consExpr;
}

BinaryPrimExprNode * BinaryPrimExprNode::clone() const
{
	return new BinaryPrimExprNode(node_name, pArg1->clone(), pArg2->clone());
}

cons* BinaryPrimExprNode::evaluate()
{
	cons* retval = NULL;
	switch(this->type)
	{
	case consExpr : retval = evaluateCons();
	break;
	case addExpr : retval = evaluateAdd();
	break;
	case subExpr : retval = evaluateSub();
	break;
	case mulExpr : retval = evaluateMul();
	break;
	case divExpr : retval = evaluateDiv();
	break;
	case modExpr : retval = evaluateMod();
	break;
	case ltExpr : retval =  evaluateLT();
	break;
	case gtExpr : retval =  evaluateGT();
	break;
	case eqExpr : 	retval =  evaluateEQ();
	break;
	default : retval = NULL;
	}

	return retval;
}

cons* BinaryPrimExprNode::make_closure()
{
	cons* retval = (cons*) allocate_cons();



	if (type != consExpr)
	{
		retval->val.closure.arg1 = pArg1->make_closure();
		retval->val.closure.arg2 = pArg2->make_closure();
		retval->val.closure.expr = this;
		retval->typecell = binaryprimopExprClosure;
		retval->inWHNF = false;
		retval->val.closure.arg1_name = new string(curr_return_addr + "/1");
		retval->val.closure.arg2_name = new string(curr_return_addr + "/2");
		retval->val.closure.prog_pt = new string(getLabel());
		retval->closure_id = ++closure_count;

		DBG(cout << "Arg1 = " << retval->val.closure.arg1<<endl);
		DBG(cout << "Arg2 = " << retval->val.closure.arg2<<endl);
	}
	else
	{
		retval->val.cell.car = pArg1->make_closure();
		retval->val.cell.cdr = pArg2->make_closure();
		retval->typecell = consExprClosure;
		retval->inWHNF = true;
		retval->closure_id = ++closure_count;
		DBG(cout << "CAR = " << retval->val.cell.car<<endl);
		DBG(cout << "CDR = " << retval->val.cell.cdr<<endl);
	}



	return retval;
}

cons* BinaryPrimExprNode::evaluateCons()
{
	cons* heap_cell = update_heap_refs.top();

	assert(is_valid_address(heap_cell));
	if (heap_cell->inWHNF)
		return heap_cell;

	//assert(is_valid_address(heap_cell->val.closure.arg1) && is_valid_address(heap_cell->val.closure.arg2));
	heap_cell->inWHNF = true;
	heap_cell->typecell = consExprClosure;
	heap_cell->val.cell.car = heap_cell->val.closure.arg1;
	heap_cell->val.cell.cdr = heap_cell->val.closure.arg2;

	heap_cell->reduction_id = ++reduction_count;

	return heap_cell;
}



cons* BinaryPrimExprNode::evaluateAdd( )
{

	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;


	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);

	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.
	cons* arg1 = update_heap_refs.top();
	arg1->inWHNF = temp->inWHNF;
	arg1->val = temp->val;
	arg1->typecell = temp->typecell;
	//POP IT TO ACCESS THE HEAP CELL

	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* temp2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

	cons* arg2 = update_heap_refs.top();
	arg2->val = temp2->val;
	arg2->inWHNF = temp2->inWHNF;
	arg2->typecell = temp2->typecell;
	update_heap_refs.pop();

	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();


	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal + arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

	heap_cell->reduction_id = ++reduction_count;

	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateSub()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

//	cerr << "Evaluating sub operation for node " << heap_cell << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);

	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal - arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

//	cerr << "Updated sub operation for node " << heap_cell << endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMul()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal * arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateDiv()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);

	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal / arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMod()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;


	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);

	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal % arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateLT()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;


	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);

	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal < arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateGT()
{
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;


	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);

	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal > arg2->val.intVal;
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateEQ()
{

	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;



	update_heap_refs.push(heap_cell->val.closure.arg1);
	DBG(cout << "Arg1 = " << *(heap_cell->val.closure.arg1_name) << endl);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	DBG(cout << "Processing closure " << heap_cell << endl);
	assert(arg1 == update_heap_refs.top());
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();


	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);

	update_heap_refs.push(heap_cell->val.closure.arg2);
	DBG(cout << "Processing closure " << heap_cell << endl);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	DBG(cout << "Arg2 = " << *(heap_cell->val.closure.arg2_name) << endl);
	assert(arg2 == update_heap_refs.top());
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();



	assert(is_valid_address(arg1) && is_valid_address(arg2));
    GC_STAT_UPDATE_LAST_USE(arg1);
    GC_STAT_UPDATE_LAST_USE(arg2);

	bool isequal = false;
	if (arg1->typecell == arg2->typecell)
	{
		switch(arg1->typecell)
		{
		case constStringExprClosure: isequal = (arg1->val.stringVal == arg2->val.stringVal) ||
				                                (0 == strcmp(arg1->val.stringVal->c_str(), arg2->val.stringVal->c_str()));

		break;
		case constBoolExprClosure: isequal = (arg1->val.boolval == arg2->val.boolval);
		break;
		case constIntExprClosure: isequal = (arg1->val.intVal==arg2->val.intVal);
		break;
		case consExpr: isequal = (arg1 == arg2);
		break;
		default : isequal = (arg1 == arg2);
		break;
		}
	}
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->val.boolval = isequal;
	heap_cell->inWHNF = true;

	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

/*  Definitions for FUNCTION CALL node */

FuncExprNode::~FuncExprNode()
{
	delete pID;

	for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		delete (*i);
	delete pListArgs;
}

void FuncExprNode::doLabel(bool shouldAddLabel)
{

	for(std::list<ExprNode *>::iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		(*i)->doLabel(shouldAddLabel);
	label = (shouldAddLabel ? getNextLabel() : "");
}

FuncExprNode::FuncExprNode(IdExprNode * id, std::list<ExprNode *> * args)
: ExprNode("FUNC"), pID(id), pListArgs(args), fBody(NULL)
{
	for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		vargs.push_back(*i);

}

FuncExprNode * FuncExprNode::clone() const
{
	std::list<ExprNode *> * newArgs = new std::list<ExprNode *>();
	for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		newArgs->push_back((*i)->clone());
	return new FuncExprNode(pID->clone(), newArgs);
}

std::string FuncExprNode::getFunction()
{
	return this->pID->getName();
}



cons* FuncExprNode::evaluate()
{
	if (update_heap_refs.top()->inWHNF)
		return update_heap_refs.top();

	if (gc_status != gc_disable && (gc_status == gc_live || gc_status == gc_freq))
	{
		static clock_tick last_gc_clock = 0;
		auto num_cells_reqd = func_heap_cell_reqd[getFunction()];

		if (gc_status == gc_freq)
		{
			if (GC_STAT_GET_CLOCK() - last_gc_clock > GC_FREQ_THRESHOLD())
			{

				reachability_gc();

				GC_STAT_DUMP_GARBAGE_STATS();
				last_gc_clock = GC_STAT_GET_CLOCK();
			}
		}

		if (current_heap() < num_cells_reqd)
		{

			assert(gc_status == gc_live);
			//DBG(cerr << "Updating curr_let_pgmpt to " );
			//DBG(cerr << return_stack().return_point << endl);
			//std::string curr_let_pgmpt = return_stack().return_point;
			//				return_stack().return_point = getLabel();

			liveness_gc();
			detail_gc();
			GC_STAT_DUMP_GARBAGE_STATS();
			//				return_stack().return_point = curr_let_pgmpt;


			if (current_heap() < num_cells_reqd)
			{

				cout << "heap cells required " << num_cells_reqd << endl;
				cout << "current heap size " << current_heap() << endl;

				fprintf(stderr,"No Sufficient Memory - cons\n");
				throw bad_alloc();
			}
		}

	}

	cons* heap_cell = update_heap_refs.top();
	DefineNode* funcDef = (DefineNode*)pgm->getFunction(this->getFunction());

//	cout << "Looking up function " << this->getFunction() << endl;
	assert(funcDef);
//	cout << "Creating activation record for func " << funcDef->getFuncName() << " with ret address " << curr_return_addr << endl;
	make_environment(funcDef->getFuncName().c_str(), curr_return_addr);


	auto num_args = pListArgs->size();
	auto curr = heap_cell;
	while(num_args > 0)
	{
		//cerr << "Creating ref for argument " << (funcDef->getArgs()[num_args-1]).c_str() << " with type " << curr->val.closure.arg2->typecell << endl;
		make_reference_addr((funcDef->getArgs()[num_args-1]).c_str(), curr->val.closure.arg2, true);
		--num_args;
		if (curr->val.closure.arg1 != NULL)
		{
			curr = curr->val.closure.arg1;
		}
	}


	cons *temp = funcDef->getFunctionBody()->evaluate();
	assert(temp->inWHNF);

	heap_cell = update_heap_refs.top();

	heap_cell->val = temp->val;
	heap_cell->typecell = temp->typecell;
	heap_cell->inWHNF = true;

	delete_environment();
//	cout << "Completed processing function " << funcDef->getFuncName() << endl;
	assert(heap_cell->typecell == temp->typecell);
	heap_cell->reduction_id = ++reduction_count;

	return heap_cell;
}


cons* FuncExprNode::make_closure()
{
	int num_args = pListArgs->size() - 1;
	int size = pListArgs->size();

	cons* retval = (cons*)allocate_cons();
	auto rarglistiter = this->pListArgs->rbegin();


	retval->typecell=funcApplicationExprClosure;
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = NULL;
	retval->val.closure.arg2 = NULL;
	retval->inWHNF = false;
	if (pListArgs->size() > 0)
	{

		retval->val.closure.arg2 = lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
		//retval->val.closure.arg2_name = new string(((IdExprNode*)(*rarglistiter))->getIDStr());
		retval->val.closure.arg2_name = new string(curr_return_addr + "/" + to_string(size - num_args));
		retval->val.closure.prog_pt = new string(getLabel());
		auto prev = retval;

		while(num_args > 0)
		{
			++rarglistiter;
			auto curr = allocate_cons();
			curr->inWHNF = false;
			curr->typecell = funcArgClosure;
			curr->val.closure.arg2 =  lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
			//curr->val.closure.arg2_name = new string(((IdExprNode*)(*rarglistiter))->getIDStr());
			curr->val.closure.arg2_name = new string(curr_return_addr + "/" + to_string(size - num_args + 1));
			curr->val.closure.prog_pt = new string(getLabel());

			curr->val.closure.expr = this;
			curr->val.closure.arg1 = NULL;
			prev->val.closure.arg1 = curr;
			prev = curr;
			--num_args;
		}
	}

	retval->closure_id = ++closure_count;
	return retval;
}

std::vector<ExprNode*> FuncExprNode::getArgs()
{
	return this->vargs;
}

void FuncExprNode::setNextExpr(string l)
{
	this->nextExpr = l;
}

string FuncExprNode::getNextExpr()
{
	return this->nextExpr;
}

/*  Definitions for DEFINE node */

DefineNode::~DefineNode()
{
	delete pID;
	delete pExpr;

	for(std::list<IdExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		delete (*i);
	delete pListArgs;
}

DefineNode::DefineNode(IdExprNode * id, std::list<IdExprNode *> * args, ExprNode * expr)
:  Node("DEFINE"), pExpr(expr), pID(id), pListArgs(args)
{
	for(std::list<IdExprNode*>::iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
	{
		vargs.push_back((*i)->getIDStr());
	}
}

void DefineNode::doLabel(bool shouldAddLabel)
{
	//label = (shouldAddLabel ? getNextLabel() : "");
	label = "e" + this->getFuncName();
	pExpr->doLabel(shouldAddLabel);
}

std::string DefineNode::getFuncName() const
{
	return pID->getIDStr();
}

DefineNode * DefineNode::clone() const {
	std::list<IdExprNode *> * newArgs = new std::list<IdExprNode *>();
	for(std::list<IdExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		newArgs->push_back((*i)->clone());
	return new DefineNode(pID->clone(), newArgs, pExpr->clone());
}

vector<std::string> DefineNode :: getArgs()
{
	return this->vargs;
}

/*  Definitions for PROGRAM node */

ProgramNode::~ProgramNode()
{
	delete pExpr;

	for(std::list<DefineNode *>::const_iterator i = pListDefines->begin(); i != pListDefines->end();
			++i)
		delete (*i);
	delete pListDefines;
	pgm = NULL;
}

void ProgramNode::doLabel(bool shouldAddLabel)
{

	if(shouldAddLabel == (label != "")) return;
	//label = (shouldAddLabel ? "LABELLED" : "");
	lbl_count = 0;
	
	for(std::list<DefineNode *>::iterator i = pListDefines->begin(); i != pListDefines->end(); ++i)
		(*i)->doLabel(shouldAddLabel);
	pExpr->doLabel(true); //Always add label
}

ProgramNode::ProgramNode(std::list<DefineNode *> * defines, ExprNode * expr)
: Node("PROGRAM"), pListDefines(defines), pExpr(expr), progpt_map(new std::unordered_map<std::string, const Node*>())
{
	for (std::list<DefineNode*>::iterator fundef = defines->begin(); fundef != defines->end(); ++fundef)
	{
		funcmap[(*fundef)->getFunctionName()] = (*fundef);
	}
	pgm = this;
}

ProgramNode * ProgramNode::clone() const {
	std::list<DefineNode *> * newDefines = new std::list<DefineNode *>();
	for(std::list<DefineNode *>::const_iterator i = pListDefines->begin(); i != pListDefines->end();
			++i)
		newDefines->push_back((*i)->clone());
	return new ProgramNode(newDefines, pExpr->clone());
}

cons* ProgramNode::evaluate()
{
//	std::cout << "Evaluating main expression " << std::endl;
	return this->pExpr->evaluate();
}

Node* ProgramNode::getFunction(std::string funcName)
{
	return this->funcmap[funcName];
}





