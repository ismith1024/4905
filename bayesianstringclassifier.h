#ifndef BAYESIANSTRINGCLASSIFIER_H
#define BAYESIANSTRINGCLASSIFIER_H



#include<string>
#include<vector>
#include<map>
#include "component.h"
#include"QString"
#include"QStringList"

using namespace std;

class StringRecord{

public:
    int count;
    map<string, int>* entries;

    StringRecord();
    ~StringRecord();
};

class BayesianStringClassifier{

private:
    static const int MIN_SUPPORT = 10;
    static const int MNI_CONF = 5;
    map<string, StringRecord>* frequencies;

public:
    void learn(vector<Component*>&);
    //void learnCollocations(vector<Component*>&);

    map<string, float>*  classify(Component*, vector<Component*>&);
    map<string, float>*  classify(string&, vector<Component*>&);

    map<string,float>* classifyCollocation(pair<string,string>&, vector<Component*>&);

    BayesianStringClassifier();
    ~BayesianStringClassifier();

};


#endif // BAYESIANSTRINGCLASSIFIER_H
