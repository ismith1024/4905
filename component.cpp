#include "component.h"
#include<algorithm>

Component::Component(string mf, string mp, string de, string ty){
    mfr = mf;
    mp.erase(remove(mp.begin(), mp.end(), ' '), mp.end());
    mp.erase(remove(mp.begin(), mp.end(), '('), mp.end());
    mp.erase(remove(mp.begin(), mp.end(), ')'), mp.end());
    mp.erase(remove(mp.begin(), mp.end(), '-'), mp.end());

    if(mp.size() < 3)
        mp.insert(0, 3 - mp.size(), '-');

    mpn = mp;
    //mpn = replace(mp.begin(), mp.end(), ' ');

    for (auto& c: mpn)
        c = toupper(c);

    description = de;
    type = ty;
    parent = NULL;

}

Component::Component(){
    parent = NULL;
}

bool Component::operator==(Component& c){
    return(mpn == c.mpn &&
           mfr == c.mfr &&
           type == c.type);
}

/*
    string mfr;
    string mpn;
    string description;
    string type;*/

void Component::setParent(Component* par){
    parent = par;
    par->children.push_back(this);
}


