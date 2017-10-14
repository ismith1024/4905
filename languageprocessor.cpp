#include "languageprocessor.h"
#include"QString"
#include"QStringList"


LanguageProcessor::LanguageProcessor(){
    dict = new vector<pair<string, string>>();
}

LanguageProcessor::~LanguageProcessor(){
    delete dict;
}

vector<pair<string,string>>& LanguageProcessor::getDict(){
    return *dict;
}



int LanguageProcessor::getXML(){
    //open the XML file -- static location for now
    //string loc = "/home/ian/Corpus/Corpus.xml";

    //int count = 0;
    ifstream xmlfile("/home/ian/Corpus/Corpus.xml");
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
        cout << "Unable to open file " << "/home/ian/Corpus/Corpus.xml" << endl;
        return -1;
    }

}
