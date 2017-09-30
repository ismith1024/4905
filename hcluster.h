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
    //static const int INCLUSION_CRITERION = 5;
    //static const int MAX_VARIANCE = 7;

    //static const int MIN_SS = 3;

    //static const float INCLUSION_RATIO;

    HCluster();
    ~HCluster();

    bool checkForMerge(HCluster*, int, int);
    void merge(HCluster*);
    bool checkForAdd(Component*, int, int); //check if a string goes in the cluster
    void add(Component*); //add a string to the cluster

    int numEntries();
    int numCategories();

    void dumpComponents();
};
