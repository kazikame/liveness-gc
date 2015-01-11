<<<<<<< HEAD
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
extern int gcinvoke;
int maxcount;
extern double gctime;
ProgramNode* pgm;
unsigned long lbl_count = 0;
map< string, vector<ExprNode*>> func_call_points;
unsigned int closure_count = 0;
unsigned int reduction_count = 0;
extern unsigned int num_of_allocations;
extern map<cons*, int> heap_map;
extern map<int, string> root_var_map;

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

	assert(is_valid_address(heap_cell));
	if (heap_cell->inWHNF)
	{
		return heap_cell;
	}
	else
	{
		cons* retval = heap_cell;
		cons *temp = (retval->val.closure.expr->evaluate());
		retval=temp;

		assert(retval->inWHNF);
		assert(is_valid_address(retval));

		heap_cell = update_heap_refs.top();
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
	cons* retval = lookup_addr(this->pID->c_str());
	if (retval->inWHNF)
	{
		return retval;
	}
	else
	{

		update_heap_refs.push(retval);
		cons* temp = retval->val.closure.expr->evaluate();

		heap_cell = update_heap_refs.top();
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
}

ReturnExprNode * ReturnExprNode::clone() const
{
	return new ReturnExprNode(pID->clone());
}

cons* ReturnExprNode::evaluate(cons* heap_cell = NULL)
{

	//cout << " Starting processing return for  " << this->pID->getName()<< endl;
	std::string varName = this->pID->getName();
	cons* retval = (cons*)lookup_addr(varName.c_str());
	if (!retval->inWHNF)
	{

		//cout << "Pushing onto stack return expr " << retval << endl;
		update_heap_refs.push(retval);
		cons* temp = retval->val.closure.expr->evaluate();
		retval = update_heap_refs.top();
		assert(temp->inWHNF && retval->inWHNF);
		//cout << "Value of temp " << temp->inWHNF << " and retval " << retval << " and type "<< temp->typecell<<endl;
//		cout << "Updating value of variable " << varName << " at location " << heap_map[retval]
//		     << " with type " << print_cell_type(retval->typecell)<< endl;
		retval->inWHNF = true;
		retval->typecell = temp->typecell;
		retval->val = temp->val;\
//		cout << "Updated value of variable " << varName << " at location " << heap_map[retval]
//				     << " with type " << print_cell_type(retval->typecell)<< endl;
		//cout << "Popping return expr " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();
		//cout << "returning " << retval << " from returnexpr" << endl;
		//ofstream out("func_call.txt", ios::app);
		//cout << "Finished processing return " << this->type <<endl;
		//create_heap_bft(cout);
		//out.close();
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

cons* NilConstExprNode::evaluate(cons* heap_cell = NULL)
{
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

cons* IntConstExprNode::evaluate(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
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
	retval->reduction_id = ++reduction_count;
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
	retval->reduction_id = ++reduction_count;
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
	retval->reduction_id = ++reduction_count;
	return retval;
}

cons* BoolConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = constBoolExprClosure;
	retval->val.boolval = *pBoolVal;
	retval->inWHNF = true;
	//cout << "Stored boolean value " << retval->val.boolval << " at " << retval << endl;
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
	heap_cell = update_heap_refs.top();
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

		update_heap_refs.push(cond_heap_ref);
		//cout << "Pushing on to stack in if expr " << cond_heap_ref << endl;
		cons* cond_resultValue = this->pCond->evaluate();
		//cout << "Popping in if expr " << update_heap_refs.top()<<endl;
		cons* temp = update_heap_refs.top();
		temp->inWHNF = cond_resultValue->inWHNF;
		temp->typecell = cond_resultValue->typecell;
		temp->val = cond_resultValue->val;
//		cout << "If condition Updating heap cell " << heap_map[temp] << "(" << temp << ")" <<endl;
		update_heap_refs.pop();

		assert(cond_resultValue->typecell == constBoolExprClosure);
		cons* retval;

		if (cond_resultValue->val.boolval)
		{
			retval = this->pThen->evaluate();
		}
		else
		{
			retval = this->pElse->evaluate();
		}

		assert(retval->inWHNF);
//		cout << "Updating heap cell " << heap_map[retval] << "(" << retval << ")" <<endl;
		heap_cell = update_heap_refs.top();
		heap_cell->val = retval->val;
		heap_cell->inWHNF = retval->inWHNF;
		heap_cell->typecell = retval->typecell;
//		cout << "If body Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" <<endl;
		//cout << "returning " << retval << " from ifexpr"<<endl;
		return retval;
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

	if ((gc_status != gc_disable && current_heap() < 5) ||
			(getVarExpr()->isFunctionCallExpression() && (current_heap < (5 +((FuncExprNode*)(getVarExpr()))->pListArgs->size()))) )
	{
		ofstream out("GC.txt", ios::app);
		out << "reachable stack before calling GC " << num_of_allocations << endl;
		create_heap_bft(out);
		out.close();
		reachability_gc();
	}

	//Create an entry for the variable where it will be allocated on the heap
	make_reference_addr(this->getVar().c_str(), getfree());
	//ensure that the pointer does not get forwarded unnecessarily.
	cons* temp = getfree();
	temp->forward=NULL;

	//TODO: The assumption is that no GC happens during the creation of the closure. This is not correct and hence we
	//have to find a better way to implement the lazy list.
	cons* var_res = this->getVarExpr()->make_closure();





	cons* retval = this->getBody()->evaluate();
	assert(retval->inWHNF && is_valid_address(retval));

//	out.open("func_call.txt", ios::app);
//	out << "Evaluation stack after evaluating let body " << this->pID->getIDStr() << " after " << num_of_allocations << endl;
//	create_heap_bft(out);
//	out.close();

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

cons* UnaryPrimExprNode::evaluate(cons* heap_cell = NULL)
{
	//cout << "Evaluating unary primop " << this->type << endl;
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
	//ofstream out("func_call.txt", ios::app);
	//cout << "Finished processing unary primnode " << this->type <<endl;
	//create_heap_bft(cout);
	//out.close();
	return retval;
}

cons* UnaryPrimExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = unaryprimopExprClosure; //TODO: need to change this to ensure that the correct type is set
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = pArg->make_closure();
	retval->val.closure.arg2 = NULL;
	retval->inWHNF = false;
	//cout << "created closure at " << retval << " with closure at " << retval->val.closure.arg1 << endl;
	retval->closure_id = ++closure_count;
	return retval;
}


cons* UnaryPrimExprNode::evaluateCarExpr(cons* heap_cell = NULL)
{

	heap_cell = update_heap_refs.top();
	//cout << "Evaluating car for " << heap_map[heap_cell] << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack car expr1 " << heap_map[heap_cell->val.closure.arg1] << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping car expr1 " << update_heap_refs.top()<<endl;
		cons* arg1 = update_heap_refs.top();
		assert(is_valid_address(arg1));
		//cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		arg1->val = temp->val;
		update_heap_refs.pop();

		//cout << "Pushing onto stack car expr2" << arg1->val.cell.car << " with type " << arg1->val.cell.car->typecell << endl;
		assert(arg1->typecell = consExprClosure);
		//cout << "car = " << heap_map[arg1->val.cell.car] << " cdr = " << heap_map[arg1->val.cell.cdr] << endl;
		assert(is_valid_address(arg1->val.cell.car));
		update_heap_refs.push(arg1->val.cell.car);
		cons* retval = reduceParamToWHNF(arg1->val.cell.car);
		//cout << "Popping car expr2 " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();

		heap_cell = update_heap_refs.top();
		assert(retval->inWHNF);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
//		cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
//		cout << "returning " << heap_map[heap_cell] << " from car expr"<<endl;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
return heap_cell;
}

cons* UnaryPrimExprNode::evaluateCdrExpr(cons* heap_cell = NULL)
{

	heap_cell = update_heap_refs.top();
	//cout << "Evaluating cdr for " << heap_map[heap_cell] << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack cdr1 " << heap_map[heap_cell->val.closure.arg1] << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping cdr1 " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure);
//		cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
//		cout << "car = " << heap_map[arg1->val.cell.cdr] << " cdr = " << heap_map[arg1->val.cell.cdr] << endl;
		//cout << "Pushing onto stack cdr2 " << arg1->val.cell.cdr << " with type " << arg1->val.cell.cdr->typecell << endl;
		update_heap_refs.push(arg1->val.cell.cdr);
		cons* retval = reduceParamToWHNF(arg1->val.cell.cdr);
		//cout << "Popping cdr2 " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();

		heap_cell = update_heap_refs.top();
		assert(retval->inWHNF);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
//		cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
//		cout << "returning " << heap_map[heap_cell] << " from cdr expr"<<endl;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
	return heap_cell;
}

//TODO : Evaluation should modify the cons cell and update it. Need to have a handle for the cons cell
// that the expression is representing
cons* UnaryPrimExprNode::evaluateNullqExpr(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack in nullq " << heap_cell->val.closure.arg1 << endl;
		//cout << heap_cell->val.closure.arg1 << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		assert(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping in nullq " << update_heap_refs.top() << endl;
		cons* arg1 = update_heap_refs.top();
		arg1->val = temp->val;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		//cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
		//cout << "Type of heap_cell is " << arg1->typecell << endl;
		heap_cell = update_heap_refs.top();
		heap_cell->inWHNF = true;
		heap_cell->val.boolval = (arg1->typecell == nilExprClosure)? true:false;
		heap_cell->typecell = constBoolExprClosure;
		//cout << "returning " << heap_cell << " from nullexpr"<<endl;
		//cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
	return heap_cell;
}

cons* UnaryPrimExprNode::evaluatePairqExpr(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack pair " << heap_cell->val.closure.arg1 << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping pair " << update_heap_refs.top()<<endl;
		cons* arg1 = update_heap_refs.top();
		arg1->val = temp->val;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		//cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
		heap_cell = update_heap_refs.top();
		heap_cell->inWHNF = true;
		//cout << "Type of heap_cell is " << arg1->typecell << endl;
		heap_cell->val.boolval = (arg1->typecell == consExprClosure);
		heap_cell->typecell = constBoolExprClosure;
		//cout << "returning " << heap_cell << " from pairexpr"<<endl;
		heap_cell->reduction_id = ++reduction_count;
		//cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
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
	//cout << "Evaluating binary primop " << this->type << endl;
	cons* retval = NULL;
	switch(this->type)
	{
	case consExpr : retval = evaluateCons(heap_cell);
	break;
	case addExpr : retval = evaluateAdd(heap_cell);
	break;
	case subExpr : retval = evaluateSub(heap_cell);
	break;
	case mulExpr : retval = evaluateMul(heap_cell);
	break;
	case divExpr : retval = evaluateDiv(heap_cell);
	break;
	case modExpr : retval = evaluateMod(heap_cell);
	break;
	case ltExpr : retval =  evaluateLT(heap_cell);
	break;
	case gtExpr : retval =  evaluateGT(heap_cell);
	break;
	case eqExpr : 	retval =  evaluateEQ(heap_cell);
	break;
	default : retval = NULL;
	}
//	ofstream out("func_call.txt", ios::app);
//	//cout << "Finished processing binary primnop " << this->type <<endl;
//	//create_heap_bft(cout);
//	out.close();
	return retval;
}

cons* BinaryPrimExprNode::make_closure()
{
	cons* retval = (cons*) allocate_cons();

	retval->val.closure.arg1 = pArg1->make_closure();
	retval->val.closure.arg2 = pArg2->make_closure();
	retval->val.closure.expr = this;
	retval->typecell = binaryprimopExprClosure;
	retval->inWHNF = false;
	retval->closure_id = ++closure_count;

	return retval;
}

cons* BinaryPrimExprNode::evaluateCons(cons*cell = NULL)
{
	cons* heap_cell = update_heap_refs.top();
	//cout << "Processing cons cell " << heap_map[heap_cell] << endl;
	assert(is_valid_address(heap_cell));
	if (heap_cell->inWHNF)
		return heap_cell;

	assert(is_valid_address(heap_cell->val.closure.arg1) && is_valid_address(heap_cell->val.closure.arg2));
	heap_cell->inWHNF = true;
	heap_cell->typecell = consExprClosure;
	heap_cell->val.cell.car = heap_cell->val.closure.arg1;
	heap_cell->val.cell.cdr = heap_cell->val.closure.arg2;
//	cout << "Returning " << heap_map[heap_cell] << " from consexpr"<<endl;
//	cout << "car = " << heap_map[heap_cell->val.cell.car] << " cdr = " << heap_map[heap_cell->val.cell.cdr] << endl;
	heap_cell->reduction_id = ++reduction_count;
//	cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
	return heap_cell;
}



cons* BinaryPrimExprNode::evaluateAdd(cons* cell = NULL)
{
	//cout << "In function add" << endl;
	cons* heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.
	cons* arg1 = update_heap_refs.top();
	arg1->inWHNF = temp->inWHNF;
	arg1->val = temp->val;
	arg1->typecell = temp->typecell;
	//POP IT TO ACCESS THE HEAP CELL
//	cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* temp2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	cons* arg2 = update_heap_refs.top();
	arg2->val = temp2->val;
	arg2->inWHNF = temp2->inWHNF;
	arg2->typecell = temp2->typecell;
	update_heap_refs.pop();
//	cout << "Updating heap cell " << heap_map[arg2] << "(" << arg2 << ")" << endl;
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();


	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal + arg2->val.intVal;
	//cout << "returning " << heap_cell << " from addexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
//	cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateSub(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal - arg2->val.intVal;
	//cout << "returning " << heap_cell << " from subexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMul(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal * arg2->val.intVal;
	//cout << "returning " << heap_cell << " from mulexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateDiv(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal / arg2->val.intVal;
	//cout << "returning " << heap_cell << " from divexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMod(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal % arg2->val.intVal;
	//cout << "returning " << heap_cell << " from modexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateLT(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal < arg2->val.intVal;
	//cout << "returning " << heap_cell << " from ltexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateGT(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal > arg2->val.intVal;
	//cout << "returning " << heap_cell << " from gtexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateEQ(cons* heap_cell = NULL)
{
//	cout << "Processing eq " << heap_map[heap_cell] << endl;
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	assert(arg1 == update_heap_refs.top());
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);
	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	assert(arg2 == update_heap_refs.top());
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

//	cout << "Completed processing arg2 in EQ?"<<endl;
//	cout << "Type of arg1 = " << print_cell_type(arg1->typecell) << endl;
//	cout << "Type of arg2 = " << print_cell_type(arg2->typecell) << endl;

	assert(is_valid_address(arg1) && is_valid_address(arg2));

	bool isequal = false;
	if (arg1->typecell == arg2->typecell)
	{
		switch(arg1->typecell)
		{
		case constStringExprClosure: isequal = (arg1->val.stringVal == arg2->val.stringVal) ||
				                                (0 == strcmp(arg1->val.stringVal->c_str(), arg2->val.stringVal->c_str()));
//		cout << "Comparing Strings " << (*arg1->val.stringVal) << " & " << (*arg2->val.stringVal) << endl;
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
	//cout << "returning " << heap_cell << " from eqexpr"<<endl;
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


	heap_cell = update_heap_refs.top();
//	cout << "heap cell type before updating " << print_cell_type(heap_cell->typecell) << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	DefineNode* funcDef = (DefineNode*)pgm->getFunction(this->getFunction());
	make_environment(funcDef->getFuncName().c_str(), "");
	auto num_args = pListArgs->size();
	auto curr = heap_cell;
	while(num_args > 0)
	{
//		cout << "Creating reference for " << (funcDef->getArgs()[num_args-1]) << " at "
//				<< heap_map[curr->val.closure.arg2] << "(" << heap_cell  << ")" << endl;
		make_reference_addr((funcDef->getArgs()[num_args-1]).c_str(), curr->val.closure.arg2);
		--num_args;
		if (curr->val.closure.arg1 != NULL)
		{
			curr = curr->val.closure.arg1;
		}
	}

	cons *temp = funcDef->getFunctionBody()->evaluate();
	assert(temp->inWHNF);

	heap_cell = update_heap_refs.top();
//	if (heap_map[heap_cell] < root_var_map.size())
//		cout << "Updating root varibale " << root_var_map[heap_map[heap_cell]] << " at " <<
//		      heap_map[heap_cell] << "(" << heap_cell  << ")"<<" from type " << print_cell_type(heap_cell->typecell) << endl;
//	else
//		cout << "Updating " << heap_map[heap_cell] << " from type " << print_cell_type(heap_cell->typecell) << endl;
	heap_cell->val = temp->val;
	heap_cell->typecell = temp->typecell;
	heap_cell->inWHNF = true;
//	cout << "returning " << heap_map[heap_cell] << " from funcall " << funcDef->getFuncName()<<
//					" with type "<< print_cell_type(heap_cell->typecell)<<endl;

//	if (heap_cell->typecell == consExprClosure)
//	{
//		cout << "car = " << heap_map[heap_cell->val.cell.car] << " & cdr = " << heap_map[heap_cell->val.cell.cdr] << endl;
//	}
//	cout << "eval stack after updating func return value " << endl;
//	create_heap_bft(cout);
//	cout << endl;
	delete_environment();
	assert(heap_cell->typecell == temp->typecell);
//	cout << "heap_cell  " << heap_map[heap_cell] << " temp type " << heap_map[temp] << endl;
	heap_cell->reduction_id = ++reduction_count;

	return heap_cell;
}


cons* FuncExprNode::make_closure()
{
	int num_args = pListArgs->size() - 1;

	cons* retval = (cons*)allocate_cons();
	auto rarglistiter = this->pListArgs->rbegin();

	//cout << "Num of arguments " << pListArgs->size() << " for fn call " << this->getFunction()<<endl;
	retval->typecell=funcApplicationExprClosure;
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = NULL;
	retval->val.closure.arg2 = NULL;
	if (pListArgs->size() > 0)
	{
		//cout << lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str()) << " " << retval << endl;
		retval->val.closure.arg2 = lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
		//cout << "Creating argument " << (((IdExprNode*)(*rarglistiter))->getIDStr()) << " at " << retval->val.closure.arg2 << endl;
		retval->inWHNF = false;
		auto prev = retval;
		while(num_args > 0)
		{
			++rarglistiter;
			auto curr = allocate_cons();
			curr->inWHNF = false;
			curr->typecell = funcArgClosure;
			curr->val.closure.arg2 =  lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
			//cout << "Creating argument " << (((IdExprNode*)(*rarglistiter))->getIDStr()) << " at " << curr->val.closure.arg2 << endl;
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
	std::cout << "Evaluating main expression " << std::endl;
	return this->pExpr->evaluate();
}

Node* ProgramNode::getFunction(std::string funcName)
{
	return this->funcmap[funcName];
}





=======
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
extern int gcinvoke;
int maxcount;
extern double gctime;
ProgramNode* pgm;
unsigned long lbl_count = 0;
map< string, vector<ExprNode*>> func_call_points;
unsigned int closure_count = 0;
unsigned int reduction_count = 0;
extern unsigned int num_of_allocations;
extern map<cons*, int> heap_map;
extern map<int, string> root_var_map;

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

	assert(is_valid_address(heap_cell));
	if (heap_cell->inWHNF)
	{
		return heap_cell;
	}
	else
	{
		cons* retval = heap_cell;
		cons *temp = (retval->val.closure.expr->evaluate());
		retval=temp;

		assert(retval->inWHNF);
		assert(is_valid_address(retval));

		heap_cell = update_heap_refs.top();
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
	cons* retval = lookup_addr(this->pID->c_str());
			
	if (retval->inWHNF)
	{
		return retval;
	}
	else
	{

		update_heap_refs.push(retval);
		cons* temp = retval->val.closure.expr->evaluate();

		heap_cell = update_heap_refs.top();
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
	//label = (shouldAddLabel) ? getNextLabel() : "";
	pID->doLabel(shouldAddLabel);
	label = pID->getLabel();
}

ReturnExprNode * ReturnExprNode::clone() const
{
	return new ReturnExprNode(pID->clone());
}

cons* ReturnExprNode::evaluate(cons* heap_cell = NULL)
{

	//cout << " Starting processing return for  " << this->pID->getName()<< endl;
	std::string varName = this->pID->getName();
	cons* retval = (cons*)lookup_addr(varName.c_str());
	if (!retval->inWHNF)
	{

		update_heap_refs.push(retval);
		cons* temp = retval->val.closure.expr->evaluate();
		retval = update_heap_refs.top();

		assert(temp->inWHNF && retval->inWHNF);

		retval->inWHNF = true;
		retval->typecell = temp->typecell;
		retval->val = temp->val;\

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

cons* NilConstExprNode::evaluate(cons* heap_cell = NULL)
{
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

cons* IntConstExprNode::evaluate(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
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
	retval->reduction_id = ++reduction_count;
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
	retval->reduction_id = ++reduction_count;
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
	retval->reduction_id = ++reduction_count;
	return retval;
}

cons* BoolConstExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = constBoolExprClosure;
	retval->val.boolval = *pBoolVal;
	retval->inWHNF = true;
	//cout << "Stored boolean value " << retval->val.boolval << " at " << retval << endl;
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
//	assert(update_heap_refs.size());
//	heap_cell = update_heap_refs.top();
//	if (heap_cell->inWHNF)
//	{
//		//cout << "returning already evaluated result"<<endl;
//		return heap_cell;
//	}
//	else
	{
		//cout << "Processing if-then-else"<<endl;
		//The condition is always a variable. Get the heap location associated with it
		//by looking up in the stack.
		IdExprNode* i = (IdExprNode*)this->pCond;
		cons* cond_heap_ref = (cons*)lookup_addr(i->getIDStr().c_str());

		update_heap_refs.push(cond_heap_ref);
		//cout << "Pushing on to stack in if expr " << cond_heap_ref << endl;
		cons* cond_resultValue = this->pCond->evaluate();
		//cout << "Popping in if expr " << update_heap_refs.top()<<endl;
		cons* temp = update_heap_refs.top();
		temp->inWHNF = cond_resultValue->inWHNF;
		temp->typecell = cond_resultValue->typecell;
		temp->val = cond_resultValue->val;
//		cout << "If condition Updating heap cell " << heap_map[temp] << "(" << temp << ")" <<endl;
		update_heap_refs.pop();

		assert(cond_resultValue->typecell == constBoolExprClosure);
		cons* retval;

		if (cond_resultValue->val.boolval)
		{
			retval = this->pThen->evaluate();
		}
		else
		{
			retval = this->pElse->evaluate();
		}

		assert(retval->inWHNF);
//		cout << "Updating heap cell " << heap_map[retval] << "(" << retval << ")" <<endl;

		//Commented the below code to check if having a condition as the main expression works		
		
//		heap_cell = update_heap_refs.top();
//		heap_cell->val = retval->val;
//		heap_cell->inWHNF = retval->inWHNF;
//		heap_cell->typecell = retval->typecell;
//		cout << "If body Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" <<endl;
		
		//cout << "returning " << retval << " from ifexpr"<<endl;
		return retval;
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
//	cout << "Processing let variable " << this->pID->getIDStr() << " at label " << getLabel() <<  endl;
	
	if ((gc_status != gc_disable && current_heap() < 1) ||
			(getVarExpr()->isFunctionCallExpression() && (current_heap < (0 + ((FuncExprNode*)(getVarExpr()))->pListArgs->size()))) )
	{
//		ofstream out("GC.txt", ios::app);
//		out << "reachable stack before calling GC " << num_of_allocations << endl;
//		create_heap_bft(out);
//		out.close();
		if (gc_status != gc_live)
		{
			std::cout << "Reachability based GC" << endl;
			reachability_gc();
		}
		else
		{
			std::cout << "Liveness based GC at pgm pt " << getLabel() << endl;
			std::cout << "Number of elements in  heap ref stack = " << update_heap_refs.size() << endl;
			std::string curr_let_pgmpt = return_stack().return_point;
			return_stack().return_point = getLabel();
			liveness_gc();
			return_stack().return_point = curr_let_pgmpt;
		}
	}

	
	//Create an entry for the variable where it will be allocated on the heap
	make_reference_addr(this->getVar().c_str(), getfree());
	//ensure that the pointer does not get forwarded unnecessarily.
	cons* temp = getfree();
	temp->forward=NULL;

	//If VarExpr is a function call, store the pgmpt of the let as the return point for liveness based GC 
	if (getVarExpr()->isFunctionCallExpression())
	{
		FuncExprNode* funExpr = (FuncExprNode*)getVarExpr();
		funExpr->parent_let_pgmpt = getLabel();
	}
	
	cons* var_res = this->getVarExpr()->make_closure();
	cons* retval = this->getBody()->evaluate();
	assert(retval->inWHNF && is_valid_address(retval));

//	out.open("func_call.txt", ios::app);
//	out << "Evaluation stack after evaluating let body " << this->pID->getIDStr() << " after " << num_of_allocations << endl;
//	create_heap_bft(out);
//	out.close();
	
	
	
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

cons* UnaryPrimExprNode::evaluate(cons* heap_cell = NULL)
{
	//cout << "Evaluating unary primop " << this->type << endl;
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
	//ofstream out("func_call.txt", ios::app);
	//cout << "Finished processing unary primnode " << this->type <<endl;
	//create_heap_bft(cout);
	//out.close();
	return retval;
}

cons* UnaryPrimExprNode::make_closure()
{
	cons* retval = (cons*)allocate_cons();
	retval->typecell = unaryprimopExprClosure; //TODO: need to change this to ensure that the correct type is set
	retval->val.closure.expr = this;
	retval->val.closure.arg1 = pArg->make_closure();
	retval->val.closure.arg2 = NULL;
	retval->inWHNF = false;
	//cout << "created closure at " << retval << " with closure at " << retval->val.closure.arg1 << endl;
	retval->closure_id = ++closure_count;
	return retval;
}


cons* UnaryPrimExprNode::evaluateCarExpr(cons* heap_cell = NULL)
{

	heap_cell = update_heap_refs.top();
	//cout << "Evaluating car for " << heap_map[heap_cell] << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack car expr1 " << heap_map[heap_cell->val.closure.arg1] << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping car expr1 " << update_heap_refs.top()<<endl;
		cons* arg1 = update_heap_refs.top();
		assert(is_valid_address(arg1));
		//cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		arg1->val = temp->val;
		update_heap_refs.pop();

		//cout << "Pushing onto stack car expr2" << arg1->val.cell.car << " with type " << arg1->val.cell.car->typecell << endl;
		assert(arg1->typecell = consExprClosure);
		//cout << "car = " << heap_map[arg1->val.cell.car] << " cdr = " << heap_map[arg1->val.cell.cdr] << endl;
		assert(is_valid_address(arg1->val.cell.car));
		update_heap_refs.push(arg1->val.cell.car);
		cons* retval = reduceParamToWHNF(arg1->val.cell.car);
		//cout << "Popping car expr2 " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();

		heap_cell = update_heap_refs.top();
		assert(retval->inWHNF);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
//		cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
//		cout << "returning " << heap_map[heap_cell] << " from car expr"<<endl;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
return heap_cell;
}

cons* UnaryPrimExprNode::evaluateCdrExpr(cons* heap_cell = NULL)
{

	heap_cell = update_heap_refs.top();
	//cout << "Evaluating cdr for " << heap_map[heap_cell] << endl;
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack cdr1 " << heap_map[heap_cell->val.closure.arg1] << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping cdr1 " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure);
//		cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
//		cout << "car = " << heap_map[arg1->val.cell.cdr] << " cdr = " << heap_map[arg1->val.cell.cdr] << endl;
		//cout << "Pushing onto stack cdr2 " << arg1->val.cell.cdr << " with type " << arg1->val.cell.cdr->typecell << endl;
		update_heap_refs.push(arg1->val.cell.cdr);
		cons* retval = reduceParamToWHNF(arg1->val.cell.cdr);
		//cout << "Popping cdr2 " << update_heap_refs.top()<<endl;
		update_heap_refs.pop();

		heap_cell = update_heap_refs.top();
		assert(retval->inWHNF);
		heap_cell->typecell = retval->typecell;
		heap_cell->val = retval->val;
		heap_cell->inWHNF = true;
//		cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
//		cout << "returning " << heap_map[heap_cell] << " from cdr expr"<<endl;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
	return heap_cell;
}

//TODO : Evaluation should modify the cons cell and update it. Need to have a handle for the cons cell
// that the expression is representing
cons* UnaryPrimExprNode::evaluateNullqExpr(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack in nullq " << heap_cell->val.closure.arg1 << endl;
		//cout << heap_cell->val.closure.arg1 << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		assert(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping in nullq " << update_heap_refs.top() << endl;
		cons* arg1 = update_heap_refs.top();
		arg1->val = temp->val;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		//cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
		//cout << "Type of heap_cell is " << arg1->typecell << endl;
		heap_cell = update_heap_refs.top();
		heap_cell->inWHNF = true;
		heap_cell->val.boolval = (arg1->typecell == nilExprClosure)? true:false;
		heap_cell->typecell = constBoolExprClosure;
		//cout << "returning " << heap_cell << " from nullexpr"<<endl;
		//cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
		heap_cell->reduction_id = ++reduction_count;
		return heap_cell;
	}
	return heap_cell;
}

cons* UnaryPrimExprNode::evaluatePairqExpr(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;
	else
	{
		//cout << "Pushing onto stack pair " << heap_cell->val.closure.arg1 << endl;
		update_heap_refs.push(heap_cell->val.closure.arg1);
		cons* temp = reduceParamToWHNF(heap_cell->val.closure.arg1);
		//cout << "Popping pair " << update_heap_refs.top()<<endl;
		cons* arg1 = update_heap_refs.top();
		arg1->val = temp->val;
		arg1->typecell = temp->typecell;
		arg1->inWHNF = temp->inWHNF;
		//cout << "Updating heap cell " << heap_map[arg1] << "(" << arg1 << ")" << endl;
		update_heap_refs.pop();

		assert(is_valid_address(arg1));
		assert(arg1->typecell == consExprClosure || arg1->typecell == nilExprClosure);
		heap_cell = update_heap_refs.top();
		heap_cell->inWHNF = true;
		//cout << "Type of heap_cell is " << arg1->typecell << endl;
		heap_cell->val.boolval = (arg1->typecell == consExprClosure);
		heap_cell->typecell = constBoolExprClosure;
		//cout << "returning " << heap_cell << " from pairexpr"<<endl;
		heap_cell->reduction_id = ++reduction_count;
		//cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
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
	//cout << "Evaluating binary primop " << this->type << endl;
	cons* retval = NULL;
	switch(this->type)
	{
	case consExpr : retval = evaluateCons(heap_cell);
	break;
	case addExpr : retval = evaluateAdd(heap_cell);
	break;
	case subExpr : retval = evaluateSub(heap_cell);
	break;
	case mulExpr : retval = evaluateMul(heap_cell);
	break;
	case divExpr : retval = evaluateDiv(heap_cell);
	break;
	case modExpr : retval = evaluateMod(heap_cell);
	break;
	case ltExpr : retval =  evaluateLT(heap_cell);
	break;
	case gtExpr : retval =  evaluateGT(heap_cell);
	break;
	case eqExpr : 	retval =  evaluateEQ(heap_cell);
	break;
	default : retval = NULL;
	}
//	ofstream out("func_call.txt", ios::app);
//	//cout << "Finished processing binary primnop " << this->type <<endl;
//	//create_heap_bft(cout);
//	out.close();
	return retval;
}

cons* BinaryPrimExprNode::make_closure()
{
	cons* retval = (cons*) allocate_cons();

	retval->val.closure.arg1 = pArg1->make_closure();
	retval->val.closure.arg2 = pArg2->make_closure();
	retval->val.closure.expr = this;
	retval->typecell = binaryprimopExprClosure;
	retval->inWHNF = false;
	retval->closure_id = ++closure_count;

	return retval;
}

cons* BinaryPrimExprNode::evaluateCons(cons*cell = NULL)
{
	cons* heap_cell = update_heap_refs.top();
	//cout << "Processing cons cell " << heap_map[heap_cell] << endl;
	assert(is_valid_address(heap_cell));
	if (heap_cell->inWHNF)
		return heap_cell;

	assert(is_valid_address(heap_cell->val.closure.arg1) && is_valid_address(heap_cell->val.closure.arg2));
	heap_cell->inWHNF = true;
	heap_cell->typecell = consExprClosure;
	heap_cell->val.cell.car = heap_cell->val.closure.arg1;
	heap_cell->val.cell.cdr = heap_cell->val.closure.arg2;
//	cout << "Returning " << heap_map[heap_cell] << " from consexpr"<<endl;
//	cout << "car = " << heap_map[heap_cell->val.cell.car] << " cdr = " << heap_map[heap_cell->val.cell.cdr] << endl;
	heap_cell->reduction_id = ++reduction_count;
//	cout << "Updating heap cell " << heap_map[heap_cell] << "(" << heap_cell << ")" << endl;
	return heap_cell;
}



cons* BinaryPrimExprNode::evaluateAdd(cons* cell = NULL)
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

	heap_cell->reduction_id = ++reduction_count;

	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateSub(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
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

	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMul(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal * arg2->val.intVal;
	//cout << "returning " << heap_cell << " from mulexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateDiv(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal / arg2->val.intVal;
	//cout << "returning " << heap_cell << " from divexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateMod(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constIntExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal % arg2->val.intVal;
	//cout << "returning " << heap_cell << " from modexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateLT(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal < arg2->val.intVal;
	//cout << "returning " << heap_cell << " from ltexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateGT(cons* heap_cell = NULL)
{
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
			return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

	assert(arg1->typecell == constIntExprClosure && arg2->typecell == constIntExprClosure);
	heap_cell = update_heap_refs.top();
	heap_cell->typecell = constBoolExprClosure;
	heap_cell->inWHNF = true;
	heap_cell->val.intVal = arg1->val.intVal > arg2->val.intVal;
	//cout << "returning " << heap_cell << " from gtexpr"<<endl;
	heap_cell->reduction_id = ++reduction_count;
	return heap_cell;
}

cons* BinaryPrimExprNode::evaluateEQ(cons* heap_cell = NULL)
{
//	cout << "Processing eq " << heap_map[heap_cell] << endl;
	heap_cell = update_heap_refs.top();
	if (heap_cell->inWHNF)
		return heap_cell;

	//cout << "Pushing onto stack " << heap_cell->val.closure.arg1 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg1);
	cons* arg1 = reduceParamToWHNF(heap_cell->val.closure.arg1);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	assert(arg1 == update_heap_refs.top());
	//CANNOT IMMEDIATELY POP THE FIRST PARAMETER. NEED TO KEEP IT ON THE STACK
	//SO THAT IF A GC HAPPENS DURING THE PROCESSING OF THE SECOND PARAMETER
	//THE POINTER TO THE FIRST PARAMETER ALSO GETS UPDATED.

	//POP IT TO ACCESS THE HEAP CELL
	update_heap_refs.pop();

	//cout << "Completed processing arg1"<<endl;
	heap_cell = update_heap_refs.top();
	//PUSH IT BACK AGAIN !!!!!!
	update_heap_refs.push(arg1);
	//cout << "Pushing onto stack " << heap_cell->val.closure.arg2 << endl;
	update_heap_refs.push(heap_cell->val.closure.arg2);
	cons* arg2 = reduceParamToWHNF(heap_cell->val.closure.arg2);
	//cout << "Popping " << update_heap_refs.top()<<endl;
	assert(arg2 == update_heap_refs.top());
	update_heap_refs.pop();
	//NOW IT CAN BE SAFELY TRANSFERRED. THEN POP THE FIRST ARGUMENT ALSO.
	arg1 = update_heap_refs.top();
	update_heap_refs.pop();

//	cout << "Completed processing arg2 in EQ?"<<endl;
//	cout << "Type of arg1 = " << print_cell_type(arg1->typecell) << endl;
//	cout << "Type of arg2 = " << print_cell_type(arg2->typecell) << endl;

	assert(is_valid_address(arg1) && is_valid_address(arg2));

	bool isequal = false;
	if (arg1->typecell == arg2->typecell)
	{
		switch(arg1->typecell)
		{
		case constStringExprClosure: isequal = (arg1->val.stringVal == arg2->val.stringVal) ||
				                                (0 == strcmp(arg1->val.stringVal->c_str(), arg2->val.stringVal->c_str()));
//		cout << "Comparing Strings " << (*arg1->val.stringVal) << " & " << (*arg2->val.stringVal) << endl;
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
	//cout << "returning " << heap_cell << " from eqexpr"<<endl;
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


	heap_cell = update_heap_refs.top();

	if (heap_cell->inWHNF)
		return heap_cell;
	DefineNode* funcDef = (DefineNode*)pgm->getFunction(this->getFunction());
	//TODO : WHAT SHOULD BE THE PROGRAM POINT TO BE PASSED? 
	make_environment(funcDef->getFuncName().c_str(), this->parent_let_pgmpt);
	auto num_args = pListArgs->size();
	auto curr = heap_cell;
	while(num_args > 0)
	{
		make_reference_addr((funcDef->getArgs()[num_args-1]).c_str(), curr->val.closure.arg2);
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

	assert(heap_cell->typecell == temp->typecell);
	heap_cell->reduction_id = ++reduction_count;

	return heap_cell;
}


cons* FuncExprNode::make_closure()
{
	int num_args = pListArgs->size() - 1;

	cons* retval = (cons*)allocate_cons();
	auto rarglistiter = this->pListArgs->rbegin();


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
			curr->typecell = funcArgClosure;
			curr->val.closure.arg2 =  lookup_addr(((IdExprNode*)(*rarglistiter))->getIDStr().c_str());
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
	std::cout << "Evaluating main expression " << std::endl;
	return this->pExpr->evaluate();
}

Node* ProgramNode::getFunction(std::string funcName)
{
	return this->funcmap[funcName];
}





>>>>>>> 4bb03a5c3d7b9f8e48b056d77b2ef7801e7effa3
