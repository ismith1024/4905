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
void BayesianStringClassifier::learn(vector<Component*>& comps){

    //map<string, StringRecord>& freq = *frequencies;
    //counts the frequency of substrings given component type, and the frequency of each substring
    for(Component* c: comps){
        if(c->mpn.size() < 3) continue;
        for(int i = 0; i < c->mpn.size() -2; ++i){
            string s = c->mpn.substr(i, i+2);
            (*frequencies)[s].count++;
            (*(*frequencies)[s].entries)[c->mpn]++;
        }
    }

    cout << "Learning complete" << endl;
}


/*
    To Bayesian classify a string on substrings:

    Pr(type given substring) = Pr(substring given type) * Pr(type) / Pr(substring)

    Here, probability of the type given the substring is

    For all substrings:

*/

map<string, float>* BayesianStringClassifier::classify(Component* comp, vector<Component*>& components){
    map<string, float>* ret = new map<string, float>();

    //find the substrings in the component's part number
        vector<string> substrings = vector<string>();
        for(int i = 0; i < comp->mpn.size() -2; ++i){
            substrings.push_back(comp->mpn.substr(i, i+2));
        }

    //count the components
        map<string, int> typeCounts = map<string, int>();
        for(Component* c: components){
            typeCounts[c->type]++;
        }

    //Determine the probability of each class
        map<string, float> probType = map<string, float>();

        for(Component* c: components){
            probType[c->type]++;
        }

        for(const auto& entry: probType){
            probType[entry.first] = entry.second / probType.size();
        }

    //probability of substring

         map<string, float> probSubs = map<string, float>();
         for(Component* c: components){
             for(string str: substrings){
                 if(c->mpn.find(str) != string::npos) probSubs[str]++;
             }
         }

         for(auto& entry: probSubs){
             probSubs[entry.first] = entry.second / probSubs.size();
         }

     //Probability of the substring given type
         map<pair<string, string>, float> probSubsGivenType = map<pair<string, string>, float>(); //first is type and second is substring

         for(Component* c: components){
             for(auto& entry: probSubs){
                 std::size_t found = c->mpn.find(entry.first);
                if(found != string::npos){
                    probSubsGivenType[make_pair(c->type, entry.first)]++;
                }
             }
         }

         //divide by the number of components with that type
         for(auto& entry: probSubsGivenType){
             probSubsGivenType[entry.first] = probSubsGivenType[entry.first] / typeCounts[entry.first.first];
         }

     //Determine the probability of type given substring

         //first, determine the probability it is not a type -- start with 1.0
         map<string, float> complementProbs = map<string, float>();
         for(auto& entry: probType){
             complementProbs[entry.first] = 1.0;
         }

         //second, multiply this by the probability that the substring implies a type
         for(string subs: substrings){
             if(probSubs[subs] == 0) continue;
             else for(auto& cType: probType){
                 complementProbs[cType.first] = complementProbs[cType.first] * (1.0 - (probSubsGivenType[make_pair(cType.first, subs)] * probType[cType.first] / probSubs[subs] ));
             }
         }

         for(auto& entry: complementProbs){
             (*ret)[entry.first] = 1.0 - entry.second;
         }


    return ret;
}

/*
map<string, float>* BayesianStringClassifier::classify2(Component* comp, vector<Component*>& components){

    //get the substrings in the component
        vector<string> substrings = vector<string>();
        for(int i = 0; i < comp->mpn.size() -2; ++i){
            substrings.push_back(comp->mpn.substr(i, i+2));
        }

    //Probability of type
        map<string, float> probType = map<string, float>();

        for(Component* c: components){
            probType[c->type]++;
        }

        for(const auto& entry: probType){
            probType[entry.first] = entry.second / probType.size();
        }


   //probability of substring

        map<string, float> probSubs = map<string, float>();
        for(Component* c: components){
            for(string str: substrings){
                if(c->mpn.find(str) != string::npos) probSubs[str]++;
            }
        }

        for(auto& entry: probSubs){
            probSubs[entry.first] = entry.second / probSubs.size();
        }


    //Probability of the substring given type
        map<pair<string, string>, float> probSubsGivenType = map<pair<string, string>, float>(); //first is type and second is substring

        for(Component* c: components){
            for(auto& entry: probSubs){
                probSubsGivenType[make_pair(c->type, entry.first)]++;
            }
        }

        //divide by the number of components with this type
        for(auto& entry: probSubsGivenType){
            probSubsGivenType[entry.first] = probType[entry.first.first] * probType.size(); // probSubsGivenType[entry] /= (probType[entry.first] * probType.size());
        }

     //return value:
        /*
        Pr(component is a type) = ~(Pr(component is not a type)) = 1 - Product_k=0_n_(Pr(component is not a type given substring_k)) for n substrings
        * /

        map<string, float>* ret = new map<string,float>();

        //apply Bayes' theorem: probability of type given substring
        for(auto& typeentry: probType){
            (*ret)[typeentry.first] = 1.0;
            for(string subs: substrings){
                //remove any substring that is not found anywhere
                bool stringunknown = true;
                for(auto& entry: probSubs){
                    if(probSubs[entry.first] > 0.0) stringunknown = false;
                }

                if(stringunknown) continue;
                else
                    (*ret)[typeentry.first] *= probSubsGivenType[make_pair(typeentry.first, subs)] * probType[typeentry.first] / probSubs[subs];
            }
        }

     return ret;

} */

StringRecord::StringRecord(){
    count = 0;
    entries = new map<string, int>();
}

StringRecord::~StringRecord(){
    delete entries;
}

