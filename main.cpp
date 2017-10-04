#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <iostream>
#include "utilityalgorithms.h"
#include "repository.h"
#include "hcluster.h"
#include<map>
#include<list>
#include "bayesianstringclassifier.h"
//#include "component.h"

/* code for the GUI app
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
*/

using namespace std;

int main(int argc, char* argv[]){

    //////////////////

    int INCLUSION_CRITERION = 4;
    int MAX_VARIANCE = 6;

    int MIN_SS = 3;

    float INCLUSION_RATIO = 0.33333;

    ////////////////


    Repository repo = Repository();
    vector<Component*> collection = vector<Component*>();
    vector<Component*> testing = vector<Component*>();
    vector<Component*>::iterator it;
    int i = 0;
    //vector<HCluster*> clusters = vector<HCluster*>();
    repo.getComponents(collection);

    /*for(Component* comp: collection){
        cout << *comp << endl;
    }*/

    BayesianStringClassifier bayes = BayesianStringClassifier();

    list<Component*> training = list<Component*>();

    for(it = collection.begin(); it != collection.end(); ++i ,++it){
        if(i % 4 == 0){
            testing.push_back(*it);
        } else training.push_back(*it);
    }

    vector<Component*> training2;
    training2.reserve(training.size());
    copy(begin(training), end(training), back_inserter(training2));

    cout << "Size of training: " << collection.size() << "  Testing: " << testing.size() << endl;

    bayes.learn(training2);
    //bayes.learn(collection);
    cout << "Learning" << endl;

    //Component* testComp = new Component("Bob's bolts", "1215", "Some widget", "");

    int right = 0;
    int wrong = 0;

    for(Component* c: testing){
        map<string, float>* results = bayes.classify(c, collection);
        auto choice = std::max_element(results->begin(), results->end(),
            [](const pair<string, float>& p1, const pair<string, float>& p2) {
                return p1.second < p2.second; });
        if((*choice).first.compare(c->type) == 0) right++; else wrong++;
            cout << "Right: " << right << " Wrong: " << wrong << endl;
        delete results;
    }





    //map<string, float>* results = bayes.classify(testComp, collection);

    /*for(auto& entry: *results){
        cout << entry.first << " .. " << entry.second << endl;
    }*/


    ///////////////
    /// CLUSTERING
    ///
    /*
    int i = 0;
    for(Component* c: collection){
        ++i;
        //if(i == 100) break;
        bool added = false;
        if(c->mfr.compare("ANY SUPPLIER") == 0) continue;

        for(HCluster* clust: clusters){
            if(clust->checkForAdd(c, MAX_VARIANCE, INCLUSION_CRITERION)){
                clust->add(c);
                added = true;
                //cout << "Found cluster for " << c->mfr << " : " << c->mpn << endl;
            }
            if(added) break;
        }

        if(!added){
            HCluster* newClust = new HCluster();
            newClust->add(c);
            clusters.push_back(newClust);
            //cout << "Create new cluster " << c->mfr << " : " << c->mpn << endl;
        }
    }

    int totalComps = 0;
    int minorityComps = 0;
    int singletonComps = 0;
    int majoritycomps = 0;

    for(HCluster* clus: clusters){

        int sz = clus->numCategories();
        cout << "Cluster -- size: " << clus->numEntries() << "   Number of categories: " << sz << endl;
        if(sz >= 10){
            cout << "Large category:" << endl;
            clus->dumpComponents();
        }

        totalComps += clus->data->size();

        if(clus->data->size() == 1){
            singletonComps++;
        } else{
            map<string, int> counts;

            for(Component* comp: *(clus->data)){
                counts[comp->type]++;
            }

            int max = 0;
            for(auto& item: counts){
                if(item.second > max) max = item.second;
            }

            minorityComps += clus->data->size() - max;
            majoritycomps += max;
        }


    }

    cout << "---------------------" << endl << "Opened " << collection.size() << " components." << endl;
    cout << "Found " << clusters.size() << " clusters." << endl;

    cout << "Total components: " << totalComps << endl << "Singleton components: " << singletonComps << endl << "Minority Components: " << minorityComps << endl << "Majority components: " << majoritycomps << endl;
    cout << "---------------------" << endl << endl;
    //cout<< "TEST" << endl << "593D476X9020D2TE3" << " " << "BLM18PG181SH1D" << endl;
    //cout << UtilityAlgorithms::levDist("593D476X9020D2TE3", "BLM18PG181SH1D");
*/


    /* Code for testing out the strign distance
     * string s1 = "Kittens"; << endl
    string s2 = "Mittens";
    string s3 = "Mitts";

    int test = UtilityAlgorithms::longestCommonSS(s1, s2);

    cout << "LCSS -- " << s1 << ", " << s2 << " " << test << endl;

    test = UtilityAlgorithms::levDist(s1, s2);

    cout << "Levenshtein -- " << s1 << ", " << s2 << " " << test << endl;

    test = UtilityAlgorithms::longestCommonSS(s2, s3);

    cout << "LCSS -- " << s2 << ", " << s3 << " " << test << endl;

    test = UtilityAlgorithms::levDist(s2, s3);

    cout << "Levenshtein -- " << s2 << ", " << s3 << " " << test << endl;
*/


    /* TOKENIZER
    if(argc < 2){
        //qDebug() << "File name required";
        cout << "File name required";
        return 1;
    }

    Tokenizer* tok = new Tokenizer(argv[1]);
    tok->tokenize();
    tok->writeToSQL();

    delete tok;
    */
}
