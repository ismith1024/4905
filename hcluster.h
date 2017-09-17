#ifndef HCLUSTER_H
#define HCLUSTER_H

#endif // HCLUSTER_H

#include <vector>
#include <string>
#include <iterator>
#include "component.h"

using namespace std;

class HCluster{
public:
    vector<component*>* data;
    static const int INCLUSION_CRITERION = 1;
    static const int MAX_VARIANCE = 4;

    HCluster();
    ~HCluster();

    bool checkForMerge(HCluster*);
    void merge(HCluster*);
    bool checkForAdd(Component*); //check if a string goes in the cluster
    void add(Component*); //add a string to the cluster

};
