#include "bayesianclassifier.h"

BayesianClassifier::BayesianClassifier(){
    substringFrequencies = new map<string, StringRecord>();

}


BayesianClassifier::~BayesianClassifier(){

    delete substringFrequencies;

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
///
///This learning will:
///    - iterate a three-character window over the part number of all components
///    - increment the frequencies map as follows:
///        - <C1C2C3: <classification: count++> >
///
///
void BayesianClassifier::learn(vector<Component*>& comps){

    //map<string, StringRecord>& freq = *frequencies;
    //counts the frequency of substrings given component type, and the frequency of each substring
    for(Component* c: comps){
        if(c->mpn.size() < 3) continue;
        for(int i = 0; i < c->mpn.size() -2; ++i){
            string s = c->mpn.substr(i, i+2);
            (*substringFrequencies)[s].count++;
            (*(*substringFrequencies)[s].entries)[c->mpn]++;
        }
    }

    cout << "Learning complete" << endl;
}


////////////
/// \brief BayesianClassifier::classify
/// \param comp - the Component object we are trying to classify
/// \param components - the training set
/// \return
///
map<string, float>* BayesianClassifier::classifyType(Component* comp, vector<Component*>& components){
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
/// \brief BayesianClassifier::classifyType
/// \param comp - the part number of the component weare trying to classify
/// \param components - the training set
/// \return
///
map<string, float>* BayesianClassifier::classifyType(string& comp, vector<Component*>& components){

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

////////////
/// \brief BayesianClassifier::classifySupplier
/// \param comp - the component we are trying to classify
/// \param components - the training set
/// \return
///
map<string, float>* BayesianClassifier::classifySupplier(Component* comp, vector<Component*>& components){
    map<string, float>* ret = new map<string, float>();

    //find the substrings in the component's part number
        vector<string> substrings = vector<string>();
        for(int i = 0; i < comp->mpn.size() -2; ++i){
            substrings.push_back(comp->mpn.substr(i, i+2));
        }

    //count the components
        map<string, int> typeCounts = map<string, int>();
        for(Component* c: components){
            typeCounts[c->mfr]++;
        }

    //Determine the probability of each class
        map<string, float> probSupp = map<string, float>();

        for(Component* c: components){
            probSupp[c->mfr]++;
        }

        for(const auto& entry: probSupp){
            probSupp[entry.first] = entry.second / probSupp.size();
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
         map<pair<string, string>, float> probSubsGivenSupp = map<pair<string, string>, float>(); //first is mfr and second is substring

         for(Component* c: components){
             for(auto& entry: probSubs){
                 std::size_t found = c->mpn.find(entry.first);
                if(found != string::npos){
                    probSubsGivenSupp[make_pair(c->mfr, entry.first)]++;
                }
             }
         }

         //divide by the number of components with that type
         for(auto& entry: probSubsGivenSupp){
             probSubsGivenSupp[entry.first] = probSubsGivenSupp[entry.first] / typeCounts[entry.first.first];
         }

     //Determine the probability of type given substring

         //first, determine the probability it is not a type -- start with 1.0
         map<string, float> complementProbs = map<string, float>();
         for(auto& entry: probSupp){
             complementProbs[entry.first] = 1.0;
         }

         //second, multiply this by the probability that the substring implies a type
         for(string subs: substrings){
             if(probSubs[subs] == 0) continue;
             else for(auto& cSupp: probSupp){
                 complementProbs[cSupp.first] = complementProbs[cSupp.first] * (1.0 - (probSubsGivenSupp[make_pair(cSupp.first, subs)] * probSupp[cSupp.first] / probSubs[subs] ));
             }
         }

         for(auto& entry: complementProbs){
             (*ret)[entry.first] = 1.0 - entry.second;
         }


    return ret;
}

//////////////////////
/// \brief BayesianClassifier::classifySupplier
/// \param comp - the part number of the component we are trying to classify
/// \param components - the training set
/// \return
///
map<string, float>* BayesianClassifier::classifySupplier(string& comp, vector<Component*>& components){

    map<string, float>* ret = new map<string, float>();
    if(comp.size() < 3) return ret;

    //find the substrings in the component's part number
        vector<string> substrings = vector<string>();
        for(int i = 0; i < comp.size() -2; ++i){
            substrings.push_back(comp.substr(i, i+2));
        }

    //count the components
        map<string, int> suppCounts = map<string, int>();
        for(Component* c: components){
            suppCounts[c->mfr]++;
        }

    //Determine the probability of each class
        map<string, float> probSupp = map<string, float>();

        for(Component* c: components){
            probSupp[c->mfr]++;
        }

        for(const auto& entry: probSupp){
            probSupp[entry.first] = entry.second / probSupp.size();
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
         map<pair<string, string>, float> probSubsGivenSupp = map<pair<string, string>, float>(); //first is type and second is substring

         for(Component* c: components){
             for(auto& entry: probSubs){
                 std::size_t found = c->mpn.find(entry.first);
                if(found != string::npos){
                    probSubsGivenSupp[make_pair(c->mfr, entry.first)]++;
                }
             }
         }

         //divide by the number of components with that type
         for(auto& entry: probSubsGivenSupp){
             probSubsGivenSupp[entry.first] = probSubsGivenSupp[entry.first] / suppCounts[entry.first.first];
         }

     //Determine the probability of type given substring

         //first, determine the probability it is not a type -- start with 1.0
         map<string, float> complementProbs = map<string, float>();
         for(auto& entry: probSupp){
             complementProbs[entry.first] = 1.0;
         }

         //second, multiply this by the probability that the substring implies a type
         for(string subs: substrings){
             if(probSubs[subs] == 0) continue;
             else for(auto& cSupp: probSupp){
                 complementProbs[cSupp.first] = complementProbs[cSupp.first] * (1.0 - (probSubsGivenSupp[make_pair(cSupp.first, subs)] * probSupp[cSupp.first] / probSubs[subs] ));
             }
         }

         for(auto& entry: complementProbs){
             (*ret)[entry.first] = 1.0 - entry.second;
         }


    return ret;
}


StringRecord::StringRecord(){
    count = 0;
    entries = new map<string, int>();
}

StringRecord::~StringRecord(){
    delete entries;
}

