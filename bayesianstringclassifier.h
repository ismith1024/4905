#ifndef BAYESIANSTRINGCLASSIFIER_H
#define BAYESIANSTRINGCLASSIFIER_H

#endif // BAYESIANSTRINGCLASSIFIER_H

#include<string>
#include<vector>
#include<map>
#include "component.h"


class BayesianStringClassifier{

private:
    static const int MIN_SUPPORT = 10;
    static const int MNI_CONF = 5;
    map<string, map<string, int>>* frequencies;

public:
    static void learn(vector<Component>);
    static string classify(Component);

    BayesianStringClassifier();
    ~BayesianStringClassifier();

};

