#ifndef TOKENIZER_H
#define TOKENIZER_H

#endif // TOKENIZER_H

#include"sqlrecord.h";

using namespace std;

class Tokenizer{

private:
    vector<SQLrecord*>* records;
    string workingFile;


public:
    Tokenizer(string);
    ~Tokenizer();
    void tokenize();
    void writeToSQL();

}
