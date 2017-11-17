#include "bayesianclassifier.h"

BayesianClassifier::BayesianClassifier(){
    frequencies = new map<string, StringRecord>();
}


BayesianClassifier::~BayesianClassifier(){

    delete frequencies;
}


//////////
/// \brief BayesianStringClassifier::classifyCollocations
/// \input parameter: a collection of collocations <string, string>
/// \return a pointer to the results.  format -- [class: probability]
/// <word1, word2>, class
///
/// implements pr(class | coll) = pr(coll | class) * pr(class) / pr(coll)
/// Requires strings to be in lower case
/// RETURNS NULL POINTER IF THE COLLOCATION DOES NOT EXIST IN THE CORPUS
///
map<string, float>* BayesianClassifier::classifyCollocation(pair<string,string>& collocation, vector<Component*>& comps){

    //we will ignore components without a description field
    int totalComponents = 0;

    //calculate pr(class)
    map<string, float> prClass = map<string, float>();
    for(auto& entry: comps){
        if(entry->description != ""){
            prClass[entry->type]++;
            totalComponents ++;
        }
    }
    //divide by the denominator to find probability
    for(auto& entry: prClass){
        entry.second /= totalComponents;
    }

    //calculate pr(coll)
    //get rid of the map, only need a float
    //map<pair<string,string>, float> prColl = map<pair<string,string>, float>();
    float prColl = 0.0;
    for(auto& entry: comps){
        if(entry->description != ""){
            bool hasFirst = false;
            bool hasSecond = false;
            QStringList pieces = QString::fromStdString(entry->description).toLower().split(" ");
            for(auto& e2: pieces){
                string test = e2.toStdString();
                if(test == collocation.first) hasFirst = true;
                else if(test == collocation.second) hasSecond = true;
            }
            if(hasFirst && hasSecond) prColl++;//prColl[collocation]++;
        }
    }
    //for(auto& entry: prColl) entry.second /= totalComponents;
    prColl /= totalComponents;
    if(prColl == 0) return 0;

    //Apply Beayes' Theorem
    //calculate pr(coll | class)
    //key = the class that is given
    //value = probability
    map<string, float> prCollGivenClass = map<string, float>();
    for(auto& entry: comps){
        if(entry->description != ""){
            bool hasFirst = false;
            bool hasSecond = false;
            QStringList pieces = QString::fromStdString(entry->description).toLower().split(" ");
            for(auto& e2: pieces){
                string test = e2.toStdString();
                if(test == collocation.first) hasFirst = true;
                else if(test == collocation.second) hasSecond = true;
            }
            if(hasFirst && hasSecond) prCollGivenClass[entry->type]++;
        }
    }
    for(auto& entry: prCollGivenClass){
        entry.second /= prClass[entry.first] * totalComponents;

    }

    //put the results together:
    map<string, float>* ret = new map<string, float>();

    //iterates over all <class, probability> tuples where class is known to this function
    for(auto& entry: prClass){
        (*ret)[entry.first] = prCollGivenClass[entry.first] * entry.second / prColl;
    }

    return ret;

}

//////////////////
/*
This learning will:
    - iterate a three-character window over the part number of all components
    - increment the frequencies map as follows:
        - <C1C2C3: <classification: count++> >

*/
void BayesianClassifier::learn(vector<Component*>& comps){

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

////////////
/// \brief BayesianClassifier::classify
/// \param comp
/// \param components
/// \return
///
map<string, float>* BayesianClassifier::classify(Component* comp, vector<Component*>& components){
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

//////////////////////
/// \brief BayesianClassifier::classify
/// \param comp
/// \param components
/// \return
///
map<string, float>* BayesianClassifier::classify(string& comp, vector<Component*>& components){

    map<string, float>* ret = new map<string, float>();
    if(comp.size() < 3) return ret;

    //find the substrings in the component's part number
        vector<string> substrings = vector<string>();
        for(int i = 0; i < comp.size() -2; ++i){
            substrings.push_back(comp.substr(i, i+2));
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

/////////////////
/// \brief BayesianClassifier::classifySupplier
/// \return
/// Similar to matieral-article classifier:
/// Determines the probability of a manufacturer for an arbitrary alphanumeric string
///
/// Pr(supplier | trigram) = Pr(trigram | supplier) * Pr(supplier) / Pr(trigram)
/// RETURNS NULL IF INVALID WHICH MUST BE HANDLED
///
map<string, float>* BayesianClassifier::classifySupplier(string& mpn, vector<Component*>& components){

    if(mpn.length() < 3) return 0;

    auto trigrams = vector<string>();
    auto ret = new map<string,float>();

    /*string::iterator it, it2;
    for(it = mpn.begin(); it != mpn.end() -3; ++it){

    }*/
    for(int i = 0; i < mpn.length() - 2; ++i){
        trigrams.push_back(mpn.substr(i,3));
    }

    for(auto& entry: trigrams){
        cout << entry << endl;
    }


    map<string, float> prTrigs = map<string, float>();

    for(auto& trig: tragrams){

        prTrigs = map<string,float>();

        //Pr(supplier | trigram)
        int count = 0;
        map<string, float> prSuppGivenTrig = map<string, float>();

        //prTrigs
        for(auto& comp: comps){
            std::size_t found = e2->mpn.find(entry);
            if(found != string::npos)
               prTrigs[trig]++;
        }

        int sz = components.size();

        for(auto& entry: prTrigs){
            entry.second /= sz;
        }

        //pr supp | trig
        for(auto& e2: components){

            std::size_t found = e2->mpn.find(entry);
            if(found != string::npos){
               prSuppGivenTrig[e2->mfr]++;
               count++;
           }
        }



        for(auto& entry: prSuppGivenTrig){
            entry.second /= count;

            //if the working return map doesn't contain the supplier, set it to 1.0
            map<string,float>::iterator it = ret->find(entry.first);
            if(it != ret->end()) ret->operator[](entry.first) = 1.0;

            //conjjugate probability multiplied by the conjugate of the trigram
            ret->operator[](entry.first) *= (1.0 - entry.second) / (prTrigs[trig]);
        }

        //pr(supplier)
        map<string, float> prSupp = map<string, float>();
        for(auto& entry: components){
            prSupp[entry->mfr]++;
        }

        int sz = components.size();

        for(auto& entry: prSupp){
            entry.second /= sz;
        }

        //complete bayes theorem
        //not right
        for(auto& entry: (*ret)){
            entry.second *= prSupp[entry.first];
        }

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

