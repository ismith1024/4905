#include "controller.h"
#include "QDebug"
#include "languageprocessor.h"
#include "repository.h"
#include "component.h"
#include "bayesianstringclassifier.h"
#include "tokenizer.h"
#include "QStringList"
#include "QString"

Controller::Controller(){

}

void Controller::handleTokenizeRequest(){
    qDebug() << "Tokenize button pressed";
}

//////////////////////
/// Controller::run
/// Manages high-level control flow
///
void Controller::run(){

    /////obtain the text
    //this is the text we will be working on
    vector<string> text = vector<string>();

    if(getTextFromFile(text) != 0) exit(-1);

    for(auto& entry: text){
        cout << entry << endl;
    }

    LanguageProcessor processor = LanguageProcessor();
    vector<string> placeholder = vector<string>();
    //placeholder.push_back("Placeholder");
    //placeholder.push_back("text");
    //placeholder.push_back("is");
    //placeholder.push_back("good");
    //placeholder.push_back("for");
    //placeholder.push_back("now");

    ////TODO:tokenize the text
    if(processor.getTestCase(placeholder) != 0){
        exit(-1);
    }


    ////tag the text
    processor.getXML();
    processor.countTags();
    vector<pair<string,string>> tagResults = vector<pair<string,string>>();
    processor.tag(placeholder, tagResults);




    /*for(auto& entry: tagResults){
        cout << entry.first << " : " << entry.second << endl;
    }*/





    vector<pair<string,string>> dict = processor.getDict();

    /*for(auto& entry: dict){
        cout << entry.first << " : " << entry.second << endl;
    }*/

    ////TODO: topic analysis


    ////TODO: run the text through the technical dictionary


    //////classify the unidentified alphanumeric strings
    classifyAlpha("hi");

    ////TODO: classify word collocations


    ////TODO: classify the noun and verb phrases


    ////TODO: consolidate duplicate material-article types


    ////TODO: establish parent-child relationships


    ////TODO: write the findings

}







//////////////////////////////////
/// This code gets the compoents and uses Baysian learning to classify them
///
int Controller::classifyAlpha(string val){

    Repository repo = Repository();
    vector<Component*> collection = vector<Component*>();

    int i = 0;
    //vector<HCluster*> clusters = vector<HCluster*>();
    repo.getComponents(collection);

    BayesianStringClassifier bayes = BayesianStringClassifier();

    /////// For testing
    //crossValidate(bayes, collection);

    //map<string, float>* results = bayes.classify(testComp, collection);

    /*for(auto& entry: *results){
        cout << entry.first << " .. " << entry.second << endl;
    }*/

    return 0;

}


/////////////
/// getTextFromFile()
/// gets teh text from a file
///
int Controller::getTextFromFile(vector<string>& text){

    //open the file -- static location for now

    ifstream thefile("/home/ian/Data/testcase.txt");
    string line = "";
    if (thefile.is_open()) {
      while ( getline (thefile,line) ) {
          QString theLine = QString::fromStdString(line);
          QStringList pieces = theLine.split(' ');
          for(auto& entry: pieces){
              text.push_back(entry.toStdString());//QString::toStdString(entry));
          }
        //text.push_back(line);
      }

      thefile.close();
      return 0;
    }
    return -1;
}




///////////////
/// TOKENIZE
/// Tokenizes a text file given by fileName
///
///
int Controller::tokenize(string fileName){

    Tokenizer* tok = new Tokenizer(fileName);
    tok->tokenize();
    tok->writeToSQL();

    delete tok;

    return 0;
}

void Controller::crossValidate(BayesianStringClassifier& bayes, vector<Component*>& collection){
    Component* testComp = new Component("Bob's bolts", "CRCW040222R0FKED", "Some widget", "");
    vector<Component*> testing = vector<Component*>();
    vector<Component*>::iterator it;
    int right = 0;
    int wrong = 0;
    int i = 0;

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


    for(Component* c: testing){
        map<string, float>* results = bayes.classify(c, collection);
        auto choice = std::max_element(results->begin(), results->end(),
            [](const pair<string, float>& p1, const pair<string, float>& p2) {
                return p1.second < p2.second; });
        if((*choice).first.compare(c->type) == 0) right++; else wrong++;
            cout << "Right: " << right << " Wrong: " << wrong << endl;
        delete results;
    }

    delete testComp;
}

////////////////////////////////////////////////////////////////
///SCRAP CODE BELOW HERE


///////////////
/// CLUSTERING
/// Legacy code for now
/*
 *
 *     /*int INCLUSION_CRITERION = 4;
    int MAX_VARIANCE = 6;

    int MIN_SS = 3;

    float INCLUSION_RATIO = 0.33333;
    * /
 *
 *
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
