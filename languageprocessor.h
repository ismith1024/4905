#ifndef LANGUAGEPROCESSOR_H
#define LANGUAGEPROCESSOR_H
#endif // LANGUAGEPROCESSOR_H

#include<vector>
#include<string>
#include<iostream>
#include<fstream>




using namespace std;

class LanguageProcessor{

private:
    vector<pair<string, string>>* dict;

public:

    LanguageProcessor();
    ~LanguageProcessor();
    vector<pair<string,string>>& getDict();

    int getXML();


};





