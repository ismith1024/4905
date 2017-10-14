#include "languageprocessor.h"
#include"QString"
#include"QStringList"


LanguageProcessor::LanguageProcessor(){
    dict = new vector<pair<string, string>>();
    tagCounts = new map<string, map<string, int>>();
}

LanguageProcessor::~LanguageProcessor(){
    delete dict;
    delete tagCounts;
}

vector<pair<string,string>>& LanguageProcessor::getDict(){
    return *dict;
}


string LanguageProcessor::getTag(string s){
    map<string, int> vals = tagCounts->operator[](s);
    if(vals.size() == 0) return "???";

    auto largest = std::max_element(vals.begin(), vals.end(),
        [](const pair<string, int>& p1, const pair<string, int>& p2) {
            return p1.second < p2.second; });

    return (*largest).first;
}

//////TODO: search the corpus and build the tag counts
void LanguageProcessor::tag(vector<string>& inputText, vector<pair<string, string>>& outText){
    for(auto& word: inputText){
        string str = getTag(word);
        outText.push_back(make_pair(word, str));
    }
}

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
                      cout << s2 << " : " << s1 << endl;
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
