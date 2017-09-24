#include "hcluster.h"
#include "utilityalgorithms.h"
#include<map>
#include<iostream>


HCluster::HCluster(){
    data = new vector<Component*>();
}

HCluster::~HCluster(){
    delete data;
}


//can two clusters be merged?
bool HCluster::checkForMerge(HCluster* hc){
    bool ret = false;
    for(Component* s1: *data){
        for(Component* s2: *hc->data){
            int dist = UtilityAlgorithms::levDist(*s1, *s2);
            if(dist > HCluster::MAX_VARIANCE) return false;
            if(dist <= HCluster::INCLUSION_CRITERION) ret = true;
        }
    }
    return ret;
}

//merge two clusters -- if true, deletes the second cluster
void HCluster::merge(HCluster* hc){
    for(Component* s: *hc->data){
        data->push_back(s);
    }

    delete hc;
}

//check if a string goes in the cluster
bool HCluster::checkForAdd(Component* st){
    bool ret = false;
    int dist = 0;

    for(Component* s: *data){

        //use this to include on basis of substring
        //int subs = UtilityAlgorithms::longestCommonSS(*st, *s);

        //use this to include on basis of Levenshtein distance
        int dist2 = UtilityAlgorithms::levDist(*st, *s); //also try this with dist - LCSS
        if(dist2 > dist) dist = dist2;

        //use this to reject on excessive Levenshtein distance
        if(dist2 > HCluster::MAX_VARIANCE){
            //cout << "REJECTED (dist: " << dist2 << ") -- " << st->mpn << " -- " << s->mpn << endl;
            return false;
        }

        //include on substring
        //if(subs >= HCluster::MIN_SS) ret = true;

        //include on distance
        if(dist2 <= HCluster::INCLUSION_CRITERION) {
            ret = true;
        }
    }

    return ret;
}


//add a string to the cluster
void HCluster::add(Component* st){
    data->push_back(st);
}

int HCluster::numEntries(){
    return data->size();
}

int HCluster::numCategories(){
    map<string, int> counts;
    for(Component* c: *data){
        counts[c->type]++;
    }

    return counts.size();
}

void HCluster::dumpComponents(){
    for(Component* comp: *data){
        std::cout << comp->mfr << " : " << comp->mpn << " : " << comp->type << endl;
    }
}

