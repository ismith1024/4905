#include "topicanalyzer.h"

TopicAnalyzer::TopicAnalyzer(Repository& rep){
    repo = rep;
    brownCounts = map<string, int>();
    metalCounts = map<string, int>();
    plasticCounts = map<string, int>();
    cableCounts = map<string, int>();
    assemblyCounts = map<string, int>();
    otherCounts = map<string, int>();
    pcbaCounts = map<string, int>();
    labelCounts = map<string, int>();
    electronicsCounts = map<string, int>();
    packagingCounts = map<string, int>();

    ifstream xmlfile("/home/ian/Data/Corpus.xml");
    string line = "";

    //this gets the Brown Corpus
    if (xmlfile.is_open() && xml2.is_open()) {
        while ( getline (xmlfile,line) ) {
          QString qs = QString::fromStdString(line);
          QStringList pieces = qs.split("<");

          for(QString st: pieces){
              if(st.size() > 0 && st.at(0) == 'w'){
                  QStringList pieces2 = st.split('>');

                  if(pieces2.size() > 1 ){
                      QString s = pieces2.at(0);
                      string s1 = s.replace("w type=\"", "").replace("\"","").toStdString();
                      s1 = tolower(s1);
                      brownCounts[s1]++;
                  }
              }
          }
        }
    }
    xmlfile.close();

    repo.getTopicCounts(metalCounts, METAL);
    repo.getTopicCounts( plasticCounts, PLASTIC);
    repo.getTopicCounts(cableCounts, CABLE);
    repo.getTopicCounts(assemblyCounts, ASSEMBLY);
    repo.getTopicCounts(otherCounts, OTHER);
    repo.getTopicCounts(pcbaCounts, PCBA);
    repo.getTopicCounts(labelCounts, LABEL);
    repo.getTopicCounts(electronicsCounts, ELECTRONICS);
    repo.getTopicCounts(packagingCounts,  PACKAGING);

}


//////
/// \brief TopicAnalyzer::findTopic
/// \return
///Finds the most likely topic for a string representing the text from a file
/// This is accomplished by hashing the dictionary
enum TOPIC TopicAnalyzer::findTopic(string str, Repository& repo){




    return 0;
}


////////
/// \brief TopicAnalyzer::findTopic
/// \return
/// Finds the most likely topic for a collection of strings representing the text from a file
enum TOPIC TopicAnalyzer::findTopic(vector<string>& coll, repository& repo){

    string str = "";
    for(auto& entry: coll){str += entry + " ";}

    return findTopic(str, repo);
}
