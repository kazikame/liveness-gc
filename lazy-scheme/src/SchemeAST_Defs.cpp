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
#include "Simulator.h"
#include "gc.h"
using namespace std;
using namespace Scheme::AST;


extern deque<actRec> actRecStack;
extern GCStatus gc_status;
vector<double> gc_times;
int conscount;
extern int gcinvoke;
int maxcount;
extern double gctime;
ProgramNode* pgm;
unsigned long lbl_count = 0;
map< string, vector<ExprNode*>> func_call_points;

std::string getNextLabel()
{
	std::stringstream sstream;
	//sstream <<  "p#" << lbl_count++;
	sstream <<  lbl_count++;
	return sstream.str();
}

cons* reduceParamToWHNF(cons* heap_cell)
{
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		cons* retval = heap_cell;
		while(!retval->inWHNF)
			retval = retval->val.closure.expr->evaluate(heap_cell);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = retval->inWHNF;
		return heap_cell;
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
	//cout << "returning clone for identifier " << *pID << " pointing to closure " << heap_ptr << endl;
	return new IdExprNode(new std::string(*pID));
}

void IdExprNode::doLabel(bool shouldAddLabel)
{
	string l = getNextLabel();
	label = (shouldAddLabel ? l : "");
}

cons* IdExprNode::evaluate(cons* heap_cell = NULL)
{
	std::string varName = *(this->pID);
	//cout << "Evaluating " << varName << endl;
	if (heap_cell->inWHNF)
	{
		return heap_cell;
	}
	else
	{
		while (!heap_cell->inWHNF)
			heap_cell = heap_cell->val.closure.expr->evaluate(heap_cell);
		heap_cell->inWHNF = true;

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
}

ReturnExprNode * ReturnExprNode::clone() const
{
	return new ReturnExprNode(pID->clone());
}

cons* ReturnExprNode::evaluate(cons* heap_cell = NULL)
{

	std::string varName = this->pID->getName();
	cons* retval = (cons*)lookup_addr(varName.c_str());
	if (retval->inWHNF)
	{
		return retval;
	}
	else
	{
		while (!retval->inWHNF)
			retval = retval->val.closure.expr->evaluate(retval);
		retval->inWHNF = true;

		return retval;
	}
}
cons* ReturnExprNode::make_closure()
{
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

	return retval;
}

cons* NilConstExprNode::evaluate(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		while (!heap_cell->inWHNF)
			heap_cell = heap_cell->val.closure.expr->evaluate(heap_cell);
		heap_cell->inWHNF = true;
		heap_cell->typecell = nilExprClosure;

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

cons* IntConstExprNode::evaluate(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
	{
		//cout << "Returning " << heap_cell << " with value " << heap_cell->val.intVal << endl;
		return heap_cell;
	}

	cons* retval = heap_cell;
	retval->typecell = constIntExprClosure;
	retval->val.intVal = *pIntVal;
	retval->inWHNF = true;
	//cout << "Stored integer value " << retval->val.intVal << " at " << retval << endl;
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
	//cout << "Stored integer value " << retval->val.intVal << " at " << retval << endl;
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

cons* StrConstExprNode::evaluate(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
	{
		//cout << "Returning " << heap_ptr << " with value " << *(heap_ptr->val.stringVal) << endl;
		return heap_cell;
	}

	cons* retval = heap_cell;
	retval->typecell = constStringExprClosure;
	retval->val.stringVal = pStrVal;
	retval->inWHNF = true;
	//cout << "Stored boolean value " << retval->val.boolval << " at " << retval << endl;
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

    return retval;
}

BoolConstExprNode::~BoolConstExprNode()
{
	delete pBoolVal;
}

BoolConstExprNode::BoolConstExprNode(bool * bval) : ConstExprNode("BOOL"), pBoolVal(bval) {}

cons* BoolConstExprNode::evaluate(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
	{
		//cout << "Returning " << heap_ptr << " with value " << heap_ptr->val.boolval << endl;
		return heap_cell;
	}

	cons* retval = heap_cell;
	retval->typecell = constBoolExprClosure;
	retval->val.boolval = *pBoolVal;
	retval->inWHNF = true;
	//cout << "Stored boolean value " << retval->val.boolval << " at " << retval << endl;
	return retval;
}

cons* BoolConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = constBoolExprClosure;
	retval->val.boolval = *pBoolVal;
	retval->inWHNF = true;
	//cout << "Stored boolean value " << retval->val.boolval << " at " << retval << endl;
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

	//cout << "then expression is " << then_expr << endl;
	//this->heap_ptr = NULL;
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

cons* IfExprNode::evaluate(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
	{
		//cout << "returning already evaluated result"<<endl;
		return heap_cell;
	}
	else
	{
		//cout << "Processing if-then-else"<<endl;
		//The condition is always a variable. Get the heap location associated with it
		//by looking up in the stack.
		IdExprNode* i = (IdExprNode*)this->pCond;
		cons* cond_heap_ref = (cons*)lookup_addr(i->getIDStr().c_str());


		cons* cond_resultValue = this->pCond->evaluate(cond_heap_ref);

		assert(cond_resultValue->typecell == constBoolExprClosure);
		cons* retval;

		if (cond_resultValue->val.boolval)
		{
			retval = this->pThen->evaluate(heap_cell);
		}
		else
		{
			retval = this->pElse->evaluate(heap_cell);
		}
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
		return heap_cell;
	}
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

cons* LetExprNode::evaluate(cons* heap_cell = NULL)
{
	//cout << "Processing let variable " << this->pID->getIDStr() << endl;

	if (gc_status != gc_disable && current_heap() == 0)
		reachability_gc();

	//Create an entry for the variable where it will be allocated on the heap
	make_reference_addr(this->getVar().c_str(), getfree());

	cons* var_res = this->getVarExpr()->make_closure();
	//cout << "Created closure for variable " << this->pID->getIDStr() <<" at " << var_res << " with type " << var_res->typecell <<endl;

	//cout << "Evaluating let expression " << this->pBody << endl;
	var_res = this->getBody()->evaluate(var_res);

	//cout << "Evaluated let expression for variable "<< this->pID->getIDStr() <<" to " << var_res << " with type " << var_res->typecell << endl;
	return var_res;

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

cons* UnaryPrimExprNode::evaluate(cons* heap_cell = NULL)
{
	switch(this->type)
	{
	case carExpr : return evaluateCarExpr(heap_cell);
	break;
	case cdrExpr : return evaluateCdrExpr(heap_cell);
	break;
	case nullqExpr : return evaluateNullqExpr(heap_cell);
	break;
	case pairqExpr : return evaluatePairqExpr(heap_cell);
	break;
	default : return NULL;
	}
}

cons* UnaryPrimExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = unaryprimopExprClosure; //TODO: need to change this to ensure that the correct type is set
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = pArg->make_closure();
	retval->inWHNF = false;
	//cout << "created closure at " << retval << " with closure at " << retval->val.closure.arg1 << endl;

	return retval;
}


cons* UnaryPrimExprNode::evaluateCarExpr(cons* heap_cell = NULL)
{
	//cout << "Evaluating car for " << heap_cell << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
		cons* retval = arg1->val.cell.car;
		while(!retval->inWHNF)
			retval = retval->val.closure.expr->evaluate(retval);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
		return heap_cell;
	}
return heap_cell;
}

cons* UnaryPrimExprNode::evaluateCdrExpr(cons* heap_cell = NULL)
{

	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
		cons* retval = arg1->val.cell.cdr;
		while(!retval->inWHNF)
			retval = retval->val.closure.expr->evaluate(retval);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
		return heap_cell;
	}
	return heap_cell;
}

//TODO : Evaluation should modify the cons cell and update it. Need to have a handle for the cons cell
// that the expression is representing
cons* UnaryPrimExprNode::evaluateNullqExpr(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << heap_cell->val.closure.arg1 << endl;
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
		//cout << "Type of heap_cell is " << arg1->typecell << endl;
		heap_cell->inWHNF = true;
		heap_cell->val.boolval = (arg1->typecell == nilExprClosure)? true:false;
		heap_cell->typecell = constBoolExprClosure;
		return heap_cell;
	}
	return heap_cell;
}

cons* UnaryPrimExprNode::evaluatePairqExpr(cons* heap_cell = NULL)
{

	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
		heap_cell->inWHNF = true;
		//cout << "Type of heap_cell is " << arg1->typecell << endl;
		heap_cell->val.boolval = (arg1->typecell == consExprClosure);
		heap_cell->typecell = constBoolExprClosure;
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
	else if (strcmp(primop, "cons") == 0) this->type = consExpr;
}

BinaryPrimExprNode * BinaryPrimExprNode::clone() const
{
	return new BinaryPrimExprNode(node_name, pArg1->clone(), pArg2->clone());
}

cons* BinaryPrimExprNode::evaluate(cons* heap_cell = NULL)
{
	//cout << "Evaluating the operator " << this->type << endl;
	switch(this->type)
	{
	case consExpr : return evaluateCons(heap_cell);
	break;
	case addExpr : return evaluateAdd(heap_cell);
	break;
	case subExpr : return evaluateSub(heap_cell);
	break;
	case mulExpr : return evaluateMul(heap_cell);
	break;
	case divExpr : return evaluateDiv(heap_cell);
	break;
	case modExpr : return evaluateMod(heap_cell);
	break;
	case ltExpr : return evaluateLT(heap_cell);
	break;
	case gtExpr : return evaluateGT(heap_cell);
	break;
	case eqExpr : 	return evaluateEQ(heap_cell);
	break;
	default : return NULL;
	}
	return NULL;
}

cons* BinaryPrimExprNode::make_closure()
{
	cons* retval = (cons*) allocate_cons();
//	arg1Closure = pArg1->make_closure();
//	arg2Closure = pArg2->make_closure();
	retval->val.closure.arg1 = pArg1->make_closure();
	retval->val.closure.arg2 = pArg2->make_closure();
	retval->val.closure.expr = this;
	retval->typecell = binaryprimopExprClosure;
	retval->inWHNF = false;
//	retval->val.expr=this;
//	this->heap_ptr = retval;
//	this->heap_ptr->inWHNF = false;
	return retval;
}

cons* BinaryPrimExprNode::evaluateCons(cons* heap_cell = NULL)
{

	//cout << "Processing cons cell " << heap_cell << endl;
	if (heap_cell->inWHNF)
		return heap_cell;

	heap_cell->inWHNF = true;
	heap_cell->typecell = consExprClosure;
	heap_cell->val.cell.car = heap_cell->val.closure.arg1;
	heap_cell->val.cell.cdr = heap_cell->val.closure.arg2;
	return heap_cell;
}



cons* BinaryPrimExprNode::evaluateAdd(cons* heap_cell = NULL)
{

	if (heap_cell->inWHNF)
		return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal + arg2->val.intVal;

	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateSub(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
			return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal - arg2->val.intVal;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMul(cons* heap_cell = NULL)
{

	if (heap_cell->inWHNF)
			return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal * arg2->val.intVal;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateDiv(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal / arg2->val.intVal;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMod(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal % arg2->val.intVal;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateLT(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal < arg2->val.intVal;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateGT(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal > arg2->val.intVal;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateEQ(cons* heap_cell = NULL)
{
	if (heap_cell->inWHNF)
		return heap_cell;

	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);

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
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->val.boolval = isequal;
	heap_cell->inWHNF = true;
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
: ExprNode("FUNC"), pID(id), pListArgs(args)
{
	for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		vargs.push_back(*i);
	//func_call_points[id].push_back(this);
}

FuncExprNode * FuncExprNode::clone() const {
	std::list<ExprNode *> * newArgs = new std::list<ExprNode *>();
	for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i)
		newArgs->push_back((*i)->clone());
	return new FuncExprNode(pID->clone(), newArgs);
}

std::string FuncExprNode::getFunction()
{
	return this->pID->getName();
}



cons* FuncExprNode::evaluate(cons* heap_cell = NULL)
{
	//cout << "Evaluating function " << this->getFunction() << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	DefineNode* funcDef = (DefineNode*)pgm->getFunction(this->getFunction());
	make_environment(funcDef->getFuncName().c_str(), "");
	auto num_args = pListArgs->size();
	auto curr = heap_cell;
	while(num_args > 0)
	{
		make_reference_addr((funcDef->getArgs()[num_args-1]).c_str(), curr->val.closure.arg2);
		--num_args;
		if (num_args > 0)
		{
			curr = curr->val.closure.arg1;
		}
	}
	heap_cell = funcDef->getFunctionBody()->evaluate(heap_cell);
	assert(heap_cell->inWHNF);
	heap_cell->inWHNF = true;
	delete_environment();
	return heap_cell;
//	if (!this->heap_ptr->inWHNF)
//	{
//		cout << "Calling function " << this->getFunction() << endl;
//		DefineNode* funcDef = (DefineNode*)pgm->getFunction(this->getFunction());
//		make_environment(funcDef->getFuncName().c_str(), this->getNextExpr());
//		int i = 0;
//		for(std::list<cons*>::iterator arg = argsClosureList.begin(); arg != argsClosureList.end(); ++arg, ++i)
//		{
//			//cout << "creating reference for parameter " << (funcDef->getArgs()[i].c_str()) << " pointing to " << *arg << endl;
//			make_reference_addr((funcDef->getArgs()[i]).c_str(), (void*)(*arg)); //TODO: Change this to accept cons*
//		}
//		//cout << "Evaluating function body expression " << funcDef->getFunctionBody() << endl;
//		//ExprNode* funcBodyExpr = funcDef->getFunctionBody()->clone();
//		//funcBodyExpr->heap_ptr = allocate_cons();
//		//funcBodyExpr->heap_ptr->inWHNF = false;
//		cons* res = fBody->evaluate();
//		while(!res->inWHNF)
//			res = res->val.expr->evaluate();
//		this->heap_ptr->typecell = res->typecell;
//		this->heap_ptr->inWHNF = res->inWHNF;
//		//cout << "Assigning to " << heap_ptr << " value of " << res << endl;
//		this->heap_ptr->val = res->val;
//		//this->heap_ptr = res;
//		cout << "Function body for " << getFunction() << " evaluated to " << res << " with type " << res->typecell << endl;
//		//cout << "removing activation record from stack for function " << this->getFunction() << endl;
//		delete_environment();
//		return res;
//	}
//	else
//	{
//		cout << "Function " << getFunction() << " already evaluated to WHNF" << endl;
//		return this->heap_ptr;
//	}
	return heap_cell;
}


cons* FuncExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	auto rarglistiter = this->pListArgs->rbegin();
	int num_args = pListArgs->size() - 1;
	//cout << "Num of arguments " << pListArgs->size() << " for fn call " << this->getFunction()<<endl;
	retval->typecell=funcApplicationExprClosure;
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = NULL;
	retval->val.closure.arg2 = NULL;
	if (pListArgs->size() > 0)
	{
		retval->val.closure.arg2 = lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
		retval->inWHNF = false;
		auto prev = retval;
		while(num_args > 0)
		{
			++rarglistiter;
			auto curr = allocate_cons();
			curr->inWHNF = false;
			curr->val.closure.arg2 =  lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
			curr->val.closure.expr = this;
			curr->val.closure.arg1 = NULL;
			prev->val.closure.arg1 = curr;
			prev = curr;
			--num_args;
		}
	}
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
	label = (shouldAddLabel ? "LABELLED" : "");
	lbl_count = 0;
//	for(std::list<DefineNode *>::iterator i = pListDefines->begin(); i != pListDefines->end(); ++i)
//		(*i)->doLabel(shouldAddLabel);
//	cout << pExpr << endl;
//	pExpr->doLabel(true); //Always add label
}

ProgramNode::ProgramNode(std::list<DefineNode *> * defines, ExprNode * expr)
: Node("PROGRAM"), pListDefines(defines), pExpr(expr)
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

cons* ProgramNode::evaluate(cons* heap_cell = NULL)
{
	//std::cout << "Evaluating main expression " << std::endl;
	return this->pExpr->evaluate();
}

Node* ProgramNode::getFunction(std::string funcName)
{
	return this->funcmap[funcName];
}





