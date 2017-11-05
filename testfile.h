#ifndef TESTFILE_H
#define TESTFILE_H

class testFile{
public:
    string filename;
    string text;
    vector<string> words;
    vector<string, string> tags;
    enum enums::TOPIC topic;
    testFile* parent;
    vector<string> nounPhrases;
    vector<string> verbPhrases;
};


#endif // TESTFILE_H
