#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include <locale>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/algorithm/string.hpp>
#include<boost/tokenizer.hpp>

//#include <boost/filesystem.hpp>

#include "DemandStructure.h"

using namespace Scheme::Demands;

// LivenessState Constructors
Scheme::Demands::LivenessState::LivenessState(const std::string& s, const int& n): idString(s), latticeNo(n) {}
Scheme::Demands::LivenessState::LivenessState(): latticeNo(0) {idString = "";}

// Possible Liveness
LivenessState Scheme::Demands::PHI = LivenessState("phi", 0);
LivenessState Scheme::Demands::EPSILON = LivenessState("eps", 1);
LivenessState Scheme::Demands::ZERO = LivenessState("zero", 2);
LivenessState Scheme::Demands::ONE = LivenessState("one", 3);
LivenessState Scheme::Demands::ZEROALL = LivenessState("zeroall", 4);
LivenessState Scheme::Demands::ONEALL = LivenessState("oneall", 5);
LivenessState Scheme::Demands::ALL = LivenessState("all", 6);

bool Scheme::Demands::operator<(const LivenessState& s1, const LivenessState& s2)
{
  return s1.latticeNo < s2.latticeNo;
}

std::unordered_map<std::string, std::unordered_map<std::string, LivenessState> > getUnionRules()
{
    std::unordered_map<std::string, std::unordered_map<std::string, LivenessState> > m {{"phi", {{"phi", PHI}, 
                                                                                                  {"eps", EPSILON},
                                                                                                  {"zero", ZERO},
                                                                                                  {"one", ONE},
                                                                                                  {"zeroall", ZEROALL},
                                                                                                  {"oneall", ONEALL},
                                                                                                  {"all", ALL}}},
                                                                                         {"eps", {{"phi", EPSILON}, 
                                                                                                  {"eps", EPSILON},
                                                                                                  {"zero", ZERO},
                                                                                                  {"one", ONE},
                                                                                                  {"zeroall", ZEROALL},
                                                                                                  {"oneall", ONEALL},
                                                                                                  {"all", ALL}}},
                                                                                        {"zero", {{"phi", ZERO}, 
                                                                                                  {"eps", ZERO},
                                                                                                  {"zero", ZERO},
                                                                                                  {"one", ALL},
                                                                                                  {"zeroall", ZEROALL},
                                                                                                  {"oneall", ALL},
                                                                                                  {"all", ALL}}},
                                                                                         {"one", {{"phi", ONE}, 
                                                                                                  {"eps", ONE},
                                                                                                  {"zero", ALL},
                                                                                                  {"one", ONE},
                                                                                                  {"zeroall", ALL},
                                                                                                  {"oneall", ONEALL},
                                                                                                  {"all", ALL}}},
                                                                                     {"zeroall", {{"phi", ZEROALL}, 
                                                                                                  {"eps", ZEROALL},
                                                                                                  {"zero", ZEROALL},
                                                                                                  {"one", ALL},
                                                                                                  {"zeroall", ZEROALL},
                                                                                                  {"oneall", ALL},
                                                                                                  {"all", ALL}}},
                                                                                      {"oneall", {{"phi", ONEALL}, 
                                                                                                  {"eps", ONEALL},
                                                                                                  {"zero", ALL},
                                                                                                  {"one", ONEALL},
                                                                                                  {"zeroall", ALL},
                                                                                                  {"oneall", ONEALL},
                                                                                                  {"all", ALL}}},
                                                                                         {"all", {{"phi", ALL}, 
                                                                                                  {"eps", ALL},
                                                                                                  {"zero", ALL},
                                                                                                  {"one", ALL},
                                                                                                  {"zeroall", ALL},
                                                                                                  {"oneall", ALL},
                                                                                                  {"all", ALL}}}};
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
    std::unordered_map<std::string, LivenessState> temp = { {"phi", EPSILON},
                                                            {"eps", ZERO},
                                                            {"zero", ZEROALL},
                                                            {"one", ZEROALL},
                                                            {"zeroall", ZEROALL},
                                                            {"oneall", ZEROALL},
                                                            {"all", ZEROALL} };
    return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::catZeroRules = getCatZeroRules();

std::unordered_map<std::string, LivenessState> getCatOneRules()
{
    std::unordered_map<std::string, LivenessState> temp = { {"phi", EPSILON},
                                                            {"eps", ONE},
                                                            {"zero", ONEALL},
                                                            {"one", ONEALL},
                                                            {"zeroall", ONEALL},
                                                            {"oneall", ONEALL},
                                                            {"all", ONEALL} };
    return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::catOneRules = getCatOneRules();

std::unordered_map<std::string, LivenessState> getStripZeroRules()
{
  std::unordered_map<std::string, LivenessState> temp = { {"phi", PHI},
                                                          {"eps", PHI},
                                                          {"zero", EPSILON},
                                                          {"one", PHI},
                                                          {"zeroall", ALL},
                                                          {"oneall", PHI},
                                                          {"all", ALL} };

  return temp;
}
std::unordered_map<std::string, LivenessState> LivenessState::stripZeroRules = getStripZeroRules();

std::unordered_map<std::string, LivenessState> getStripOneRules()
{
  std::unordered_map<std::string, LivenessState> temp = { {"phi", PHI},
                                                          {"eps", PHI},
                                                          {"zero", PHI},
                                                          {"one", EPSILON},
                                                          {"zeroall", PHI},
                                                          {"oneall", ALL},
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
    table[ZEROALL] = PHI;
    table[ONEALL] = PHI;
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
    table[ZEROALL] = ZEROALL;
    table[ONEALL] = ONEALL;
    table[ALL] = ALL;
  }
}



void LivenessTable::catZero()
{
  for (auto i = table.begin(); i != table.end(); i++)
    {
      i->second = i->second.catZero();
    }
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
    {
      i->second = i->second + state;
    }
  return answer;
}

void LivenessTable::doUnion(const LivenessTable& t)
{
  if(varName != t.varName)
  {
    std::cerr<<"Taking union of different variables\n";
    std::cout<<varName<<' '<<t.varName<<std::endl;
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
  out<<"| "<<t.varName;
  if (t.varName.size() < 7)
    for (int i = 0; i < 7 - t.varName.size(); ++i)
    {
      out<<" ";
    }
  out<<"\t|";

  for (auto& i : t.table)
  {
    out<<" "<<i.second<<"\t|";
  }
  return out;
}

std::ostream& Scheme::Demands::operator<<(std::ostream& out, const LivenessInformation& l)
{
  if (l.empty())
  {
    return out;
  }
  out<<"| VarName\t|";
  auto iter = l.begin();

  for (const auto& i : (iter->second).table)
  {
    out<<" "<<boost::to_upper_copy<std::string>(i.first.idString)<<"\t|";
  }
  out<<'\n';
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

bool Scheme::Demands::printLivenessToFile(const ProgramLiveness& l, const std::string& path)
{
  std::ofstream of;
  of.open(path);
  of<<l;
  of.close();
  return true;
}

void Scheme::Demands::doUnion(LivenessInformation& l1, const LivenessInformation& l2)
{
  for(auto i=l1.begin();i != l1.end(); i++)
  {
    auto temp  = l2.find(i->first);
    if(temp != l2.end())
    {
      // std::cout<<i->first<<' '<<i->second.varName<<std::endl;
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

bool Scheme::Demands::operator==(const LivenessState& l1, const LivenessState& l2)
{
  return l1.idString == l2.idString;
}

bool Scheme::Demands::operator!=(const LivenessState& l1, const LivenessState& l2)
{
  return l1.idString != l2.idString; 
}

bool Scheme::Demands::operator==(const LivenessTable& t1, const LivenessTable& t2)
{
  auto l1 = t1.table;
  auto l2 = t2.table;
  if (l1.size() != l2.size())
    return false;

  auto it1 = l1.begin();
  auto it2 = l2.begin();

  while (it1 != l1.end() && it2 != l2.end())
  {
    if (it1->first != it2->first)
      return false;
    else if(it1->second != it2->second)
      return false;
    it1++;
    it2++;
  }

  return true;  
}

bool Scheme::Demands::operator!=(const LivenessTable& l1, const LivenessTable& l2)
{
  return !(l1 == l2);
}

bool Scheme::Demands::operator==(const LivenessInformation& l1, const LivenessInformation& l2)
{
  if (l1.size() != l2.size())
    return false;

  auto it1 = l1.begin();
  auto it2 = l2.begin();

  while (it1 != l1.end() && it2 != l2.end())
  {
    if (it1->first != it2->first)
      return false;
    else if(!(it1->second == it2->second))
      return false;
    it1++;
    it2++;
  }

  return true;  
}


//Unit Tests

// int main()
// {
//  std::cout<<"Testing!\n";
//  std::cout<<PHI + ALL<<'\n';
//  std::cout<<ONE_STAR<<'\n';
//  LivenessInformation l1, l2;
//  doUnion(l1, l2);
// }

