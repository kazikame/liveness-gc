#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include<boost/tokenizer.hpp>
//#include <boost/filesystem.hpp>

#include "DemandStructure.h"

using namespace Scheme::Demands;

// LivenessState Constructors
Scheme::Demands::LivenessState::LivenessState(const std::string& s, const int& n): idString(s), latticeNo(n) {}
Scheme::Demands::LivenessState::LivenessState(): latticeNo(0) {idString = "";}

// Possible Liveness
LivenessState Scheme::Demands::PHI = LivenessState("phi", 0);
LivenessState Scheme::Demands::EPSILON = LivenessState("e", 1);
LivenessState Scheme::Demands::ZERO = LivenessState("0", 2);
LivenessState Scheme::Demands::ONE = LivenessState("1", 3);
LivenessState Scheme::Demands::ONE_STAR = LivenessState("1*", 4);
LivenessState Scheme::Demands::ALL = LivenessState("all", 5);

bool Scheme::Demands::operator<(const LivenessState& s1, const LivenessState& s2)
{
  return s1.latticeNo < s2.latticeNo;
}

//Can parse from text file
std::unordered_map<std::string, std::unordered_map<std::string, LivenessState> > getUnionRules()
{
    std::unordered_map<std::string, std::unordered_map<std::string, LivenessState> > m { {"phi", { {"phi", PHI}, 
                                                                                                                      {"e", EPSILON},
                                                                                                                      {"0", ZERO},
                                                                                                                      {"1", ONE},
                                                                                                                      {"1*", ONE_STAR},
                                                                                                                      {"all", ALL} }},
                                                                                                             {"e", { {"phi", EPSILON}, 
                                                                                                                      {"e", EPSILON},
                                                                                                                      {"0", ZERO},
                                                                                                                      {"1", ONE},
                                                                                                                      {"1*", ONE_STAR},
                                                                                                                      {"all", ALL} }},
                                                                                                             {"0", { {"phi", ZERO}, 
                                                                                                                      {"e", ZERO},
                                                                                                                      {"0", ZERO},
                                                                                                                      {"1", ALL},
                                                                                                                      {"1*", ALL},
                                                                                                                      {"all", ALL} }},
                                                                                                             {"1", { {"phi", ONE}, 
                                                                                                                      {"e", ONE},
                                                                                                                      {"0", ALL},
                                                                                                                      {"1", ONE},
                                                                                                                      {"1*", ONE_STAR},
                                                                                                                      {"all", ALL} }},
                                                                                                             {"1*", { {"phi", PHI}, 
                                                                                                                      {"e", EPSILON},
                                                                                                                      {"0", ZERO},
                                                                                                                      {"1", ONE},
                                                                                                                      {"1*", ONE_STAR},
                                                                                                                      {"all", ALL} }},
                                                                                                             {"all", { {"phi", ALL}, 
                                                                                                                      {"e", ALL},
                                                                                                                      {"0", ALL},
                                                                                                                      {"1", ALL},
                                                                                                                      {"1*", ALL},
                                                                                                                      {"all", ALL} }} };
    return m;
}
std::unordered_map<std::string, std::unordered_map<std::string, LivenessState> > LivenessState::unionRules = getUnionRules();


//Operator overloading
bool Scheme::Demands::operator==(const LivenessState l1, const LivenessState l2)
{
	return (l1.idString == l2.idString);
}
LivenessState LivenessState::operator+(const LivenessState& other)
{
   return unionRules[idString][other.idString];
}

std::unordered_map<std::string, LivenessState> getCatZeroRules()
{
    std::unordered_map<std::string, LivenessState> temp = { {"phi", PHI},
                                                                                      {"e", ZERO},
                                                                                      {"0", ALL},
                                                                                      {"1", ALL},
                                                                                      {"1*", ALL},
                                                                                      {"all", ALL} };
    return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::catZeroRules = getCatZeroRules();

std::unordered_map<std::string, LivenessState> getCatOneRules()
{
    std::unordered_map<std::string, LivenessState> temp = { {"phi", PHI},
                                                                                      {"e", ONE},
                                                                                      {"0", ALL},
                                                                                      {"1", ONE_STAR},
                                                                                      {"1*", ONE_STAR},
                                                                                      {"all", ALL} };
    return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::catOneRules = getCatOneRules();

std::unordered_map<std::string, LivenessState> getStripZeroRules()
{
  std::unordered_map<std::string, LivenessState> temp = { {"phi", PHI},
                                                                                      {"e", PHI},
                                                                                      {"0", EPSILON},
                                                                                      {"1", PHI},
                                                                                      {"1*", PHI},
                                                                                      {"all", ALL} };

  return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::stripZeroRules = getStripZeroRules();

std::unordered_map<std::string, LivenessState> getStripOneRules()
{
  std::unordered_map<std::string, LivenessState> temp = { {"phi", PHI},
                                                                                      {"e", PHI},
                                                                                      {"0", PHI},
                                                                                      {"1", EPSILON},
                                                                                      {"1*", ONE_STAR},
                                                                                      {"all", ALL} };

  return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::stripOneRules = getStripOneRules();

LivenessState LivenessState::catZero()
{
    return catZeroRules[idString];
}


LivenessState LivenessState::catOne()
{
    return catOneRules[idString];
}

LivenessState LivenessState::stripZero()
{
  return stripZeroRules[idString];
}
LivenessState LivenessState::stripOne()
{
  return stripOneRules[idString];
}
//LivenessTable operator overloading
LivenessState& LivenessTable::operator[] (LivenessState k) { return this->table[k];}
const LivenessState& LivenessTable::operator[] (const LivenessState& k) const { return (table.find(k))->second;}
// LivenessTable Constructor
LivenessTable::LivenessTable()
{
    table[PHI] = PHI;
    table[EPSILON] = PHI;
    table[ZERO] = PHI;
    table[ONE] = PHI;
    table[ONE_STAR] = PHI;
    table[ALL] = PHI;
}

LivenessTable::LivenessTable(std::string name, bool self = false): LivenessTable()
{
	varName = name;

  if (self)
  {
    table[PHI] = PHI;
    table[EPSILON] = EPSILON;
    table[ZERO] = ZERO;
    table[ONE] = ONE;
    table[ONE_STAR] = ONE_STAR;
    table[ALL] = ALL;
  }
}



void LivenessTable::catZero()
{
  for (auto i = table.begin(); i != table.end(); i++)
    i->second = i->second.catZero();
}
void LivenessTable::catOne()
{
  for (auto i = table.begin(); i != table.end(); i++)
    i->second = i->second.catOne();
}
void LivenessTable::stripZero()
{
  for (auto i = table.begin(); i != table.end(); i++)
    i->second = i->second.stripZero();
}
void LivenessTable::stripOne()
{
  for (auto i = table.begin(); i != table.end(); i++)
    i->second = i->second.stripOne();
}

LivenessTable LivenessTable::operator+(const LivenessState& state)
{
  LivenessTable answer = *this;
  for (auto i = answer.table.begin(); i != answer.table.end(); i++)
    i->second = i->second + state;
  return answer;
}

void LivenessTable::doUnion(const LivenessTable& t)
{
  if(varName != t.varName)
  {
    std::cerr<<"Taking union of different variables";
    exit(-1);
  }
  for (auto i = table.begin(); i != table.end(); i++) 
   {
    i->second = i->second + t[i->first];
   } 
  
}



//Printing and Debugging

// Printing demands
std::ostream& Scheme::Demands::operator<<(std::ostream& out, const LivenessState& l)
{
	out<<l.idString;
	return out;
}

std::ostream& Scheme::Demands::operator<<(std::ostream& out, const LivenessTable& t)
{
	out<<t.varName<<"\t\t";
	out<<t[PHI]<<'\t';
	out<<t[EPSILON]<<'\t';
	out<<t[ZERO]<<'\t';
	out<<t[ONE]<<'\t';
	out<<t[ONE_STAR]<<"\t\t";
	out<<t[ALL]<<'\t';
	return out;
}

std::ostream& Scheme::Demands::operator<<(std::ostream& out, const LivenessInformation& l)
{
	out<<"Variable Name\t"<<"PHI\t"<<"EPSILON\t"<<"ZERO\t"<<"ONE\t"<<"ONE_STAR\t"<<"ALL\n";
	for (auto i: l)
	{
		out<<i.second<<'\n';
	}
	out<<'\n';
	return out;
}

std::ostream& Scheme::Demands::operator<<(std::ostream& out, const ProgramLiveness& l)
{
	for (auto i: l)
	{
		out<<"For program point: "<<i.first<<'\n';
		out<<i.second;
	}

  return out;
}


void Scheme::Demands::doUnion(LivenessInformation& l1,const LivenessInformation& l2)
{
  for(auto i=l1.begin();i != l1.end(); i++)
  {
    auto temp  = l2.find(i->first);
    if(temp != l2.end())
    {
      i->second.doUnion(temp->second);
    }
  }

  for(auto i=l2.begin();i != l2.end(); i++)
  {
    auto temp  = l1.find(i->first);
    if(temp == l1.end())
    {
      l1.insert(*i);
    }
  }

}

LivenessInformation Scheme::Demands::mapLiveness(const LivenessTable& lt, const LivenessInformation& li)
{
  LivenessInformation returnValue = li;
  for(auto i=li.begin(); i!= li.end(); i++)
  {
    LivenessTable temp = LivenessTable(i->first);
     for(auto j=lt.table.begin();j != lt.table.end(); j++)
     {
       temp[j->first] = (i->second)[(j->second)];
     }

     returnValue[i->first] = temp;
  }

  return returnValue;
}

//Unit Tests

// int main()
// {
// 	std::cout<<"Testing!\n";
// 	std::cout<<PHI + ALL<<'\n';
// 	std::cout<<ONE_STAR<<'\n';
// 	LivenessInformation l1, l2;
// 	doUnion(l1, l2);
// }