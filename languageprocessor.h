#ifndef LANGUAGEPROCESSOR_H
#define LANGUAGEPROCESSOR_H
#endif // LANGUAGEPROCESSOR_H

#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<map>
#include"QString"
#include"QStringList"
#include"repository.h"

using namespace std;


////////////
/// \brief The LanguageProcessor class
/// Handles part-of-speech tagging
/// Builds and manages the tagged dictionary obtained the Brown Corpus
class LanguageProcessor{

private:
    vector<pair<string, string>>* dict;
    map<string,string>* techdict;
    map<string, map<string, int>>* tagCounts;
    string getTag(string);

public:

    LanguageProcessor();
    ~LanguageProcessor();

    int getXML();
    vector<pair<string,string>>& getDict();
    void tag(vector<string>&, vector<pair<string, string>>&);
    void countTags();
    //int getTestCase(vector<string>&);
    int openTechDictionary(Repository&);
    int applyTechDictionary(vector<pair<string,string>>&);

    string toupper(string&);

    int getNounPhrases(vector<pair<string,string>>&, vector<vector<pair<string, string>>*>&);
    bool isNoun(pair<string,string>&);
    bool isAdjective(pair<string,string>&);
    bool isPreposition(pair<string,string>&);

};





