#ifndef TOPICANALYZER_H
#define TOPICANALYZER_H

#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include "repository.h"
#include "enums.h"
//#include "tokenizer.h"

class TopicAnalyzer{

public:

    enum enums::TOPIC findTopic(string, Repository&);
    enum enums::TOPIC findTopic(vector<string>&, Repository&);

    void printTopicWords();

    TopicAnalyzer(Repository&);

private:
    //Repository repo;
    //Tokenizer tok;
    map<string,int> aggregateCounts;
    map<string,int> metalCounts;
    map<string,int> plasticCounts;
    map<string,int> cableCounts;
    map<string,int> assemblyCounts;
    map<string,int> otherCounts;
    map<string,int> pcbaCounts;
    map<string,int> labelCounts;
    map<string,int> electronicsCounts;
    map<string,int> packagingCounts;

};

#endif // TOPICANALYZER_H
