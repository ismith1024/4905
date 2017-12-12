#ifndef COMPONENT_H
#define COMPONENT_H

#include<string>
#include<iostream>
#include"vector"

using namespace std;

class Component{

public:
    Component(string, string, string, string);
    Component();
    string title;
    string mfr;
    string mpn;
    string description;
    string type;
    int supplierNumber;
    Component* parent;
    vector<Component*> children;


    friend std::ostream& operator<< (std::ostream& os, const Component& obj){
        os << "MFR: " << obj.mfr << " .. MPN: " << obj.mpn << " .. Type: " << obj.type << " .. DESC: " << obj.description << ((!obj.parent) ? " .. Parent: ROOT" : (" .. Parent: " + obj.parent->description)) << endl;
        //for(auto& e1: obj.children) os << *e1 << endl;
        return os;
    }

    bool operator==(Component&);

    void setParent(Component*);

};

#endif // COMPONENT_H
