#ifndef COMPONENT_H
#define COMPONENT_H



#include<string>

using namespace std;

class Component{

public:
    Component(string, string, string, string);
    string mfr;
    string mpn;
    string description;
    string type;
};

#endif // COMPONENT_H
