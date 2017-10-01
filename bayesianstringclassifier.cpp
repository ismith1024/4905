#include "bayesianstringclassifier.h"

BayesianStringClassifier::BayesianStringClassifier(){
    frequencies = new map<string, StringRecord>();
}


BayesianStringClassifier::~BayesianStringClassifier(){

    delete frequencies;
}



/*

*/
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
map<string, float>* BayesianStringClassifier::classify(Component* comp){

    //get the substrings
    vector<string> substrings = vector<string>();
    for(int i = 0; i < comp->mpn.size() -2; ++i){
        substrings.push_back(comp->mpn.substr(i, i+2));
    }

    //Probability of the substring given type
        map<string, float> subsGivenType = map<string, float>();

        //////probability of substring and probability of type
        float prSubs = 0.0;
        float prType = 0.0;
        float subscount = 0.0;
        float typeCount = 0.0;
        for(Component* c: components){
            if(c->mpn.find(substring) != string::npos) subscount++;
            if(c->type == type) typeCount++;
        }
        prSubs = count / components.size();
        prType = count / components.size();


    //Probability of type given substring
        map<string, float> typeGivenSubs = map<string, float>();
        for(Component* c: components){
            if(c->mpn.find(subs)) typeGivenSubs[c->type]++;
        }

        int totalCompsWithSubstring = 0;
        for(auto& entry: typeGivenSubs){
            totalCompsWithSubstring += typeGivenSubs[entry];
        }

        //normalize probability to number of components
        for(auto& entry: typeGivenSubs){
            typeGivenSubs[entry] /= totalCompsWithSubstring;
        }

    //return value:
        /*
        Pr(component is a type) = ~(Pr(component is not a type)) = 1 - Product_k=0_n_(Pr(component is not a type given substring_k)) for n substrings
        */

        map<string, float>* ret = new map<string,float>*();

        //apply Bayes' theorem
        for()


     return ret;

}

StringRecord::StringRecord(){
    count = 0;
    entries = new map<string, int>();
}

StringRecord::~StringRecord(){
    delete entries;
}

