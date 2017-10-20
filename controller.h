#ifndef CONTROLLER_H
#define CONTROLLER_H



#include<string>
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include "component.h"
#include "bayesianstringclassifier.h"
#include "tokenizer.h"

using namespace std;

class Controller
{
public:
    Controller();
    void handleTokenizeRequest();
    int getTextFromFile(vector<string>&, Tokenizer&);
    void crossValidate(BayesianStringClassifier&, vector<Component*>&);

    int tokenize(string);

    int classifyAlpha(string);

    void run();

};

#endif // CONTROLLER_H
