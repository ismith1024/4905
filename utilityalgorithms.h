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
    static int longestCommonSS(string, string);
    static int levDist(string, string);
    static int longestCommonSS(Component&, Component&);
    static int levDist(Component&, Component&);
    static pair<string,float> argmax(map<string,float>*);
    static pair<int,float> argmax(map<int,float>*);

};


/*
TBD:
https://en.wikipedia.org/wiki/Hierarchical_clustering

*/
#endif // UTILITYALGORITHMS_H
