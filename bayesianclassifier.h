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

class BayesianClassifier{

private:
    static const int MIN_SUPPORT = 10;
    static const int MNI_CONF = 5;
    map<string, StringRecord>* substringFrequencies;


public:
    void learn(vector<Component*>&);
    //void learnCollocations(vector<Component*>&);

    map<string, float>*  classifyType(Component*, vector<Component*>&);
    map<string, float>*  classifyType(string&, vector<Component*>&);

    map<string, float>*  classifySupplier(Component*, vector<Component*>&);
    map<string, float>*  classifySupplier(string&, vector<Component*>&);

    map<string,float>* classifyCollocation(pair<string,string>&, vector<Component*>&);
    map<string, float>* classifySupplier_Old(string&, vector<Component*>&);

    BayesianClassifier();
    ~BayesianClassifier();

};


#endif // BAYESIANSTRINGCLASSIFIER_H
