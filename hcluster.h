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
    vector<Component*>* data;
    static const int INCLUSION_CRITERION = 3;
    static const int MAX_VARIANCE = 6;

    HCluster();
    ~HCluster();

    bool checkForMerge(HCluster*);
    void merge(HCluster*);
    bool checkForAdd(Component*); //check if a string goes in the cluster
    void add(Component*); //add a string to the cluster

    int numEntries();
    int numCategories();

    void dumpComponents();
};
