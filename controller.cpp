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
#include "topicanalyzer.h"
#include<string>

/******************************************************
 * Description of overall control flow:
 * Open a text case
 *   for each file:
 *       determine the topic                                                        --TODO: optimize
 *       scan for parents                                                           -- done
 *   for each line:
 *       tokenize (remove stop characters, split into words, fix capitalization)    -- done
 *       extract noun and verb phrases, put each into a collection                  -- done
 *       for each phrase:
 *           identify collocations based on augmented corpus                        -- TODO: tentatively works -- optimize parms, compile stop-words
 *               for each collocation:
 *                   associate with most likely material-article type in augmented corpus (subject to MIN_SUPPORT) (given the topic) --TODO: start
 *               for each remaining word:
 *                   associate with most likely material-article type in augmented corpus (subject to MIN_SUPPORT) (given the topic) --TODO: Bayesian not working
 *               for untagged words:
 *                   if candidate part number
 *                       associate with most likely material-article type based on bayesian classifier --done
 *                       associate with most likely suppleir based on Bayesian classifier --TODO: start (but can adapt the classifier)
 *               for each association:
 *                   collapse based on:
 *                       deduplication                                               --TODO: start
 *                       taxonomy                                                    --TODO: start
 *       for each association:
 *           determine a parent based on probability of material-article type having the parent in:
 *                   the file                                                        --TODO: start
 *                   the parent file                                                 --TODO: start
 *
 * Output:
 * ||Filename||Line item||Parent||MFR||MPN||Description||Material-article type||
 *
 ******************************************************/

//Controller::Controller(){

    //processor = LanguageProcessor();
    //tok = Tokenizer();
    //repo = Repository(tok);
    //top = TopicAnalyzer(repo);

//}

/*class testFile{
public:
    string filename;
    string text;
    vector<string> words;
    vector<string, string> tags;
    enum enums::TOPIC topic;
    testFile* parent;
    vector<string> nounPhrases;
    vector<string> verbPhrases;
};*/


///////////////
/// \brief Controller::runTestCase
/// \param tcNum - the test case number
/// Expects a test case in the following text format:
/// <filename> word1 word2 ... wordn \n
/// One line per file
///
/// Syntax of a data file:
///
///  Tokens are delimited by spaces
///     The first token is the filename
///   Fragments are delmited by tabs (this is primarily used for spreadsheet cells)
///   Lines are terminated by a ~ Tilde character
///   Files are delimited by a newline character
///
/// To create a test file:
///     - Create one or more text files and/or tab-delimited tables.  Use the .txt extension.
///     - Use pdftotext (from the popplertools library) to convert a pdf to text
///         pdftotext source.pdf target.txt
///     - Write the filenames and file contents to a single text file:
///         more * | cat > out.txt
///     - Replace newline characters with the DOT symbol (in this system, the ~ character)
///         sed -i 's/\n/ ~ /g' out.txt
///     - If the source of the document is a pdf, a double newline is the DOT symbol.
///         sed -t 's/~~/|/g' out.txt
///         tr '~' ' ' < out.txt >out2.txt
///         tr '|' '~' < out2.txt >out3.txt
///         mv out3.txt out.txt
///     - Replace the more command's 14-colon symbol
///         sed -i 's/~::::::::::::::~/\n/g' out.txt
///     - Replace the newline that follows a filename with a space
///         sed -i 's/.txt\n/.txt /g' testcase2.txt
///         (or just do it by hand with a text editor)
///
///
void Controller::runTestCase(int tcNum){


    string dir = "/home/ian/Data/Testcases/";
    dir += to_string(tcNum) + "/testcase.txt";

    vector<testFile*> files = vector<testFile*>();
    vector<string> text = vector<string>();

    vector<Component*> results = vector<Component*>();
    BayesianStringClassifier bayes = BayesianStringClassifier();

    vector<string>::iterator it;

    ifstream thefile(dir);
    string line = "";
    if (!thefile.is_open()){
        cout << "Cannot open " << dir << endl;
        return;
    }

    //else do the work
    //This part fills in the filename, <words collection>
    while ( getline (thefile,line) ) {
        text = vector<string>();
        QString theLine = QString::fromStdString(line);
        QStringList pieces = theLine.split(' ');

        for(auto& entry: pieces){
            tok.removeStopCharacters(entry);
            string s = entry.toLower().toStdString();
            text.push_back(s);//QString::toStdString(entry));
        }
        //text.push_back(".");
        testFile* tf = new testFile();
        tf->filename = text.front();
        for(it = text.begin() +1; it != text.end(); ++it){
            tf->words.push_back(*it);        }
        files.push_back(tf);
    }
    thefile.close();

    //determine the topic
    for(auto& entry: files){
        entry->topic = top.findTopic(entry->words, repo);
    }

    //scan for parents
    //if another file's f1 name is found in the text of a file f2, then f1's parent is f2
    for(auto& e1: files){
        for(auto& e2: e1->words){
            for(auto& e3: files){
                //TODO: strip off "txt" and ".pdf" from filenames
                if(e3 != e1 && e3->filename == e2){
                    e3->parent = e1;
                }
            }
        }
    }

    //tag the words
    processor.getXML();
    processor.countTags();
    for(auto& entry: files){
        processor.tag(entry->words, entry->tags);
    }


    //look for collocations
    const int SEARCHDIST = 4;
    vector<pair<string,string>> colls = vector<pair<string, string>>();
    //processor.findCollocationMetrics();
    getCollocationsFromDBDescriptions(colls);
    vector<string>::iterator it3, it4;
    for(auto& entry: files){
        for(it3 = entry->words.begin(); it3 != entry->words.end(); ++it3){
            if(*it3 != "~"){
                for(it4 = it3+1; it4 != entry->words.end() && it4 <= it3 + SEARCHDIST && *it4 != "~"; ++it4){
                    for(auto& e3: colls){
                        if(e3.first == *it3 && e3.second == *it4){
                            (*entry).collocations[e3] = "0";
                        }
                    }
                }
            }
        }
    }

    //classify collocations
    vector<Component*> components = vector<Component*>();
    repo.getComponentsIncludingGenerics(components);
    getCollocationsFromDBDescriptions(colls);
    vector<Component*> compsIncGenerics = vector<Component*>();
    repo.getComponentsIncludingGenerics(compsIncGenerics);

    vector<string> supplierNames = vector<string>();
    repo.getSupplierNames(supplierNames);

    for(auto& entry: files){
        for(auto& e2: entry->collocations){
            pair<string,string> coll = make_pair(e2.first.first, e2.first.second);
            map<string, float>* results = bayes.classifyCollocation(coll, compsIncGenerics);
            cout << endl << endl << "CLASSIFY COLLOCATION: <" << coll.first << ", " << coll.second << ">" << endl;
            if(results != 0){

                for(auto& e3: (*results)){
                    if(e3.second > 0) cout << e3.first << " : " << e3.second << endl;
                }
                delete results;

            } else cout << "Not found in corpus" << endl;

        }
    }


    //Tag manufacturer names
    for(auto& e1:files){
        for(auto& e2: e1->tags){
            if(e2.first == "~") e2.second = "DOT";
            else
            for(auto& e3: supplierNames){
                if(e3 == e2.first){
                    e2.second = "NNP type=MFR";
                }
            }
        }
    }

    //build noun phrases
    //build verb phrases
    for(auto& entry: files){
        processor.getNounPhrases(entry->tags, entry->nounPhrases);
        processor.getVerbPhrases(entry->tags, entry->verbPhrases);
    }


    //lookup mixed alphanumerics

    vector<Component*> collection = vector<Component*>();
    repo.getComponents(collection);
    bayes.learn(collection);


    //classify untagged words
    //TODO: remove words in collocations
    for(auto& e1: files){
        for(auto& e2: e1->tags){
            if(e2.second == "???"){
                map<string, float>* results = new map<string,float>();
                results = bayes.classify(e2.first, collection);
                if(results->size() > 0){
                    auto choice = std::max_element(results->begin(), results->end(),
                        [](const pair<string, float>& p1, const pair<string, float>& p2) {
                            return p1.second < p2.second; });
                    if(choice->second > 0.1)
                        e2.second = "NN type=" + choice->first;
                    else e2.second = "UNCLASSIFIED";
                }
                delete results;
            }
        }
    }

    //classify remaining mixed alphanumerics
    //match MPN and MFR and MFR - MPN matches
    //Deduplicate generics
    //build component hierarchy

    ////// Display results for testing purposes
    for(auto& entry: files){
        cout << endl << "FILE: " << entry->filename << "       TOPIC: " << enums::topicStrings[entry->topic] << " ....... " << endl;
        cout << "PARENT: " << (entry->parent == 0 ? "ROOT" : entry->parent->filename) << endl;
        for(auto& e2: entry->tags){
            cout << e2.first << "   " << e2.second << endl;
        }
        cout << "Collocations..." << endl;
        for(auto& e2: entry->collocations){
            cout << "<" << e2.first.first << ", " << e2.first.second << ">" << endl;
        }
        cout << "Noun Phrases..." << endl;
        for(auto& e2: entry->nounPhrases){
            for(auto& e3: (*e2)){
                cout << e3.first << "..";
            }
            cout << endl;
        }

        cout << "Verb Phrases..." << endl;
        for(auto& e2: entry->verbPhrases){
            for(auto& e3: (*e2)){
                cout << e3.first << "..";
            }
            cout << endl;
        }

    }

    /*for(auto& entry: files){
        cout << "..........." << endl << entry->filename << endl << "................" << endl;
        for(auto& e2: entry->tags){
            if(e2.second == "DOT") cout << endl;
            else cout << e2.first << " " << e2.second << " | ";
        }
    }*/


    //shutdown
    for(auto& entry: files) delete entry;

}

///////////////
/// \brief Controller::cleanTestCase
/// \param tcNum
/// Compiles the reuslts of the more * | cat to the format expected by runTestCase()
void Controller::cleanTestCase(int tcNum){
    /*
    string dir = "/home/ian/Data/Testcases/";
    dir += tcNum + "/testcase.txt";
    */
}



void Controller::handleTokenizeRequest(){
    qDebug() << "Tokenize button pressed";
}






////////////////////////////////////////////////////////////////
/////
///   UNIT TEST FUNCTIONS
///         Unit testing is supported for:
///             - Topic Extraction
///             - Classifying a mixed-character alphanumeric string
///             - Finding quasi-word collocations
///             - Classifying a quasi-word collocation
///             - Finding noun phrases in free text
///             - Finding verb phrase fragments in free text
///             - Classifying the supplier of an identified part number
///             - Entity deduplication
/////
///////////////////////////////////////////////////////////////

//TODO: broken
int Controller::testTopicExtraction(){
    /////obtain the text ..............................................
    ////tokenize the text .............................................
    //This collection is intended for use by fully implemented application
    vector<vector<string>*> testingSet = vector<vector<string>*>();

    //temporary text to test functions
    vector<string> text = vector<string>();

    cout << "Get test case" << endl;

    //if(getTextFromFile(text, tok) != 0) exit(-1);        // <--- USE THIS TO GET TEST CASE 1
    if(getTestCase2(text, tok) != 0) exit(-1);             // <--- USE THIS TO GET TEST CASE 2
    //if(getTestCase3(text, tok) != 0) exit(-1);           // <--- USE THIS TO GET TEST CASE 3

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

}

///// NOT THE TEST CASE -- placeholder
int Controller::testClassifyingString(){
    cout << "Words from material dictionary -- not the test case" << endl;
    repo.getWordsFromMaterialDictionary();

}

 // Finding quasi-word collocations

int Controller::testFindCollocations(){
    vector<pair<string,string>> colls = vector<pair<string,string>>();
    BayesianStringClassifier bayes = BayesianStringClassifier();
    vector<Component*> components = vector<Component*>();
    repo.getComponentsIncludingGenerics(components);
    getCollocationsFromDBDescriptions(colls);

    for(auto& entry: colls){
        map<string, float>* results = bayes.classifyCollocation(entry, components);

        cout << endl << endl << "CLASSIFY COLLOCATION: <" << entry.first << ", " << entry.second << ">" << endl;
        if(results != 0){

            for(auto& e2: (*results)){
                if(e2.second > 0) cout << e2.first << " : " << e2.second << endl;
            }
            delete results;

        } else cout << "Not found in corpus" << endl;

    }


}

//////
/// \brief Controller::testClassifyCollocations
/// \return success or failure
///
/// Test case for classifying some arbitrary quasi-word collocations
///
int Controller::testClassifyCollocations(){
    vector<pair<string, string>> testVector = vector<pair<string, string>>();
    BayesianStringClassifier bayes = BayesianStringClassifier();
    vector<Component*> components = vector<Component*>();
    repo.getComponentsIncludingGenerics(components);

    //the test cases
    testVector.push_back(make_pair("stainless", "steel"));
    testVector.push_back(make_pair("zinc", "blue"));
    testVector.push_back(make_pair("res", "0603"));
    testVector.push_back(make_pair("15", "pos"));
    testVector.push_back(make_pair("washer", "ss"));
    testVector.push_back(make_pair("fh", "zn"));
    testVector.push_back(make_pair("1206", "x5r"));
    testVector.push_back(make_pair("astm", "967"));
    testVector.push_back(make_pair("rubber", "bumper"));

    for(auto& entry: testVector){
        map<string, float>* results = bayes.classifyCollocation(entry, components);

        cout << endl << endl << "CLASSIFY COLLOCATION: <" << entry.first << ", " << entry.second << ">" << endl;
        if(results != 0){

            for(auto& e2: (*results)){
                if(e2.second > 0) cout << e2.first << " : " << e2.second << endl;
            }
            delete results;

        } else cout << "Not found in corpus" << endl;

    }


}

//todo: broken
int Controller::testNounPhrases(){

    //temporary text to test functions
    vector<string> text = vector<string>();

    cout << "Get test case" << endl;

    //if(getTextFromFile(text, tok) != 0) exit(-1);        // <--- USE THIS TO GET TEST CASE 1
    if(getTestCase2(text, tok) != 0) exit(-1);             // <--- USE THIS TO GET TEST CASE 2
    //if(getTestCase3(text, tok) != 0) exit(-1);           // <--- USE THIS TO GET TEST CASE 3

    ////tag the text .................................................
    cout << "Tagging words" << endl;

    processor.getXML();
    processor.countTags();
    vector<pair<string,string>> tagResults = vector<pair<string,string>>();
    processor.tag(text, tagResults);

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

    //this collection will hold the words we are analyzing
    vector<string> dwgText = vector<string>();

    //function that retrieves the test case
    getTestCase2(dwgText, tok);

    ////////// Code that can be used to open training text for testing instead
    //repo.getAllDwgTextFromDB(dwgText);
    //repo.getAllDescriptionsFromDB(dwgText);

    ////// Extracts noun and verb phrases from the free text in dwgText

    processor.tag(dwgText, tagResults);
    processor.getNounPhrases(tagResults, nPhrases);

    for(auto& entry: nPhrases){
        cout << "NOUN PHRASE : " << endl;
        for(auto& e2: (*entry)){
            cout << e2.first << endl;
        }
        cout << endl;
    }

    return 0;

}

//todo: broken
int Controller::testVerbPhrases(){
    //temporary text to test functions
    vector<string> text = vector<string>();

    //if(getTextFromFile(text, tok) != 0) exit(-1);        // <--- USE THIS TO GET TEST CASE 1
    if(getTestCase2(text, tok) != 0) exit(-1);             // <--- USE THIS TO GET TEST CASE 2
    //if(getTestCase3(text, tok) != 0) exit(-1);           // <--- USE THIS TO GET TEST CASE 3


    ////tag the text .................................................
    cout << "Tagging words" << endl;

    processor.getXML();
    processor.countTags();
    vector<pair<string,string>> tagResults = vector<pair<string,string>>();
    processor.tag(text, tagResults);

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

    processor.tag(dwgText, tagResults);
    processor.getVerbPhrases(tagResults, vPhrases);

    for(auto& entry: vPhrases){
        cout << "VERB PHRASE : " << endl;
        for(auto& e2: (*entry)){
            cout << e2.first << endl;
        }
        cout << endl;
    }

    return 0;

}

int Controller::testClassifySupplier(){return 0;} // Classifying the supplier of an identified part number

int Controller::testEntityDeduplication(){return 0;} //Entity deduplication

void Controller::testParent(){
    map<string, float> res = map<string, float>();
    cout << "Test parents" << endl;
    string t = "Subassembly";
    repo.getParentTypes(res, t);
}


//////////////////////////////////
/// This code gets the compoents and uses Baysian learning to classify them
///
int Controller::classifyAlpha(string val){

    //Repository repo = Repository();
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
/// Used to otain the text from testcase.txt
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

///////////////
/// \brief Controller::crossValidate
/// \param bayes
/// \param collection
/// Used when performing cross-validation to test the Bayesian component classifier
///
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

/////////
/// \brief Controller::getCollocationsFromDBDescriptions
/// Used during development to find the word collocations from the training set
void Controller::getCollocationsFromDBDescriptions(vector<pair<string, string>>& coll){

    vector<string> allDescriptions = vector<string>();
    map<string, int> singles = map<string, int>();
    map<pair<string,string>, int> pairs = map<pair<string,string>, int>();
    vector<pair<string,string>> collocations = vector<pair<string,string>>();

    //repo.getAllDescriptionsFromDB(allDescriptions);
    repo.getContractsComponentsDescriptionsFromDB(allDescriptions);

    //for(auto& entry: allDescriptions) cout << entry << endl;

    processor.findCollocationMetrics(allDescriptions, singles, pairs, tok);

    //for(auto& entry: singles) cout << entry.first << endl;
    //for(auto& entry: pairs) cout << entry.first.first << " , " << entry.first.second << endl;

    processor.mimForCollocations(singles, pairs, collocations);

    cout << "Getting Collocations From DB ............" << endl;
    for(auto& entry: collocations){
        //cout << "<" << entry.first << "," << entry.second << ">" << endl;
        coll.push_back(entry);
    }


}


////////////////////////////////////////////////////////////////
///LEGACY CODE BELOW HERE
////////////////////////////////////////////////////////////////

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
    if(getTestCase2(text, tok) != 0) exit(-1);             // <--- USE THIS TO GET TEST CASE 2
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
