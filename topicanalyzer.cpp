#include "topicanalyzer.h"

TopicAnalyzer::TopicAnalyzer(Repository& repo){
    //repo = rep;
    //tok = tk;

    //don't actually need these -- the constructor was going to create them anyway.
    /*aggregateCounts = map<string, int>();
    metalCounts = map<string, int>();
    plasticCounts = map<string, int>();
    cableCounts = map<string, int>();
    assemblyCounts = map<string, int>();
    otherCounts = map<string, int>();
    pcbaCounts = map<string, int>();
    labelCounts = map<string, int>();
    electronicsCounts = map<string, int>();
    packagingCounts = map<string, int>();
    counts = map<enums::TOPIC, int>();
*/
    /*ifstream xmlfile("/home/ian/Data/Corpus.xml");
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
                      aggregateCounts[s1]++;
                  }
              }
          }
        }
    }
    xmlfile.close();*/

    repo.getTopicCounts(metalCounts, enums::METAL);
    repo.getTopicCounts(plasticCounts, enums::PLASTIC);
    repo.getTopicCounts(cableCounts, enums::CABLE);
    repo.getTopicCounts(assemblyCounts, enums::ASSEMBLY);
    repo.getTopicCounts(otherCounts, enums::OTHER);
    repo.getTopicCounts(pcbaCounts, enums::PCBA);
    repo.getTopicCounts(labelCounts, enums::LABEL);
    repo.getTopicCounts(electronicsCounts, enums::ELECTRONICS);
    repo.getTopicCounts(packagingCounts,  enums::PACKAGING);

    repo.getTopicsByNumber(counts);

    for(auto& entry: metalCounts){aggregateCounts[entry.first]+= metalCounts[entry.first];}
    for(auto& entry: plasticCounts){aggregateCounts[entry.first]+= plasticCounts[entry.first];}
    for(auto& entry: cableCounts){aggregateCounts[entry.first]+=cableCounts[entry.first];}
    for(auto& entry: assemblyCounts){aggregateCounts[entry.first]+=assemblyCounts[entry.first];}
    for(auto& entry: otherCounts){aggregateCounts[entry.first]+=otherCounts[entry.first];}
    for(auto& entry: pcbaCounts){aggregateCounts[entry.first]+=pcbaCounts[entry.first];}
    for(auto& entry: labelCounts){aggregateCounts[entry.first]+=labelCounts[entry.first];}
    for(auto& entry: electronicsCounts){aggregateCounts[entry.first]+=electronicsCounts[entry.first];}
    for(auto& entry: packagingCounts){aggregateCounts[entry.first]+=packagingCounts[entry.first];}


}


//////
/// \brief TopicAnalyzer::findTopic
/// \return
///Finds the most likely topic for a string representing the text from a file
/// This is accomplished by hashing the dictionary
enum enums::TOPIC TopicAnalyzer::findTopic(vector<string> coll, Repository& repo){

    map<enum enums::TOPIC, float> probs = map<enum enums::TOPIC, float>();

    //uses total probability: this map contains the complement of the probability
    probs[enums::METAL] = 1.0;
    probs[enums::PLASTIC] = 1.0;
    probs[enums::CABLE] = 1.0;
    probs[enums::ASSEMBLY] = 1.0;
    probs[enums::OTHER] = 1.0;
    probs[enums::PCBA] = 1.0;
    probs[enums::LABEL] = 1.0;
    probs[enums::ELECTRONICS] = 1.0;
    probs[enums::PACKAGING] = 1.0;

    int totalPopulation = 0;

    for(auto& entry: counts) totalPopulation += entry.second;


/*
Probability of a topic given a string:
    Probability of the string given a topic * probability of the topic / probability of the string
*/
    for(auto& entry: coll){
        cout << "Check: " << entry << endl;
        //probability of string
        //////////// this is wrong
        float probOfString = (float) aggregateCounts[entry] / totalPopulation;

        if(metalCounts.find(entry) != metalCounts.end() && metalCounts[entry] > 0) {
            //probability of topic given string
            float probTopicGivenStr = (float) repo.countOfStringGivenTopic(entry, enums::METAL) / counts[enums::METAL];

            //probability of topic
            float probOfTopic = (float) counts[enums::METAL] / totalPopulation;

            probs[enums::METAL] *= 1.0 - (probTopicGivenStr * probOfString / probOfTopic);

            cout << "METAL: " << "T|s: " << probTopicGivenStr << " T: " << probOfTopic << " S: " << probOfString << " ... prob now:" << probs[enums::METAL] << endl;
        }
        if(plasticCounts.find(entry) != plasticCounts.end() && plasticCounts[entry] > 0)
            probs[enums::PLASTIC] *= 1.0 - ((float)plasticCounts[entry] / aggregateCounts[entry]);
        if(assemblyCounts.find(entry) != assemblyCounts.end() && assemblyCounts[entry] > 0)
            probs[enums::ASSEMBLY] *= 1.0 - ((float)assemblyCounts[entry] / aggregateCounts[entry]);
        if(otherCounts.find(entry) != otherCounts.end() && otherCounts[entry] > 0)
            probs[enums::OTHER] *= 1.0 - ((float)otherCounts[entry] / aggregateCounts[entry]);
        if(pcbaCounts.find(entry) != pcbaCounts.end() && pcbaCounts[entry] > 0)
            probs[enums::PCBA] *= 1.0 - ((float)pcbaCounts[entry] / aggregateCounts[entry]);
        if(labelCounts.find(entry) != labelCounts.end() && labelCounts[entry] > 0)
            probs[enums::LABEL] *= 1.0 - ((float)labelCounts[entry] / aggregateCounts[entry]);
        if(electronicsCounts.find(entry) != electronicsCounts.end() && electronicsCounts[entry] > 0)
            probs[enums::ELECTRONICS] *= 1.0 - ((float)electronicsCounts[entry]/ aggregateCounts[entry]);
        if(packagingCounts.find(entry) != packagingCounts.end() && packagingCounts[entry] > 0)
            probs[enums::PACKAGING] *= 1.0 - ((float)packagingCounts[entry]/ aggregateCounts[entry]);
    }

    for(auto& entry: probs){
        entry.second = 1.00 - entry.second;
    }

    cout << "METAL: " << probs[enums::METAL] << endl;
    cout << "PLASTIC: " << probs[enums::PLASTIC] << endl;
    cout << "CABLE: " << probs[enums::CABLE] << endl;
    cout << "ASSEMBLY: " << probs[enums::ASSEMBLY] << endl;
    cout << "OTHER: " << probs[enums::OTHER] << endl;
    cout << "PCBA: " << probs[enums::PCBA] << endl;
    cout << "LABEL: " << probs[enums::LABEL] << endl;
    cout << "ELECTRONICS: " << probs[enums::ELECTRONICS] << endl;
    cout << "PACKAGING: " << probs[enums::PACKAGING] << endl;

    float maxVal = probs[enums::METAL];
    enum enums::TOPIC max = enums::METAL;
    if(probs[enums::PLASTIC] > maxVal){maxVal = probs[enums::PLASTIC]; max = enums::PLASTIC; }
    if(probs[enums::CABLE] > maxVal){maxVal = probs[enums::CABLE]; max = enums::CABLE; }
    if(probs[enums::ASSEMBLY] > maxVal){maxVal = probs[enums::ASSEMBLY]; max = enums::ASSEMBLY; }
    if(probs[enums::OTHER] > maxVal){maxVal = probs[enums::OTHER]; max = enums::OTHER; }
    if(probs[enums::PCBA] > maxVal){maxVal = probs[enums::PCBA]; max = enums::PCBA; }
    if(probs[enums::ELECTRONICS] > maxVal){maxVal = probs[enums::ELECTRONICS]; max = enums::ELECTRONICS; }
    if(probs[enums::PACKAGING] > maxVal){maxVal = probs[enums::PACKAGING]; max = enums::PACKAGING; }

    return max;
}


////////
/// \brief TopicAnalyzer::findTopic
/// \return
/// Finds the most likely topic for a collection of strings representing the text from a file
enum enums::TOPIC TopicAnalyzer::findTopic(vector<vector<string>*>& coll, Repository& repo){

    vector<string> text = vector<string>();
    for(auto& entry: coll){
        for(auto& e2: (*entry)){
            text.push_back(e2);
        }
    }

    return findTopic(text, repo);
}


void TopicAnalyzer::printTopicWords(){

    //Will consider the word interesting if it appears k% more frequently than usual
    /////////////TODO: set these numbers to whatever works best
    const float THRESHOLD = 0.3 * (1.0/9.0);
    const int SUPPORT = 5;
    cout << endl <<"..METAL................" << endl;

    for(auto& entry: metalCounts){
        float ratio = ((float) metalCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && metalCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << metalCounts[entry.first] << endl;
    }

    cout << endl << "..PLASTIC................" << endl;

    for(auto& entry: plasticCounts){
        float ratio = ((float) plasticCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && plasticCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << plasticCounts[entry.first] <<  endl;
    }

    cout << endl << "..CABLE................" << endl;

    for(auto& entry: cableCounts){
        float ratio = ((float) cableCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && cableCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << cableCounts[entry.first] <<  endl;
    }
    cout << endl << "..ASSEMBLY................" << endl;

    for(auto& entry: assemblyCounts){
        float ratio = ((float) assemblyCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && assemblyCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << assemblyCounts[entry.first] <<  endl;
    }
    cout << endl << "..OTHER................" << endl;

    for(auto& entry: otherCounts){
        float ratio = ((float) otherCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && otherCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << otherCounts[entry.first] <<  endl;
    }
    cout << endl << "..PCBA................" << endl;

    for(auto& entry: pcbaCounts){
        float ratio = ((float) pcbaCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && pcbaCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << pcbaCounts[entry.first] <<  endl;
    }
    cout << "..LABEL................" << endl;

    for(auto& entry: labelCounts){
        float ratio = ((float) labelCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && labelCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << labelCounts[entry.first] <<  endl;
    }
    cout << "..ELECTRONICS................" << endl;

    for(auto& entry: electronicsCounts){
        float ratio = ((float) electronicsCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && electronicsCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << electronicsCounts[entry.first] <<  endl;
    }
    cout << "..PACKAGING................" << endl;

    for(auto& entry: packagingCounts){
        float ratio = ((float) packagingCounts[entry.first]) / ((float) aggregateCounts[entry.first]);
        if(ratio > THRESHOLD && packagingCounts[entry.first] >= SUPPORT) cout << entry.first << " .... ratio: " << ratio << " ......support: " << packagingCounts[entry.first] <<  endl;
    }
}
