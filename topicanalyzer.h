#ifndef TOPICANALYZER_H
#define TOPICANALYZER_H

#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include "repository.h"

class TopicAnalyzer{

public:
    enum TOPIC{
        METAL,
        PLASTIC,
        CABLE,
        ASSEMBLY,
        OTHER,
        PCBA,
        LABEL,
        ELECTRONICS,
        PACKAGING
    };

    enum TOPIC findTopic(string);
    enum TOPIC findTopic(vector<string>&);

    TopicAnalyzer(Repository&);

private:
    Repository repo;
    map<string,int> brownCounts;
    map<string,int> metalCounts;
    map<string,int> plasticCounts;
    map<string,int> cableCounts;
    map<string,int> assemblyCounts;
    map<string,int> otherCounts;
    map<string,int> pcbaCounts;
    map<string,int> labelCounts;
    map<string,int> electronicsCounts;
    map<string,int> packagingCounts;

}

#endif // TOPICANALYZER_H
