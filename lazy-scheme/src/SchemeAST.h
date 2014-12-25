#ifndef __SCHEME_AST_H__
#define __SCHEME_AST_H__ 1

#include "Output.h"
#include <vector>
#include<map>
#include<iostream>
#include <boost/variant.hpp>
#include "Demands.h"
#include <functional>
#include <cstring>
#include<utility>
#include<set>

using namespace std;

namespace Scheme {

namespace AST {

class ExprNode;

typedef int state_index;
typedef std::set<state_index> stateset;

enum cell_type
{
	nilExprClosure,
	constIntExprClosure,
	constBoolExprClosure,
	constStringExprClosure,
	funcApplicationExprClosure,
	consExprClosure,
	unaryprimopExprClosure,
	binaryprimopExprClosure,
	funcArgClosure,
	defaultClosure
};
typedef unsigned long clock_tick;

struct cons
{
	cell_type typecell;
	union
	{
		struct
		{
			cons *car, *cdr;
		}cell;
		int intVal;
		bool boolval;
		std::string* stringVal;
		struct
		{
			ExprNode* expr;
			cons* arg1;
			cons* arg2;
		}closure;
	}val;
	void *forward;
	stateset *setofStates;
	int depth;
	bool inWHNF;
	unsigned int closure_id;
	unsigned int reduction_id;
#ifdef GC_ENABLE_STATS
	/*----------------------------------------------------------------------
	 * Following fields are added by Amey Karkare to
	 * generate gc related statistics
	 */
	/* size is used only if we support vectors */
	clock_tick created;       /* creation time of cell */
	clock_tick first_use;     /* first use time of cell */
	clock_tick last_use;      /* last use time of cell */
	char       is_reachable:1,  /* true if cell is reachable during current gc */
	is_used:1;       /* true if cell is dereferenced for use */
	/*----------------------------------------------------------------------*/
#endif
#ifdef ENABLE_SHARING_STATS
	int visited;
#endif


};

enum exprType
{
	nilExpr,
	constIntExpr,
	constBoolExpr,
	constStringExpr,
	varExpr,
	letExpr,
	funcDefine,
	funcApplicationExpr,
	consExpr,
	carExpr,
	cdrExpr,
	nullqExpr,
	pairqExpr,
	addExpr,
	subExpr,
	mulExpr,
	divExpr,
	gtExpr,
	ltExpr,
	modExpr,
	eqExpr,
	ifExpr,
	defaultType
};



class Node {
public:
	virtual ~Node();

	virtual void doLabel(bool = true) = 0;
	virtual std::string getLabel() const;

	virtual Node * clone() const = 0;
	virtual Node * getANF() const = 0;


	virtual Scheme::Demands::expr_demand_grammars *
	transformDemandRef(const Scheme::Demands::rule&) const
	{
		Scheme::Demands::expr_demand_grammars *r;
		std::cout << "Error : should not be called" << std::endl;
		return r;
	}
	virtual std::unordered_map<string, Scheme::Demands::expr_demand_grammars*>
	transformDemand(const Scheme::Demands::rule&) const
	{
		std::unordered_map<string, Scheme::Demands::expr_demand_grammars*> r;
		std::cout << "Error : should not be called" << std::endl;
		return r;
	}

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const = 0;
	enum exprType type;
	const std::string node_name;
	std::unordered_set<std::string> label_set;
protected:
	std::string label;



	Node(const std::string);

};




class ExprNode : public Node {
public:
	virtual ~ExprNode();

	/* We need to re-write this to tell the compiler that,
	 * the clone() and getANF() of any derived classes of
	 * ExprNode, is Exprnode * and not Node *.                */
	virtual ExprNode * clone() const = 0;
	virtual ExprNode * getANF() const = 0;
	virtual cons* evaluate(struct cons* heap_cell = NULL) = 0;
	virtual bool isFunctionCallExpression()	{return false;}
	virtual bool isConsExpression() {return false;}
	virtual cons* make_closure() = 0;
	virtual bool isExpressionRecursive(const std::string) const {return false;}
	//cons* heap_ptr;
protected:
	ExprNode(const std::string name);


};


class IdExprNode;
class ReturnExprNode : public ExprNode {
public:
	virtual ~ReturnExprNode();
	ReturnExprNode(IdExprNode *);

	virtual void doLabel(bool = true);

	virtual ReturnExprNode * clone() const;
	virtual ReturnExprNode * getANF() const;

	virtual std::unordered_map<string, Scheme::Demands::expr_demand_grammars*>
	transformDemand(const Scheme::Demands::rule&) const;
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;

	virtual std::string getLabel() const;
	virtual cons* make_closure();

protected:
	IdExprNode * pID;

	friend ExprNode * pushDown(ExprNode *, ExprNode *);
};

class IdExprNode : public ExprNode {
public:
	virtual ~IdExprNode();
	IdExprNode(std::string *);

	virtual void doLabel(bool = true);

	virtual IdExprNode * clone() const;
	virtual ExprNode * getANF() const;

	std::string getIDStr() const;

	virtual Scheme::Demands::expr_demand_grammars *
	transformDemandRef(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	std::string getName() {return *pID;}
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();
	virtual std::string getLabel() const;
	//cons *closure;
protected:
	std::string * pID;

};



class LetExprNode : public ExprNode {
public:
	virtual ~LetExprNode();
	LetExprNode(IdExprNode *, ExprNode *, ExprNode *);

	virtual void doLabel(bool = true);

	virtual LetExprNode * clone() const;
	virtual ExprNode * getANF() const;

	virtual LetExprNode * fillHoleWith(ExprNode * pSubExpr);

	virtual std::unordered_map<string, Scheme::Demands::expr_demand_grammars*>
	transformDemand(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	std::string getVar();
	ExprNode* getVarExpr();
	ExprNode* getBody();
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();


protected:
	IdExprNode * pID;
	ExprNode * pExpr, * pBody;
	friend ExprNode * pushDown(ExprNode *, ExprNode *);
};


class IfExprNode : public ExprNode {
public:
	virtual ~IfExprNode();
	IfExprNode(ExprNode *, ExprNode *, ExprNode *);

	virtual void doLabel(bool = true);

	virtual IfExprNode * clone() const;
	virtual ExprNode * getANF() const;

	virtual IfExprNode * fillHoleWith(IdExprNode * pSubExpr);
	//
	virtual std::unordered_map<string, Scheme::Demands::expr_demand_grammars*>
	transformDemand(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	ExprNode* getCond();
	ExprNode* getThenPart();
	ExprNode* getElsePart();
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();



protected:
	ExprNode * pCond, * pThen, * pElse;
	//cons *condExprClosure, *thenExprClosure, *elseExprClosure;

	friend ExprNode * pushDown(ExprNode *, ExprNode *);
};


class ConstExprNode : public ExprNode {
public:
	virtual ~ConstExprNode();
	virtual LetExprNode * getANF() const;

	virtual void doLabel(bool = true);

	virtual Scheme::Demands::expr_demand_grammars *
	transformDemandRef(const Scheme::Demands::rule&) const;
protected:
	ConstExprNode(const std::string);
};


class NilConstExprNode : public ConstExprNode {
public:
	NilConstExprNode();

	virtual NilConstExprNode * clone() const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();
};


class IntConstExprNode : public ConstExprNode {
public:
	virtual ~IntConstExprNode();
	IntConstExprNode(int *);

	virtual IntConstExprNode * clone() const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	int getVal();
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();


protected:
	int * pIntVal;
	//cons *constIntClosure;
};


class StrConstExprNode : public ConstExprNode {
public:
	virtual ~StrConstExprNode();
	StrConstExprNode(std::string *);

	virtual StrConstExprNode * clone() const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	string* getVal();
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();


protected:
	std::string * pStrVal;
	//cons *constStringClosure;
};


class BoolConstExprNode : public ConstExprNode {
public:
	virtual ~BoolConstExprNode();
	BoolConstExprNode(bool *);

	virtual BoolConstExprNode * clone() const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	bool getVal();
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();


protected:
	bool * pBoolVal;
	//cons *constBoolClosure;
};


class PrimExprNode : public ExprNode {
public:
	virtual ~PrimExprNode();
	//virtual void nextExpr()=0;
protected:
	PrimExprNode(const std::string);
};


class UnaryPrimExprNode : public PrimExprNode {
public:
	virtual ~UnaryPrimExprNode();
	UnaryPrimExprNode(const std::string, ExprNode *);

	virtual void doLabel(bool = true);

	virtual UnaryPrimExprNode * clone() const;
	virtual ExprNode * getANF() const;

	virtual UnaryPrimExprNode * fillHoleWith(IdExprNode * pSubExpr);

	virtual Scheme::Demands::expr_demand_grammars *
	transformDemandRef(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();
	virtual bool isExpressionRecursive(const std::string) const;
	//cons *argClosure;


protected:
	ExprNode * pArg;

	cons* evaluateCarExpr(cons* heap_cell = NULL);
	cons* evaluateCdrExpr(cons* heap_cell = NULL);
	cons* evaluateNullqExpr(cons* heap_cell = NULL);
	cons* evaluatePairqExpr(cons* heap_cell = NULL);
	friend class LetExprNode;
	friend ExprNode * pushDown(ExprNode *, ExprNode *);
};


class BinaryPrimExprNode : public PrimExprNode {
public:
	virtual ~BinaryPrimExprNode();
	BinaryPrimExprNode(const std::string, ExprNode *, ExprNode *);

	virtual void doLabel(bool = true);

	virtual BinaryPrimExprNode * clone() const;
	virtual ExprNode * getANF() const;

	virtual BinaryPrimExprNode * fillHoleWith(IdExprNode * pSubExpr);

	virtual Scheme::Demands::expr_demand_grammars *
	transformDemandRef(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual cons* make_closure();

	virtual bool isConsExpression() {return (node_name == "cons");}
	virtual bool isExpressionRecursive(const std::string) const;
	//cons *arg1Closure, *arg2Closure;
protected:
	ExprNode * pArg1, * pArg2;

	cons* evaluateCons(cons* heap_cell = NULL);
	cons* evaluateAdd(cons* heap_cell = NULL);
	cons* evaluateSub(cons* heap_cell = NULL);
	cons* evaluateMul(cons* heap_cell = NULL);
	cons* evaluateDiv(cons* heap_cell = NULL);
	cons* evaluateMod(cons* heap_cell = NULL);
	cons* evaluateLT(cons* heap_cell = NULL);
	cons* evaluateGT(cons* heap_cell = NULL);
	cons* evaluateEQ(cons* heap_cell = NULL);

	friend class LetExprNode;
	friend ExprNode * pushDown(ExprNode *, ExprNode *);
};


class FuncExprNode : public ExprNode {
public:
	virtual ~FuncExprNode();
	FuncExprNode(IdExprNode *, std::list<ExprNode *> *);

	virtual void doLabel(bool = true);

	virtual FuncExprNode * clone() const;
	virtual ExprNode * getANF() const;

	virtual FuncExprNode * fillHoleWith(IdExprNode * pSubExpr);

	virtual Scheme::Demands::expr_demand_grammars *
	transformDemandRef(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	std::string getFunction();
	std::vector<ExprNode*> getArgs();
	virtual cons* evaluate(struct cons* heap_cell = NULL);
	virtual bool isFunctionCallExpression()	{return true;}
	void setNextExpr(std::string);
	std::string getNextExpr();
	virtual cons* make_closure();
	std::string parent_let_pgmpt;
	virtual bool isExpressionRecursive(const std::string) const;
	//std::list<cons*> argsClosureList;
protected:
	IdExprNode * pID;
	std::list<ExprNode *> * pListArgs;
	std::vector<ExprNode*> vargs;
	//std::vector<cons*> argsClosureVector;
	std::string nextExpr;
	ExprNode* fBody;
	friend class LetExprNode;
	friend ExprNode * pushDown(ExprNode *, ExprNode *);
};


class DefineNode : public Node {
public:
	virtual ~DefineNode();
	DefineNode(IdExprNode *, std::list<IdExprNode *> *, ExprNode *);

	virtual void doLabel(bool = true);

	virtual DefineNode * clone() const;
	virtual DefineNode * getANF() const;

	virtual std::string getFuncName() const;
	virtual std::unordered_map<string, Scheme::Demands::expr_demand_grammars*>
	transformDemand(const Scheme::Demands::rule&) const;

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	std::string getFunctionName() {return pID->getIDStr();}
	std::vector<std::string> getArgs();
	ExprNode* getFunctionBody() {return pExpr;}

protected:
	ExprNode * pExpr;
	IdExprNode * pID;
	std::list<IdExprNode *> * pListArgs;
	std::vector<std::string> vargs;
};

class ProgramNode : public Node {
public:
	virtual ~ProgramNode();
	ProgramNode(std::list<DefineNode *> *, ExprNode *);

	virtual void doLabel(bool = true);

	virtual ProgramNode * clone() const;
	virtual ProgramNode * getANF() const;

	virtual std::unordered_map<string, Scheme::Demands::expr_demand_grammars*>
	transformDemand(const Scheme::Demands::rule&);

	virtual std::ostream & print(std::ostream &, unsigned = 0, bool = true, bool = false,
			Scheme::output::output_t format = Scheme::output::PLAIN) const;
	cons* evaluate(cons* heap_cell = NULL);

	Node* getFunction(std::string);

	void doLivenessAnalysis();

	std::unordered_map<string, Node*> *progpt_map;
	Scheme::Demands::demand_grammar liveness_data;

	//TODO : Add a function to process function definitions (Similar to the LE function in python)
protected:
	std::list<DefineNode *> * pListDefines;

	ExprNode * pExpr;
	//cons *mainExprClosure;
	std::map<std::string, Node*> funcmap;
};

ExprNode * pushDown(ExprNode *, ExprNode *);

}

}





#endif
