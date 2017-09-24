#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <iostream>
#include "utilityalgorithms.h"
#include "repository.h"
#include "hcluster.h"
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

    Repository repo = Repository();
    vector<Component*> collection = vector<Component*>();
    vector<HCluster*> clusters = vector<HCluster*>();
    repo.getComponents(collection);

    int i = 0;
    for(Component* c: collection){
        ++i;
        if(i == 100) break;
        bool added = false;
        if(c->mfr.compare("ANY SUPPLIER") == 0) continue;

        for(HCluster* clust: clusters){
            if(clust->checkForAdd(c)){
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

    cout << "Opened " << collection.size() << " components." << endl;
    cout << "Found " << clusters.size() << " clusters." << endl;

    for(HCluster* c: clusters){
        int sz = c->numCategories();
        cout << "Cluster -- size: " << c->numEntries() << "   Number of categories: " << sz << endl;
        if(sz >= 10){
            cout << "Large category:" << endl;
            c->dumpComponents();
        }
    }

    cout<< "TEST" << endl << "593D476X9020D2TE3" << " " << "BLM18PG181SH1D" << endl;
    cout << UtilityAlgorithms::levDist("593D476X9020D2TE3", "BLM18PG181SH1D");



    /* Code for testing out the strign distance
     * string s1 = "Kittens";
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
