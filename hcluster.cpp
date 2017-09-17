#include "hcluster.h"
#include "utilityalgorithms.h"


HCLuster::HCluster(){
    data = new vector<string>();
}

~HCluster(){
    delete data;
}

//can two clusters be merged?
bool HCLuster::checkForMerge(HCluster* hc){
    bool ret = false;
    for(string s1: data){
        for(string s2: hc->data){
            int dist = levdist(s1, s2);
            if(dist > HCluster::MAX_VARIANCE) return false;
            if(dist <= HCluster::INCLUSION_CRITERION) ret = true;
        }
    }
    return ret;
}

//merge two clusters -- if true, deletes the second cluster
void HCLuster::merge(HCluster* hc){
    for(string s: hc-> data){
        data.push_back(s);
    }

    delete hc;
}

//check if a string goes in the cluster
bool HCLuster::checkForAdd(string& st){
    bool ret = false;

    for(string s: data){
        int dist = levdist(st, s); //also try this with dist - LCSS
        if(dist > HCluster::MAX_VARIANCE) return false;
        if(dist <= HCluster::INCLUSION_CRITERION) ret = true;
    }

    return ret;
}


//add a string to the cluster
void HCLuster::add(string& st){
    data.push_back(st);
}
