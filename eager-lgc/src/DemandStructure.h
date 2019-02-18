#ifndef __DEMANDSTRUCTURE_H__
#define __DEMANDSTRUCTURE_H__ 1
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
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
	LivenessState();
	LivenessState(const std::string& s);
	LivenessState operator+(const LivenessState&);	//Union
	LivenessState catZero();						//Concatenate Zero
	LivenessState catOne();							//Concatenate One
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


//Defines a type for liveness table of any variable
class LivenessTable
{
private:
	std::unordered_map<LivenessState, LivenessState, LivenessStateHasher> table;
public:
	std::string varName;
	LivenessTable();
	LivenessTable(std::string);
	LivenessState& operator[] (const LivenessState& k);

	friend std::ostream& operator<<(std::ostream& out, const LivenessTable& t);
};

typedef std::unordered_map< std::string, LivenessTable> LivenessInformation;

//Printing Demands
std::ostream& operator<<(std::ostream& out, const LivenessState& l);
std::ostream& operator<<(std::ostream& out, const LivenessTable& t);
std::ostream& operator<<(std::ostream& out, const LivenessInformation& t);
}
}


#endif

