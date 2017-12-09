#include "testfile.h"

string TestFile::toString(){
    string ret = "| FILE: " + filename + "| " + title + " | PARENT: ";
    if(parent == NULL) ret += " ROOT |";
    else ret += parent->filename;
    ret += "\n";

    for(auto& e1: comps){
        if(e1->parent == NULL)
        ret += "| ....Component " + e1->title + " | MFR: " + e1->mfr + " | MPN: " + e1->mpn + " | TYPE: " + e1->type + " | DESC: " + e1->description + "| Parent : ROOT |\n";
        else
        ret += "| ....Component " + e1->title + " | MFR: " + e1->mfr + " | MPN: " + e1->mpn + " | TYPE: " + e1->type + " | DESC: " + e1->description + "| Parent : " + e1->parent->title +"|\n";
    }

    return ret;
}
