#ifndef COMPONENT_H
#define COMPONENT_H

#include<string>
#include<iostream>

using namespace std;

class Component{

public:
    Component(string, string, string, string);
    Component();
    string mfr;
    string mpn;
    string description;
    string type;
    int supplierNumber;
    Component* parent;


    friend std::ostream& operator<< (std::ostream& os, const Component& obj){
        os << "MFR: " << obj.mfr << " .. MPN: " << obj.mpn << " .. Type: " << obj.type << " .. DESC: " << obj.description;
        return os;
    }

    void setParent(Component*);

};

#endif // COMPONENT_H
