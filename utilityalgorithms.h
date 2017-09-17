#ifndef UTILITYALGORITHMS_H
#define UTILITYALGORITHMS_H

#endif // UTILITYALGORITHMS_H

#include <string>
#include "component.h"

using namespace std;

class UtilityAlgorithms{
public:
    static int longestCommonSS(string, string);
    static int levDist(string, string);
    static int longestCommonSS(Component&, Component&);
    static int levDist(Component&, Component&);

};


/*
TBD:
https://en.wikipedia.org/wiki/Hierarchical_clustering

*/
