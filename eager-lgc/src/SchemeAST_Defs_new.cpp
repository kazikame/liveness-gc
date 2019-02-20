#include "SchemeAST_new.h"
#include "gc.h"
#include <list>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <cstring>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <boost/variant.hpp>
#include "Simulator.h"

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
//extern unordered_map<std::string, std::string> call_next;
unsigned long lbl_count = 0;
map< string, vector<ExprNode*>> func_call_points;


// Added by Saksham
// REVERSED CALL GRAPH FUNCTION

unordered_map<string, EdgeSet> ProgramNode::makeRevCallGraph()
{
	unordered_map<string, EdgeSet> result((*pListDefines).size());

	for (auto i : *pListDefines)
	{
		string funcName = i->getFuncName();
		result[funcName] = {};
	}

	for (auto i: *pListDefines)
	{
		string funcName = i->getFuncName();

		for (string j : i->makeCallGraph())
		{
			cout<<"Adding "<<funcName<<" to "<<j<<'\n';
			result[j].insert(funcName);
		}

	}

	return result;
}

EdgeSet DefineNode::makeCallGraph()
{
	EdgeSet result;
	pExpr->dependentFunctions(result);
	return result;
}

EdgeSet FuncExprNode::dependentFunctions(EdgeSet& s)
{
	s.insert(getFunction());
	return s;
}

EdgeSet LetExprNode::dependentFunctions(EdgeSet& s)
{
	pExpr->dependentFunctions(s);
	pBody->dependentFunctions(s);
	return s;
}

EdgeSet IfExprNode::dependentFunctions(EdgeSet& s)
{
	pCond->dependentFunctions(s);
	pThen->dependentFunctions(s);
	pElse->dependentFunctions(s);
}

//For every other type of ExprNode
EdgeSet ExprNode::dependentFunctions(EdgeSet& s)
{
	return s;
}

// Added by Saksham

//OLD CODE BEGINS HERE
void print_arg_type(resType r)
{
		switch(r)
		{
		case intType : cout << "Integer type " << endl; break;
		case boolType : cout << "Boolean type " << endl; break;
		case stringType : cout << "String type " << endl; break;
		case heap : cout << "heap ref type " << endl; break;
		default : 	cout << "Error type " << endl; break;
		}
	return;
}

void print_result(resultValue r)
{

		switch(r.type)
		{
		case intType : cout << "Integer type =" << r.val.intVal << endl; break;
		case boolType : cout << "Boolean type " << r.val.boolVal<<endl; break;
		case stringType : cout << "String type " << r.val.stringVal<< endl; break;
		case heap : cout << "heap ref type " ;
		           	printval(r.val.addrVal);
		            cout<<endl;
		            break;
		default : 	cout << "Error type " << endl; break;
		}
	return;
}

std::string getNextLabel()
{
	std::stringstream sstream;
	//sstream <<  "p#" << lbl_count++;
	sstream <<  lbl_count++;
	return sstream.str();
}


resultValue::resultValue(resType t, int v)
{
	assert(t==intType);
	type = t;
	//cout << "Assigning value " << val<<endl;
	this->val.intVal = v;
}
resultValue::resultValue(resType t, bool v)
{
	//cout << "Created new boolean value"<<endl;
	assert(t==boolType);
	type = t;
	this->val.boolVal = v;
}
resultValue::resultValue(resType t, string *v)
{
	assert(t==stringType);
	type = t;
	this->val.stringVal = v;
}
resultValue::resultValue(resType t, void* v)
{
	assert(t==heap);
	type = t;
	this->val.addrVal = v;
}

resultValue::resultValue(const resultValue& r)
{
	type = r.type;
	val = r.val;
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

IdExprNode * IdExprNode::clone() const {
	return new IdExprNode(new std::string(*pID));
}

void IdExprNode::doLabel(bool shouldAddLabel) {
	string l = getNextLabel();
	label = (shouldAddLabel ? l : "");
}

resultValue IdExprNode::evaluate()
{
	std::string varName = *(this->pID);
	char resultValueType = locate_var(varName.c_str());

	if( resultValueType == 'a')
		return resultValue(heap, lookup_addr(varName.c_str()));
	else if (resultValueType == 'i')
		return resultValue(intType, (int)lookup_value(varName.c_str()));
	else if (resultValueType == 'b')
		return resultValue(boolType, (bool)lookup_value(varName.c_str()));
	else if (resultValueType == 's')
		return resultValue(stringType, (string*)lookup_value(varName.c_str()));
	else
		return resultValue(errorType, (void*)NULL);
}


// LivenessInfo IdExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second.assign(lin.second.begin(), lin.second.end());
// 	l.second.push_back(this);
// 	return l.second;

// }


ReturnExprNode::~ReturnExprNode() {
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

resultValue ReturnExprNode::evaluate()
{
	//cout << "Evaluating " << this->getLabel()<<endl;
	return pID->evaluate();
}

// LivenessInfo ReturnExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second = this->pID->analyse(lin);
// 	return l.second;
// }


/*  Definitions for CONSTANT nodes */

ConstExprNode::~ConstExprNode() {}

ConstExprNode::ConstExprNode(const std::string name) : ExprNode(name) {}

void ConstExprNode::doLabel(bool shouldAddLabel) {
	label = (shouldAddLabel ? getNextLabel() : "");
}

HoleConstExprNode::HoleConstExprNode() : ConstExprNode("HOLE") {}

HoleConstExprNode * HoleConstExprNode::clone() const {
	return new HoleConstExprNode();
}

resultValue HoleConstExprNode::evaluate()
{
	return resultValue(errorType, (void*) NULL);
}

// LivenessInfo HoleConstExprNode::analyse(Liveness lin)
// {
// 	return lin.second;
// }


NilConstExprNode::NilConstExprNode() : ConstExprNode("NIL") {}

NilConstExprNode * NilConstExprNode::clone() const {
	return new NilConstExprNode();
}

resultValue NilConstExprNode::evaluate()
{
	//	cout << "returning NULL value" << endl;
	//cout << "Evaluating " << this->getLabel()<<endl;
	return resultValue(heap, (void*)NULL);
}



IntConstExprNode::~IntConstExprNode()
{
	delete pIntVal;
}

IntConstExprNode::IntConstExprNode(int * ival) : ConstExprNode("INT"), pIntVal(ival) {}

IntConstExprNode * IntConstExprNode::clone() const {
	return new IntConstExprNode(new int(*pIntVal));
}

resultValue IntConstExprNode::evaluate()
{
	//cout << "Evaluating " << this->getLabel()<<endl;
	return resultValue(intType, this->getVal());
}

// LivenessInfo IntConstExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second = lin.second;
// 	return l.second;
// }

int IntConstExprNode::getVal()
{
	return *(this->pIntVal);

}


StrConstExprNode::~StrConstExprNode() {
	delete pStrVal;
}

StrConstExprNode::StrConstExprNode(std::string * sval) : ConstExprNode("STR"), pStrVal(sval) {}

StrConstExprNode * StrConstExprNode::clone() const {
	return new StrConstExprNode(new std::string(*pStrVal));
}

resultValue StrConstExprNode::evaluate()
{
	//	cout << "Creating a string constant for " << this->getVal()->c_str() << endl;
	return resultValue(stringType, this->getVal());
}

// LivenessInfo StrConstExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second = lin.second;
// 	return l.second;
// }

string* StrConstExprNode::getVal()
{
	//	cout << "Returning string value " << this->pStrVal->c_str() << endl;
	return (this->pStrVal);
}




BoolConstExprNode::~BoolConstExprNode() {
	delete pBoolVal;
}

BoolConstExprNode::BoolConstExprNode(bool * bval) : ConstExprNode("BOOL"), pBoolVal(bval) {}

resultValue BoolConstExprNode::evaluate()
{
	//cout << "Evaluating " << this->getLabel()<<endl;
	return resultValue(boolType, this->getVal());
}

BoolConstExprNode * BoolConstExprNode::clone() const {
	return new BoolConstExprNode(new bool(*pBoolVal));
}

bool BoolConstExprNode::getVal()
{
	return *(this->pBoolVal);
}

// LivenessInfo BoolConstExprNode::analyse(Liveness lin )
// {
// 	l.first = getLabel();
// 	l.second = lin.second;
// 	return l.second;}



// /*  Definitions for IF node */

// IfExprNode::~IfExprNode() {
// 	delete pCond;
// 	delete pThen;
// 	delete pElse;
// }

IfExprNode::IfExprNode(ExprNode * cond, ExprNode * then_expr, ExprNode * else_expr)
: ExprNode("IF"), pCond(cond), pThen(then_expr), pElse(else_expr) {}

void IfExprNode::doLabel(bool shouldAddLabel)
{
	pCond->doLabel(shouldAddLabel);
	pThen->doLabel(shouldAddLabel);
	pElse->doLabel(shouldAddLabel);
	label = (shouldAddLabel ? getNextLabel() : "");
}

IfExprNode * IfExprNode::clone() const {
	return new IfExprNode(pCond->clone(), pThen->clone(), pElse->clone());

}

resultValue IfExprNode::evaluate()
{
	//cout << "Evaluating " << this->getLabel()<<endl;
	resultValue cond_resultValue = this->getCond()->evaluate();

	if (cond_resultValue.val.boolVal)
		return this->getThenPart()->evaluate();
	else
		return this->getElsePart()->evaluate();
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

// LivenessInfo IfExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	//Check liveness of the Then branch
// 	Liveness lThen;
// 	lThen.first = pThen->getLabel();
// 	lThen.second = pThen->analyse(lin);
// 	//Check liveness of the Else branch
// 	Liveness lElse;
// 	lElse.first = pElse->getLabel();
// 	lElse.second = pElse->analyse(lin);

// 	//Merge the liveness info
// 	this->l.second.insert(l.second.end(), lThen.second.begin(), lThen.second.end());
// 	this->l.second.insert(l.second.end(), lElse.second.begin(), lElse.second.end());

// 	//Check liveness of the condition expression using the merged liveness
// 	l.second = pCond->analyse(l);

// 	return l.second;
// }



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

LetExprNode * LetExprNode::clone() const {
	return new LetExprNode(pID->clone(), pExpr->clone(), pBody->clone());
}

resultValue LetExprNode::evaluate()
{
	resultValue var_res = this->getVarExpr()->evaluate();

	//cout << "Completed processing let expression " << endl;
	switch(var_res.type)
	{
	case intType : make_reference_value(this->getVar().c_str(), int_to_void(var_res.val.intVal), 'i');
	break;
	case boolType : make_reference_value(this->getVar().c_str(), int_to_void((int)var_res.val.boolVal), 'b');
	break;
	case stringType : make_reference_value(this->getVar().c_str(), char_to_void(var_res.val.stringVal), 's');
	break;
	default : make_reference_addr(this->getVar().c_str(), var_res.val.addrVal);
	break;
	}
	//cout << "Evaluating let body now" << endl;
	return this->getBody()->evaluate();
	//cout << "Completed evaluating let body " << endl;
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

// LivenessInfo LetExprNode::analyse(Liveness lin)
// {
// 	this->l.first = getLabel();
// 	l.second = this->pExpr->analyse(lin);

// 	if (find(l.second.begin(), l.second.end(), pID) != l.second.end())
// 	{
// 		l.second.erase(find(l.second.begin(), l.second.end(), pID));
// 	}
// 	l.second = this->pExpr->analyse(l);
// 	return l.second;
// }



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

UnaryPrimExprNode * UnaryPrimExprNode::clone() const {
	return new UnaryPrimExprNode(node_name, pArg->clone());
}

resultValue UnaryPrimExprNode::evaluate()
{
	//cout << "Evaluating " << this->getLabel()<<endl;
	switch(this->type)
	{
	case carExpr : return evaluateCarExpr();
	break;
	case cdrExpr : return evaluateCdrExpr();
	break;
	case nullqExpr : return evaluateNullqExpr();
	break;
	case pairqExpr : return evaluatePairqExpr();
	break;
	default : return resultValue(errorType, (void*)NULL);
	}
}



resultValue UnaryPrimExprNode::evaluateCarExpr()
{

	resultValue r = this->pArg->evaluate();

	switch (getType(r.val.addrVal, 1))
	{
	case 2 : return resultValue(intType, void_to_int(getCar(r.val.addrVal, 0)));
	break;
	case 3:  return resultValue(stringType, void_to_char(getCar(r.val.addrVal, 0)));
	break;
	case 4: return resultValue(boolType, (bool)void_to_int(getCar(r.val.addrVal, 0)));
	break;
	default : return resultValue(heap, getCar(r.val.addrVal, 0));
	break;
	}
	return resultValue(errorType, (void*)NULL);

}

resultValue UnaryPrimExprNode::evaluateCdrExpr()
{

	resultValue r = this->pArg->evaluate();


	switch (getType(r.val.addrVal, 2))
	{
	case 2 : return resultValue(intType, void_to_int(getCdr(r.val.addrVal, 0)));
	break;
	case 3:  return resultValue(stringType, void_to_char(getCdr(r.val.addrVal, 0)));
	break;
	case 4: return resultValue(boolType, (bool)void_to_int(getCar(r.val.addrVal, 0)));
	break;
	default : return resultValue(heap, getCdr(r.val.addrVal, 0));
	break;
	}
	return resultValue(errorType, (void*)NULL);

}

resultValue UnaryPrimExprNode::evaluateNullqExpr()
{
	resultValue r = this->pArg->evaluate();

	assert(r.type == heap);
	return resultValue(boolType, r.val.addrVal == NULL);
}

resultValue UnaryPrimExprNode::evaluatePairqExpr()
{
	resultValue r = this->pArg->evaluate();
	assert(r.type == heap);
	return resultValue(boolType, r.val.addrVal != NULL);
}

// LivenessInfo UnaryPrimExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second = this->pArg->analyse(lin);

// 	return l.second;

// }

BinaryPrimExprNode::~BinaryPrimExprNode() {
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

BinaryPrimExprNode * BinaryPrimExprNode::clone() const {
	return new BinaryPrimExprNode(node_name, pArg1->clone(), pArg2->clone());
}

resultValue BinaryPrimExprNode::evaluate()
{
	switch(this->type)
	{
	case consExpr : return evaluateCons();
	break;
	case addExpr : return evaluateAdd();
	break;
	case subExpr : return evaluateSub();
	break;
	case mulExpr : return evaluateMul();
	break;
	case divExpr : return evaluateDiv();
	break;
	case modExpr : return evaluateMod();
	break;
	case ltExpr : return evaluateLT();
	break;
	case gtExpr : return evaluateGT();
	break;
	case eqExpr : 	return evaluateEQ();
	break;
	default : return resultValue(errorType, (void*) NULL);
	}
	return resultValue(errorType, (void*) NULL);
}

resultValue BinaryPrimExprNode::evaluateCons()
{

	++conscount;
	clock_t gcstart, gcend;
	if (gc_status != gc_disable && current_heap() == 0)//TODO:If I keep this as 5. It gives a segmentation fault. Investigate why?
	{
		conscount = 0;
		++gcinvoke;
		if(gc_status == gc_plain || gc_status == gc_freq)
		{
			//cout << "Invoking reachability based GC"<<std::endl;
			gcstart = clock();
			reachability_gc();
			gcend = clock();
			//cout << "Completed GC"<<endl;
		}
		else if (gc_status == gc_live )
		{
			//cout << "Invoking liveness based GC"<<endl;
			return_stack().return_point = this->getLabel();
			//get handle to stack and store return point
			gcstart = clock();
			liveness_gc();
			gcend = clock();
			return_stack().return_point = -1;
			//cout << "Completed liveness based GC"<<endl;
			//reset return point on top of stack to -1
		}
		calculate_garbage();
		detail_gc();
		//Update GC time
		gctime += double(gcend-gcstart)/CLOCKS_PER_SEC;
		//cout << "GC end time="<<gcend<<"GC begin time=">>gcstart<<endl;
		gc_times.push_back(double(gcend-gcstart)/CLOCKS_PER_SEC);
		dump_garbage_stats();
	}
	else
	{
		if (gc_status == gc_freq && conscount >= maxcount)
		{
			conscount = 0;
			++gcinvoke;
			gcstart = clock();
			reachability_gc();
			gcend = clock();
			calculate_garbage();
			detail_gc();
			//Update GC time
			gctime += double(gcend-gcstart)/CLOCKS_PER_SEC;
			//cout << "GC end time="<<gcend<<"GC begin time=">>gcstart<<endl;
			gc_times.push_back(double(gcend-gcstart)/CLOCKS_PER_SEC);
			dump_garbage_stats();
		}
	}

	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();
	char arg1type;
	char arg2type;
	void* a1;
	void* a2;
	switch(arg1.type)
	{
	case intType : arg1type = 'i';
	a1 = int_to_void(arg1.val.intVal);
	break;
	case boolType : arg1type = 'b';
	a1 = int_to_void(arg1.val.boolVal);
	break;
	case stringType : arg1type = 's';
	a1 = (void*)arg1.val.stringVal;
	break;
	default : arg1type = 'a';
	a1 = arg1.val.addrVal;
	break;
	}

	switch(arg2.type)
	{
	case intType : arg2type = 'i';
	a2 = int_to_void(arg2.val.intVal);
	break;
	case boolType : arg2type = 'b';
	a2 = int_to_void(arg2.val.boolVal);
	break;
	case stringType : arg2type = 's';
	a2 = (void*)arg2.val.stringVal;
	break;
	default : arg2type = 'a';
	a2 = arg2.val.addrVal;
	break;
	}

	return resultValue(heap, allocate_cons(arg1type, a1, arg2type, a2));
}

resultValue BinaryPrimExprNode::evaluateAdd()
{
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);
	return resultValue(intType, arg1.val.intVal + arg2.val.intVal);
}

resultValue BinaryPrimExprNode::evaluateSub()
{
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);

	return resultValue(intType, arg1.val.intVal - arg2.val.intVal);
}

resultValue BinaryPrimExprNode::evaluateMul()
{
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);
	return resultValue(intType, arg1.val.intVal * arg2.val.intVal);
}

resultValue BinaryPrimExprNode::evaluateDiv()
{
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);
	return resultValue(intType, int(arg1.val.intVal / arg2.val.intVal));
}

resultValue BinaryPrimExprNode::evaluateMod()
{
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);
	return resultValue(intType, arg1.val.intVal % arg2.val.intVal);
}

resultValue BinaryPrimExprNode::evaluateLT()
{
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);
	return resultValue(boolType, arg1.val.intVal < arg2.val.intVal);
}

resultValue BinaryPrimExprNode::evaluateGT()
{
	//cout << "Evaluating greater than" << endl;
	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	assert(arg1.type == intType && arg2.type == intType);
	return resultValue(boolType, arg1.val.intVal > arg2.val.intVal);
}

resultValue BinaryPrimExprNode::evaluateEQ()
{

	resultValue arg1 = this->pArg1->evaluate();
	resultValue arg2 = this->pArg2->evaluate();

	//assert(arg1.type == arg2.type);
	if(arg1.type != arg2.type)
		return resultValue(boolType, (bool)0);

	if (arg1.type == stringType)
		return resultValue(boolType, (arg1.val.stringVal == arg2.val.stringVal) ||
				0 == strcmp(arg1.val.stringVal->c_str(), arg2.val.stringVal->c_str()));
	else if (arg1.type == intType)
		return resultValue(boolType, arg1.val.intVal == arg2.val.intVal);
	else if (arg1.type == boolType)
		return resultValue(boolType, arg1.val.boolVal == arg2.val.boolVal);
	else if(arg1.type == heap)
		return resultValue(boolType, arg1.val.addrVal == arg2.val.addrVal);
	else
		cout << "Error type. Should not have hit this point" << endl;

	return resultValue(errorType, (void*)0);
}

// LivenessInfo BinaryPrimExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second = this->pArg2->analyse(lin);
// 	l.second = this->pArg1->analyse(l);

// 	return l.second;
// }

/*  Definitions for FUNCTION CALL node */

FuncExprNode::~FuncExprNode() {
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



resultValue FuncExprNode::evaluate()
{

	DefineNode* funcDef = (DefineNode*)pgm->getFunction(this->getFunction());
	std::vector<resultValue> arg_locs;
	//cout << "Calling function " << funcDef->getFuncName() << endl;

	int j = 0;
	for(std::list<ExprNode*>::iterator arg = this->pListArgs->begin(); arg != this->pListArgs->end(); ++arg)
	{
		resultValue r = (*arg)->evaluate();
		arg_locs.push_back(r);
	}

	make_environment(funcDef->getFuncName().c_str(), this->getNextExpr());

	int i = 0;
	for(std::vector<resultValue>::iterator arg = arg_locs.begin(); arg != arg_locs.end(); ++arg, ++i)
	{

		switch(arg->type)
		{
		case intType : make_reference_value((funcDef->getArgs()[i]).c_str(), int_to_void(arg->val.intVal), 'i');
		break;
		case boolType : make_reference_value((funcDef->getArgs()[i]).c_str(), int_to_void(arg->val.boolVal), 'b');
		break;
		case stringType : make_reference_value((funcDef->getArgs()[i]).c_str(), char_to_void(arg->val.stringVal), 's');
		break;
		default : make_reference_addr((funcDef->getArgs()[i]).c_str(), arg->val.addrVal);
		break;
		}
	}

	resultValue res = funcDef->getFunctionBody()->evaluate();

	delete_environment();
	return res;
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

// LivenessInfo FuncExprNode::analyse(Liveness lin)
// {
// 	l.first = getLabel();
// 	l.second = lin.second;
// 	for(std::list<ExprNode*>::iterator arg = this->pListArgs->begin(); arg != this->pListArgs->end(); ++arg)
// 	{
// 		l.second = (*arg)->analyse(l);
// 	}
// 	return l.second;
// }



/*  Definitions for DEFINE node */

DefineNode::~DefineNode() {
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

std::string DefineNode::getFuncName() const {
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

resultValue ProgramNode::evaluate()
{
	//std::cout << "Evaluating main expression " << std::endl;
	return this->pExpr->evaluate();
}

Node* ProgramNode::getFunction(std::string funcName)
{
	//cout << "Looking for function " << funcName << endl;
	return this->funcmap[funcName];
}

void ProgramNode::doLivenessAnalysis()
{
}



