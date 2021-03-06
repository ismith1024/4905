#ifndef UTILITYALGORITHMS_H
#define UTILITYALGORITHMS_H



#include <string>
#include "component.h"
#include<iostream>
#include <map>
#include<algorithm>

using namespace std;

class UtilityAlgorithms{
public:
    static int lcs(string, string);
    static int levDist(string, string);
    static int lcs(Component&, Component&);
    static int levDist(Component&, Component&);
    static pair<string,float> argmax(map<string,float>*);
    static pair<int,float> argmax(map<int,float>*);

    static bool containsSubst(string&, string&);
    static bool endsWith(string&, string&);

    static bool mapContainsKey(map<string, int>&, string&);
    static bool mapContainsKey(map<string, string>&, string&);
    static bool mapContainsKey(map<string, float>&, string&);
    static bool isAlphanumeric(string&);
    static bool isAncestor(Component*, Component*);

};


/*
TBD:
https://en.wikipedia.org/wiki/Hierarchical_clustering

*/
#endif // UTILITYALGORITHMS_H
