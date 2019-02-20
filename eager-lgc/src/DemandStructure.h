#ifndef __DEMANDSTRUCTURE_H__
#define __DEMANDSTRUCTURE_H__ 1
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <functional>

#include <boost/unordered_set.hpp>

namespace Scheme {

namespace Demands {

/*
	Defines a type for possible liveness values
	Functions:
		operator+(): union of liveness
*/

class LivenessState
{

public:
	std::string idString;
	static std::unordered_map<std::string, std::unordered_map<std::string, LivenessState> > unionRules;
	static std::unordered_map<std::string, LivenessState> catZeroRules;
	static std::unordered_map<std::string, LivenessState> catOneRules;
	static std::unordered_map<std::string, LivenessState> stripZeroRules;
	static std::unordered_map<std::string, LivenessState> stripOneRules;

	LivenessState();
	LivenessState(const std::string& s);
	LivenessState operator+(const LivenessState&);	//Union
	LivenessState catZero();						//Concatenate Zero
	LivenessState catOne();							//Concatenate One
	LivenessState stripZero();						//Strip's Zero from front
	LivenessState stripOne();						//Strip's One from front
	friend std::ostream& operator<<(std::ostream& out, const LivenessState& l);

};

extern const LivenessState PHI;
extern const LivenessState EPSILON;
extern const LivenessState ZERO;
extern const LivenessState ONE;
extern const LivenessState ONE_STAR;
extern const LivenessState ALL;

//Operator overloading
bool operator==(const LivenessState l1, const LivenessState l2);

//Trivial hasher for maps/sets of LivenessState
class LivenessStateHasher
{
public:
	std::size_t operator()(const LivenessState& obj) const { return std::hash<std::string>()(obj.idString); }
};

class LivenessTable;
//Defines a type for liveness table of any variable
typedef std::map< std::string, LivenessTable> LivenessInformation;
LivenessInformation mapLiveness(const LivenessTable&, LivenessInformation&);
class LivenessTable
{
public:
	std::unordered_map<LivenessState, LivenessState, LivenessStateHasher> table;
	std::string varName;
	LivenessTable();
	LivenessTable(std::string, bool self);
	LivenessTable operator+(const LivenessState&);
	LivenessState& operator[] (const LivenessState k);
	void catZero();
	void catOne();
	void stripZero();
	void stripOne();
	void doUnion(const LivenessTable&);


	friend std::ostream& operator<<(std::ostream& out, const LivenessTable& t);
	friend LivenessInformation mapLiveness(const LivenessTable& lt, const LivenessInformation& li);

};


typedef std::unordered_map< std::string, LivenessInformation> ProgramLiveness;

//Printing Demands
std::ostream& operator<<(std::ostream& out, const LivenessState& l);
std::ostream& operator<<(std::ostream& out, const LivenessTable& t);
std::ostream& operator<<(std::ostream& out, const LivenessInformation& t);

void doUnion(LivenessInformation &, const LivenessInformation &);
}
}
extern Scheme::Demands::ProgramLiveness progLiveness;


#endif

