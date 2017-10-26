#include "controller.h"
#include "QDebug"
#include "languageprocessor.h"
#include "repository.h"
#include "component.h"
#include "bayesianstringclassifier.h"
#include "tokenizer.h"
#include "QStringList"
#include "QString"
#include "unordered_set"

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

    LanguageProcessor processor = LanguageProcessor();
    Tokenizer tok = Tokenizer();
    Repository repo = Repository();

    /////obtain the text ..............................................
    //this is the text we will be working on
    vector<string> text = vector<string>();

    //if(getTextFromFile(text, tok) != 0) exit(-1);
    if(getTestCase2(text, tok) != 0) exit(-1);

    cout << "TEXT" << endl << ".........................." << endl;
    for(auto& entry: text){
        cout << entry << endl;
    }

    ////TODO:tokenize the text .......................................


    ////tag the text .................................................
    processor.getXML();
    processor.countTags();
    vector<pair<string,string>> tagResults = vector<pair<string,string>>();
    processor.tag(text, tagResults);

    /*cout << "Tag results" << endl << "....................................." << endl;
    for(auto& entry: tagResults){
        cout << entry.first << " : " << entry.second << endl;
    }*/

/////////////////////////// SQL ERROR BELOW HERE /////////////////////////////////

    ////TODO: topic analysis .........................................


    ////TODO: run the text through the technical dictionary ..........
    ///     This will idenify numbers, etc. that weare interested in.
    ///     Needs to run after the tagging from corpus -- will include special tags
    //processor.openTechDictionary(repo);
    //processor.applyTechDictionary(tagResults);


    //////classify the unidentified alphanumeric strings ..............
    //classifyAlpha("hi");

    ////Parse the noun and verb phrases ......................
    // Test vector

///////////////////////////// SQL ERROR ABOVE HERE

    vector<pair<string,string>> demo = vector<pair<string,string>>();
    demo.push_back(make_pair("awesome","JJ"));
    demo.push_back(make_pair("far","NN"));
    demo.push_back(make_pair("out","IN"));
    demo.push_back(make_pair("group","NN"));
    demo.push_back(make_pair("is","VV"));
    demo.push_back(make_pair("a","DT???"));
    demo.push_back(make_pair("super","JJ"));
    demo.push_back(make_pair("nice","JJ"));
    demo.push_back(make_pair("noun","NN"));
    demo.push_back(make_pair("phrase","NN"));
    demo.push_back(make_pair("also","CC"));
    demo.push_back(make_pair("I","PRP"));
    demo.push_back(make_pair("like","VB"));
    demo.push_back(make_pair("climbing","VBG"));
    demo.push_back(make_pair("on","IN"));
    demo.push_back(make_pair("red","JJ"));
    demo.push_back(make_pair("stairs","NN"));

    vector<vector<pair<string, string>>*> nPhrases = vector<vector<pair<string, string>>*>();
    vector<vector<pair<string, string>>*> vPhrases = vector<vector<pair<string, string>>*>();

   vector<string> dwgText = vector<string>();

    //repo.getAllDwgTextFromDB(dwgText);

    repo.getAllDescriptionsFromDB(dwgText);

    vector<string> myWords = vector<string>();

    for(auto& entry: dwgText){
        QStringList pieces = QString::fromStdString(entry).split(' ');
        for(auto& e2: pieces){
            myWords.push_back(e2.toLower().toStdString());
        }
    }

    tagResults = vector<pair<string,string>>();
    processor.tag(myWords, tagResults);

    unordered_set<string> unknownWords = unordered_set<string>();

    for(auto& entry: tagResults){
        //cout << entry.first << " : " << entry.second << endl;

        if(entry.second == "???"){
            cout << entry.first << endl;
            unknownWords.insert(entry.first);
        }
    }

    ofstream outfile;
    outfile.open ("/home/ian/Data/unknownFromDBDescField.txt");
    for(auto& entry: unknownWords){
        outfile << entry << endl;
    }
    outfile.close();




    /*for(int i = 0; i < dwgText.size(); ++i){

        //cout << "-----------------" << endl << dwgText.at(i) << endl << "-----------------" << endl << endl;

        //cout << *dwgText.at(i) << endl;

        processor.tag(*dwgText.at(i), tagResults);

        processor.dumpUnknownWords(tagResults, "unknown");

        processor.getNounPhrases(tagResults, nPhrases);
        processor.getVerbPhrases(tagResults, vPhrases);

        for(auto& entry: nPhrases){
            cout << "NOUN PHRASE : " << endl;
            for(auto& e2: (*entry)){
                cout << e2.first << endl;
            }
            cout << endl;
        }

        for(auto& entry: vPhrases){
            cout << "VERB PHRASE : " << endl;
            for(auto& e2: (*entry)){
                cout << e2.first << endl;
            }
            cout << endl;
        }
    }*/

    ////TODO: scan noun and verb phrases forword collocations .........


    ////TODO: consolidate duplicate material-article types ............


    ////TODO: establish parent-child relationships ....................


    ////TODO: write the findings ......................................
    //for now, just print the final collection.
    //print text for debug
    /*for(auto& entry: tagResults){
        cout << entry.first << " : " << entry.second << endl;
    }*/

}




//////////////////////////////////
/// This code gets the compoents and uses Baysian learning to classify them
///
int Controller::classifyAlpha(string val){

    Repository repo = Repository();
    vector<Component*> collection = vector<Component*>();

    int i = 0;
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


//////////
/// \brief Controller::getTestCase2
/// \param text
/// \return gets all the text from a big test case
///
int Controller::getTestCase2(vector<string>& text, Tokenizer& tok){
    ifstream thefile("/home/ian/Data/tc2.txt");
    string line = "";
    string s = "";
    if (thefile.is_open()) {
      while ( getline (thefile,line) ) {
          QString theLine = QString::fromStdString(line);
          QStringList pieces = theLine.split(' ');
          for(auto& entry: pieces){
              tok.removeStopCharacters(entry);
              string s = entry.toLower().toStdString();
              /*if(s.length() > 0 && (s.at(0) >= 'a' && s.at(0) <= 'z') || (s.at(0) >= 'A' && s.at(0) <= 'Z') || (s.at(0) >= '1' && s.at(0) <= '0') )*/
              cout << "String " << s << " length: " << s.length() << endl;
              if(s.length() != 0) text.push_back(s);
          }
      }


      text.push_back(s);

      thefile.close();
      return 0;
    }
    return -1;
}


/////////////
/// getTextFromFile()
/// gets the text from a file
///
///
///
///
int Controller::getTextFromFile(vector<string>& text, Tokenizer& tok){

    //open the file -- static location for now

    ifstream thefile("/home/ian/Data/testcase.txt");
    string line = "";
    if (thefile.is_open()) {
      while ( getline (thefile,line) ) {
          QString theLine = QString::fromStdString(line);
          QStringList pieces = theLine.split(' ');
          for(auto& entry: pieces){
              tok.removeStopCharacters(entry);
              string s = entry.toStdString();
              text.push_back(s);//QString::toStdString(entry));
          }
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
    //tok->writeToSQL();

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
