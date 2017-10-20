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

using namespace std;


////////////
/// \brief The LanguageProcessor class
/// Handles part-of-speech tagging
/// Builds and manages the tagged dictionary obtained the Brown Corpus
class LanguageProcessor{

private:
    vector<pair<string, string>>* dict;
    map<string, map<string, int>>* tagCounts;
    string getTag(string);

public:

    LanguageProcessor();
    ~LanguageProcessor();

    int getXML();
    vector<pair<string,string>>& getDict();
    void tag(vector<string>&, vector<pair<string, string>>&);
    void countTags();
    int getTestCase(vector<string>&);

    string toupper(string&);

};





