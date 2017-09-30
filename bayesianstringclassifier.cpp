#include "bayesianstringclassifier.h"

BayesianStringClassifier::BayesianStringClassifier(){
    frequencies = new map<string, StringRecord>();
}


BayesianStringClassifier::~BayesianStringClassifier(){

    delete frequencies;
}


/*
This learning will:
    - iterate a three-character window over the part number of all components
    - increment the frequencies map as follows:
        - <C1C2C3: <classification: count++> >

*/
void BayesianStringClassifier::learn(vector<Component*>* comps){

    //map<string, StringRecord>& freq = *frequencies;
    //counts the frequency of substrings given component type, and the frequency of each substring
    for(Component* c: *comps){
        for(int i = 0; i < c->mpn.size() -2; ++i){
            string s = c->mpn.substr(i, i+2);
            (*frequencies)[s].count++;
            (*(*frequencies)[s].entries)[c->mpn]++;
        }
    }
}


/*
    To Bayesian classify a string on substrings:

    Pr(type given substring) = Pr(substring given type) * Pr(type) / Pr(substring)

    Here, probability of the type given the substring is

    For all substrings:

*/
string BayesianStringClassifier::classify(Component comp){

    //Probability of type given substring
    /*
        for(substring ss in comp.mpn){

        }

    */


}

StringRecord::StringRecord(){
    count = 0;
    entries = new map<string, int>();
}

StringRecord::~StringRecord(){
    delete entries;
}

