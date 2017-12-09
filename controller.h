#ifndef CONTROLLER_H
#define CONTROLLER_H



#include<string>
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include "component.h"
#include "bayesianclassifier.h"
#include "tokenizer.h"
#include "repository.h"
#include "topicanalyzer.h"
#include "languageprocessor.h"
#include "testfile.h"
#include "utilityalgorithms.h"
#include "hcluster.h"

using namespace std;

class Controller
{

private:
    LanguageProcessor processor;
    Tokenizer tok;
    Repository repo;
    TopicAnalyzer top;

public:
    Controller() : repo(&tok), top(repo){}
    void handleTokenizeRequest();
    int getTextFromFile(vector<string>&, Tokenizer&);
    int getTestCase1(vector<string>&, Tokenizer&);
    int getTestCase2(vector<string>&, Tokenizer&);
    int getTestCase3(vector<string>&, Tokenizer&);
    void crossValidateType(BayesianClassifier&, vector<Component*>&);
    void crossValidateSupp(BayesianClassifier&, vector<Component*>&);

    int tokenize(string);

    int classifyMatArtType(string);

    void getCollocationsFromDBDescriptions(vector<pair<string, string>>&);

    void run();
    void run2();
    void runTestCase(int);
    void cleanTestCase(int);
    //void rungui(int, char*);

    ////Unit Testing
    int testAugmentedVocabulary(int);
    int testClassifyMatType();
    int testFindCollocations(); // Finding quasi-word collocations
    int testClassifyCollocations(); //Classifying a quasi-word collocation
    int testNounPhrases(); // Finding noun phrases in free text
    int testVerbPhrases(); // Finding verb phrase fragments in free text
    int testClassifySupplier(); // Classifying the supplier of an identified part number
    //int testEntityDeduplication(); //Entity deduplication
    int testBuildComponents();
    void testParent();
    void testGetMIM();

    int buildComponentsFromPhrase(BayesianClassifier&, vector<pair<string, string>>&, vector<pair<string,string>>&, vector<Component*>&, vector<Component*>&, vector<Component*>&);
    //void buildComponentsFromPhrase(vector<pair<string,string>>&, vector<pair<string,string>>&, vector<Component*>&);

    void runOneOf();

    void consolidateCollection(vector<Component*>&);

};

#endif // CONTROLLER_H
