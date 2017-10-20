#ifndef TOKENIZER_H
#define TOKENIZER_H



#include "sqlrecord.h"
#include<vector>
#include "QString"

using namespace std;

class Tokenizer{

private:
    vector<SQLrecord*>* records;
    string workingFile;


public:
    Tokenizer(string);
    Tokenizer();
    ~Tokenizer();
    void tokenize();
    void writeToSQL();
    void replaceStopCharacters(string&);
    void removeStopCharacters(QString&);

};

#endif // TOKENIZER_H
