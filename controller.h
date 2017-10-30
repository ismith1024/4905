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
#include "repository.h"
#include "topicanalyzer.h"
#include "languageprocessor.h"

using namespace std;

class Controller
{


private:
    LanguageProcessor processor;
    Tokenizer tok;
    Repository repo;
    TopicAnalyzer top;

public:
    Controller() : repo(tok), top(repo){};
    void handleTokenizeRequest();
    int getTextFromFile(vector<string>&, Tokenizer&);
    int getTestCase1(vector<string>&, Tokenizer&);
    int getTestCase2(vector<string>&, Tokenizer&);
    int getTestCase3(vector<string>&, Tokenizer&);
    void crossValidate(BayesianStringClassifier&, vector<Component*>&);

    int tokenize(string);

    int classifyAlpha(string);

    void getCollocationsFromDBDescriptions();

    void run();


};

#endif // CONTROLLER_H
