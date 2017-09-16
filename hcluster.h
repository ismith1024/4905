#ifndef HCLUSTER_H
#define HCLUSTER_H

#endif // HCLUSTER_H

#include <vector>
#include <string>
#include <iterator>

using namespace std;

class HCluster{
public:
    vector<string>* data;
    static const int INCLUSION_CRITERION = 1;
    static const int MAX_VARIANCE = 4;

    HCluster();
    ~HCluster();

    bool checkForMerge(HCluster*);
    void merge(HCluster*);
    bool checkForAdd(string&); //check if a string goes in the cluster
    void add(string&); //add a string to the cluster

};
