BayesianStringClassifier::BayesianStringClassifier(){
    frequencies = new map<string, map<string, int>>();
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
void BayesianStringClassifier::learn(vector<Component>* comps){
    for(Component* c: comps){
        for(int i = 0; i < c->mpn.size() -2; ++i){
            String s = c->mpn.substr(i, i+2);
            *frequencies[s][c->type]++;
        }

    }
}


string BayesianStringClassifier::classify(Component){}

