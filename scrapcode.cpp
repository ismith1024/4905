#include "controller.h"
#include "QDebug"
#include "languageprocessor.h"
#include "repository.h"
#include "component.h"
#include "bayesianclassifier.h"
#include "tokenizer.h"
#include "QStringList"
#include "QString"
#include "unordered_set"
#include "topicanalyzer.h"
#include<string>


////////////////////////////////////////////////////////////////
///LEGACY CODE BELOW HERE
////////////////////////////////////////////////////////////////
///
///
///

/////////////////// Test case for clustering on Lev Dist
///
/*void Controller::runOneOf(){
int INCLUSION_CRITERION = 2;
int MAX_VARIANCE = 4;
int MIN_SS = 3;
float INCLUSION_RATIO = 0.33333;

vector<Component*> collection;
vector<HCluster*> clusters;
repo.getComponents(collection);

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

float totalComps = 0.0;
int minorityComps = 0;
int singletonComps = 0;
int majoritycomps = 0;

float classCount = 0.0;

for(HCluster* clus: clusters){

    int sz = clus->numCategories();
    classCount += sz;
    cout << "Cluster -- size: " << clus->numEntries() << "   Number of categories: " << sz << endl;
    /*if(sz >= 10){
        cout << "Large category:" << endl;
        clus->dumpComponents();
    }* /

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
cout << "Max dist for inclusion: " << INCLUSION_CRITERION << "    Max Diameter for exclusion: " << MAX_VARIANCE << endl;
cout << "Found " << clusters.size() << " clusters." << endl;
cout << "Average classes per cluster: " << classCount / clusters.size() << endl;
cout << "Average Components per cluster" << totalComps / clusters.size() << endl;

cout << "Total components: " << totalComps << endl << "Singleton components: " << singletonComps << endl << "Minority Components: " << minorityComps << endl << "Majority components: " << majoritycomps << endl;
cout << "---------------------" << endl << endl;
//cout<< "TEST" << endl << "593D476X9020D2TE3" << " " << "BLM18PG181SH1D" << endl;
//cout << UtilityAlgorithms::levDist("593D476X9020D2TE3", "BLM18PG181SH1D");
}*/

/*

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
              /*if(s.length() > 0 && (s.at(0) >= 'a' && s.at(0) <= 'z') || (s.at(0) >= 'A' && s.at(0) <= 'Z') || (s.at(0) >= '1' && s.at(0) <= '0') )* /
              //cout << "String " << s << " length: " << s.length() << endl;
              if(s.length() != 0) text.push_back(s);
          }
      }


      text.push_back(s);

      thefile.close();
      return 0;
    }
    return -1;
}


//////////
/// \brief Controller::getTestCase3
/// \param text
/// \return gets all the text from a big test case
///
int Controller::getTestCase3(vector<string>& text, Tokenizer& tok){
    ifstream thefile("/home/ian/Data/tc3.txt");
    string line = "";
    string s = "";
    if (thefile.is_open()) {
      while ( getline (thefile,line) ) {
          QString theLine = QString::fromStdString(line);
          QStringList pieces = theLine.split(' ');
          for(auto& entry: pieces){
              tok.removeStopCharacters(entry);
              string s = entry.toLower().toStdString();
              /*if(s.length() > 0 && (s.at(0) >= 'a' && s.at(0) <= 'z') || (s.at(0) >= 'A' && s.at(0) <= 'Z') || (s.at(0) >= '1' && s.at(0) <= '0') )* /
              cout << "String " << s << " length: " << s.length() << endl;
              if(s.length() != 0) text.push_back(s);
          }
      }

      text.push_back(s);

      thefile.close();
      return 0;
    }
    return -1;
}*/


/////////////////
/// \brief BayesianClassifier::classifySupplierOld
/// \return
/// Similar to matieral-article classifier:
/// Determines the probability of a manufacturer for an arbitrary alphanumeric string
///
/// Pr(supplier | trigram) = Pr(trigram | supplier) * Pr(supplier) / Pr(trigram)
/// RETURNS NULL IF INVALID WHICH MUST BE HANDLED
///
/*map<string, float>* BayesianClassifier::classifySupplier_Old(string& mpn, vector<Component*>& components){

    if(mpn.length() < 3) return 0;

    auto trigrams = vector<string>();
    auto ret = new map<string,float>();

    /*string::iterator it, it2;
    for(it = mpn.begin(); it != mpn.end() -3; ++it){

    }* /
    for(int i = 0; i < mpn.length() - 2; ++i){
        trigrams.push_back(mpn.substr(i,3));
    }

    for(auto& entry: trigrams){
        cout << entry << endl;
    }


    map<string, float> prTrigs = map<string, float>();

    for(auto& trig: tragrams){

        prTrigs = map<string,float>();

        //Pr(supplier | trigram)
        int count = 0;
        map<string, float> prSuppGivenTrig = map<string, float>();

        //prTrigs
        for(auto& comp: comps){
            std::size_t found = e2->mpn.find(entry);
            if(found != string::npos)
               prTrigs[trig]++;
        }

        int sz = components.size();

        for(auto& entry: prTrigs){
            entry.second /= sz;
        }

        //pr supp | trig
        for(auto& e2: components){

            std::size_t found = e2->mpn.find(entry);
            if(found != string::npos){
               prSuppGivenTrig[e2->mfr]++;
               count++;
           }
        }



        for(auto& entry: prSuppGivenTrig){
            entry.second /= count;

            //if the working return map doesn't contain the supplier, set it to 1.0
            map<string,float>::iterator it = ret->find(entry.first);
            if(it != ret->end()) ret->operator[](entry.first) = 1.0;

            //conjjugate probability multiplied by the conjugate of the trigram
            ret->operator[](entry.first) *= (1.0 - entry.second) / (prTrigs[trig]);
        }

        //pr(supplier)
        map<string, float> prSupp = map<string, float>();
        for(auto& entry: components){
            prSupp[entry->mfr]++;
        }

        int sz = components.size();

        for(auto& entry: prSupp){
            entry.second /= sz;
        }

        //complete bayes theorem
        //not right
        for(auto& entry: (*ret)){
            entry.second *= prSupp[entry.first];
        }

    }

    return ret;

}*/


/*
map<string, float>* BayesianStringClassifier::classify2(Component* comp, vector<Component*>& components){

    //get the substrings in the component
        vector<string> substrings = vector<string>();
        for(int i = 0; i < comp->mpn.size() -2; ++i){
            substrings.push_back(comp->mpn.substr(i, i+2));
        }

    //Probability of type
        map<string, float> probType = map<string, float>();

        for(Component* c: components){
            probType[c->type]++;
        }

        for(const auto& entry: probType){
            probType[entry.first] = entry.second / probType.size();
        }


   //probability of substring

        map<string, float> probSubs = map<string, float>();
        for(Component* c: components){
            for(string str: substrings){
                if(c->mpn.find(str) != string::npos) probSubs[str]++;
            }
        }

        for(auto& entry: probSubs){
            probSubs[entry.first] = entry.second / probSubs.size();
        }


    //Probability of the substring given type
        map<pair<string, string>, float> probSubsGivenType = map<pair<string, string>, float>(); //first is type and second is substring

        for(Component* c: components){
            for(auto& entry: probSubs){
                probSubsGivenType[make_pair(c->type, entry.first)]++;
            }
        }

        //divide by the number of components with this type
        for(auto& entry: probSubsGivenType){
            probSubsGivenType[entry.first] = probType[entry.first.first] * probType.size(); // probSubsGivenType[entry] /= (probType[entry.first] * probType.size());
        }

     //return value:
        /*
        Pr(component is a type) = ~(Pr(component is not a type)) = 1 - Product_k=0_n_(Pr(component is not a type given substring_k)) for n substrings
        * /

        map<string, float>* ret = new map<string,float>();

        //apply Bayes' theorem: probability of type given substring
        for(auto& typeentry: probType){
            (*ret)[typeentry.first] = 1.0;
            for(string subs: substrings){
                //remove any substring that is not found anywhere
                bool stringunknown = true;
                for(auto& entry: probSubs){
                    if(probSubs[entry.first] > 0.0) stringunknown = false;
                }

                if(stringunknown) continue;
                else
                    (*ret)[typeentry.first] *= probSubsGivenType[make_pair(typeentry.first, subs)] * probType[typeentry.first] / probSubs[subs];
            }
        }

     return ret;

} */


/*
/////////
/// \brief LanguageProcessir::findCollocationMetrics
/// \param inStrings - collection of space-delimited words  -- ["Hi there", "It's a nice day", ... "Bye for now!"]
/// \param singles   - occurrances of single words : <foo, x>
/// \param pairs     - occurrances of word pairs: <<bar, baz>, y>
/// Finds the metrics that will be used by Mutula Information Measure to identify word collocations
void LanguageProcessor::findCollocationMetrics(vector<string>& inStrings, map<string, int>& singles, map<pair<string,string>, int>& pairs){*/


////////////////
//// \brief Controller::obtainUntaggedWords
//// \param tegResults
/// Legacy function to idenify the untagged words in the training set
/// Used to build technical corpus
/*void Controller::obtainUntaggedWords(vector<string>& tegResults){
        vector<string> myWords = vector<string>();
        vector<QString> myWords2 = vector<QString>();

        for(auto& entry: dwgText){
            QStringList pieces = QString::fromStdString(entry).split(' ');
            for(auto& e2: pieces){
                if(!processor.containsNumbers(e2)){
                    e2.replace('/', ',');
                    QStringList p2 = e2.split(", _+");
                    for(auto& e3: p2) {
                        //cout << "Split: " << e3.toStdString() << endl;
                        myWords2.push_back(e3);
                    }
                } else {
                    tok.removeStopCharacters(e2);
                    myWords.push_back(e2.toLower().toStdString());
                }
            }
        }

        for(auto& entry: myWords2){
             tok.removeStopCharacters(entry);
             myWords.push_back(entry.toLower().toStdString());
        }

        tagResults = vector<pair<string,string>>();
        processor.tag(myWords, tagResults);

        unordered_set<string> unknownWords = unordered_set<string>();
        unordered_set<string> knownWords = unordered_set<string>();

        for(auto& entry: tagResults){
            //cout << entry.first << " : " << entry.second << endl;

            if(entry.second == "???"){
                //cout << entry.first << endl;
                unknownWords.insert(entry.first);
            } else{
                knownWords.insert(entry.first + " : " + entry.second);
            }
        }

        ofstream outfile;
        outfile.open ("/home/ian/Data/unknownFromDBDescField.txt");
        outfile << " --------------- UNKNOWN WORDS --------------- " << endl;
        for(auto& entry: unknownWords){
            outfile << entry << endl;
        }
        outfile << " --------------- KNOWN WORDS --------------- " << endl;
        for(auto& entry: knownWords){
            outfile << entry << endl;
        }
        outfile.close();

} */


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

///////////////////// DEMO TAGGED TEXT
/*vector<pair<string,string>> demo = vector<pair<string,string>>();
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
demo.push_back(make_pair("stairs","NN"));*/


// Outline of control flow
/* Open a text case
vector<testFile*> inputText = vector<testFile*>();
getTestCase(inputText);

//*   for each file:
//*       determine the topic                                                        --TODO: optimize
top.findTopic(inputText, repo);

//*       scan for parents                                                           --TODO: start
for(auto& current: inputText){
    for(auto& other: inputText){
        if(other != current && find(*current->text.begin(), *current->text.end(), *other->filename) != *current->text.end()){
            current->parent = other->filename;
        }
    }
}
//*   for each line:
//*       tokenize (remove stop characters, split into words, fix capitalization)    -- done
//*       extract noun and verb phrases, put each into a collection                  -- done
for(auto& entry: inputText){
    processor.getNounPhrases(
}

//*       for each phrase:
//*           identify collocations based on augmented corpus                        -- TODO: optimize parms, compile stop-words
//*               for each collocation:
//*                   associate with most likely material-article type in augmented corpus (subject to MIN_SUPPORT) (given the topic) --TODO: start
//*               for each remaining word:
//*                   associate with most likely material-article type in augmented corpus (subject to MIN_SUPPORT) (given the topic) --TODO: Bayesian not working
//*               for untagged words:
//*                   if candidate part number
//*                       associate with most likely material-article type based on bayesian classifier --done
//*                       associate with most likely suppleir based on Bayesian classifier --TODO: start (but can adapt the classifier)
//*               for each association:
//*                   collapse based on:
//*                       deduplication                                               --TODO: start
//*                       taxonomy                                                    --TODO: start
//*       for each association:
//*           determine a parent based on probability of material-article type having the parent in:
//*                   the file                                                        --TODO: start
//*                   the parent file                                                 --TODO: start
//*
//* Output:
//* ||Filename||Line item||Parent||MFR||MPN||Description||Material-article type|| * /




}*/

//////////////////////
/// Controller::run
/// This was the driver prior to implementation of GUI panel
///
void Controller::run(){

    /////obtain the text ..............................................
    ////tokenize the text .............................................
    //This collection is intended for use by fully implemented application
    vector<vector<string>*> testingSet = vector<vector<string>*>();

    //temporary text to test functions
    vector<string> text = vector<string>();
    //TODO: get a full test case
    // For now:
    vector<string>* forNow1 = new vector<string>();
    forNow1->push_back("Placeholder");
    forNow1->push_back("Text");
    vector<string>* forNow2 = new vector<string>();
    forNow2->push_back("More");
    forNow2->push_back("Placeholder");
    forNow2->push_back("Text");
    testingSet.push_back(forNow1);
    testingSet.push_back(forNow2);


    cout << "Get test case" << endl;

    //if(getTextFromFile(text, tok) != 0) exit(-1);        // <--- USE THIS TO GET TEST CASE 1
    //if(getTestCase2(text, tok) != 0) exit(-1);             // <--- USE THIS TO GET TEST CASE 2
    //if(getTestCase3(text, tok) != 0) exit(-1);           // <--- USE THIS TO GET TEST CASE 3

    //cout << "TEXT:" << endl;
    //for(auto& entry: text) cout << entry << endl;

    ////tag the text .................................................
    cout << "Tagging words" << endl;

    processor.getXML();
    processor.countTags();
    vector<pair<string,string>> tagResults = vector<pair<string,string>>();
    processor.tag(text, tagResults);

    ////topic analysis ..............................................

    cout << "TOPIC ANLYSIS..." << endl;
    cout << "Print topic words" << endl;
    //top.printTopicWords();
    enum enums::TOPIC topic = top.findTopic(text, repo);
    cout << "Topic: " << topic << endl;

    /*
    for(auto& entry: testingSet){
        currentTopic = top.findTopic(*entry);
        //do some stuff
    }*/

    exit(0);

/////////////////////////// SQL ERROR AFTER HERE  ----> /////////////////////////////////

    ////TODO: run the text through the technical dictionary ..........
    ///     This will idenify numbers, etc. that weare interested in.
    ///     Needs to run after the tagging from corpus -- will include special tags
    //processor.openTechDictionary(repo);
    //processor.applyTechDictionary(tagResults);


    //////classify the unidentified alphanumeric strings ..............
    //classifyAlpha("hi");

///////////////////////////// <---- SQL ERROR BEFORE HERE


    ////Parse the noun and verb phrases ......................
    vector<vector<pair<string, string>>*> nPhrases = vector<vector<pair<string, string>>*>();
    vector<vector<pair<string, string>>*> vPhrases = vector<vector<pair<string, string>>*>();

    //this collection will hold the words we are analyzing
    vector<string> dwgText = vector<string>();

    //function htat retrieves the test case
    getTestCase2(dwgText, tok);

////////// Code that can be used to open training text for testing instead
    //repo.getAllDwgTextFromDB(dwgText);
    //repo.getAllDescriptionsFromDB(dwgText);

/////////// Legacy code that was used to obtain untagged words from the training set
    //obtainUntaggedWords(tagResults);


////// Extracts noun and verb phrases from the free text in dwgText

    //for(auto& item: dwgText){ //int i = 0; i < dwgText.size(); ++i){

        //cout << "-----------------" << endl << dwgText.at(i) << endl << "-----------------" << endl << endl;

        //cout << *dwgText.at(i) << endl;

        processor.tag(dwgText, tagResults);

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
    //}

    ////TODO: scan noun and verb phrases for word collocations ........


    ////TODO: consolidate duplicate material-article types ............


    ////TODO: establish parent-child relationships ....................


    ////TODO: write the findings ......................................
    //for now, just print the final collection.
    //print text for debug
    /*for(auto& entry: tagResults){
        cout << entry.first << " : " << entry.second << endl;
    }*/

}
