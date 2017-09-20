#include "hcluster.h"
#include "utilityalgorithms.h"


HCluster::HCluster(){
    data = new vector<Component*>();
}

HCluster::~HCluster(){
    delete data;
}

/*
//can two clusters be merged?
bool HCLuster::checkForMerge(HCluster* hc){
    bool ret = false;
    for(Component* s1: data){
        for(Component* s2: hc->data){
            int dist = levdist(*s1, *s2);
            if(dist > HCluster::MAX_VARIANCE) return false;
            if(dist <= HCluster::INCLUSION_CRITERION) ret = true;
        }
    }
    return ret;
}

//merge two clusters -- if true, deletes the second cluster
void HCLuster::merge(HCluster* hc){
    for(Component* s: hc-> data){
        data.push_back(s);
    }

    delete hc;
}

//check if a string goes in the cluster
bool HCLuster::checkForAdd(Component* st){
    bool ret = false;

    for(Component* s: data){
        int dist = levdist(*st, *s); //also try this with dist - LCSS
        if(dist > HCluster::MAX_VARIANCE) return false;
        if(dist <= HCluster::INCLUSION_CRITERION) ret = true;
    }

    return ret;
}


//add a string to the cluster
void HCLuster::add(Component* st){
    data.push_back(st);
}
*/
