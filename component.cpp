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
}

/*
std::ostream& operator<< (std::ostream& os, const Component& obj){

}*/
