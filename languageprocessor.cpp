#include "languageprocessor.h"



LanguageProcessor::LanguageProcessor(){
    dict = new vector<pair<string, string>>();
    tagCounts = new map<string, map<string, int>>();
}

LanguageProcessor::~LanguageProcessor(){
    delete dict;
    delete tagCounts;
}

//////
/// Dictionary getter method
vector<pair<string,string>>& LanguageProcessor::getDict(){
    return *dict;
}

///////
/// \brief LanguageProcessor::countTags
/// \param tagcounts - the counts of the tags in the dictionary
void LanguageProcessor::countTags(){
    for(auto& entry: (*dict)){
        (*tagCounts)[entry.first][entry.second]++;
    }
}

///////
/// \brief LanguageProcessor::getTag
/// \param a string to check the tag
/// \return the tag
///
/// NOTE:   This returns the most frequently occurring tag from the corpus.
/// TODO:   Investigate a better approach (if time permits).
///
string LanguageProcessor::getTag(string s){
    map<string, int> vals = tagCounts->operator[](s);
    if(vals.size() == 0) return "???";

    auto largest = std::max_element(vals.begin(), vals.end(),
        [](const pair<string, int>& p1, const pair<string, int>& p2) {
            return p1.second < p2.second; });

    return (*largest).first;
}

////////
/// \brief LanguageProcessor::tag
/// \param collection of text - the phrase being tagged
/// \param collection of tagged text in <string, tag> pairs
///
void LanguageProcessor::tag(vector<string>& inputText, vector<pair<string, string>>& outText){
    for(auto& word: inputText){
        string str = getTag(word);
        outText.push_back(make_pair(word, str));
    }
}

///////
/// \brief LanguageProcessor::getXML
/// \return Error code
/// Opens the Brown Corpus XML file, parses the XML, and populates the dict collection
///
int LanguageProcessor::getXML(){
    //open the XML file -- static location for now
    //string loc = "/home/ian/Data/Corpus.xml";

    //int count = 0;
    ifstream xmlfile("/home/ian/Data/Corpus.xml");
    string line = "";
    if (xmlfile.is_open()) {
      while ( getline (xmlfile,line) ) {
          QString qs = QString::fromStdString(line);
          QStringList pieces = qs.split("<");


          for(QString st: pieces){
              if(st.size() > 0 && st.at(0) == 'w'){
                  QStringList pieces2 = st.split('>');

                  if(pieces2.size() > 1 ){
                      QString s = pieces2.at(0);
                      string s1 = s.replace("w type=\"", "").replace("\"","").toStdString();
                      string s2 = pieces2.at(1).toStdString();
                      //count++;
                      //cout << s2 << " : " << s1 << endl;
                      dict->push_back(make_pair(s2, s1));
                  }
              }

          }

      }

      xmlfile.close();
      //cout << "Count: " << count << endl;
      return 0;
    }

    else {
        cout << "Unable to open file " << "/home/ian/Data/Corpus.xml" << endl;
        return -1;
    }

}

////////
/// \brief LanguageProcessor::getTestCase
/// gets the test case file from \home\ian\Data\testcase.txt
/// \return error code or 0
///
int LanguageProcessor::getTestCase(vector<string>& coll){

    ifstream infile("/home/ian/Data/testcase.txt");
    string line = "";
    if(infile.is_open()){
        while(getline(infile, line)){
            coll.push_back(line);
        }
        return 0;
    }

    cout << "Unable to open \home\ian\Data\testcase.txt" << endl;
    return -1;
}
