#ifndef TESTFILE_H
#define TESTFILE_H

class TestFile{
public:
    string filename;
    string text;
    vector<string> lines;
    vector<vector<pair<string,string>>*> nounPhrases;
    vector<vector<pair<string,string>>*> verbPhrases;
    map<pair<string, string>, string> collocations;
    vector<string> words;
    vector<pair<string, string>> tags;

    enum enums::TOPIC topic;
    TestFile* parent;

};


#endif // TESTFILE_H
