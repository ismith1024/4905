#include "languageprocessor.h"



LanguageProcessor::LanguageProcessor(){
    dict = new vector<pair<string, string>>();
    techdict = new map<string, string>();
    tagCounts = new map<string, map<string, int>>();
}

LanguageProcessor::~LanguageProcessor(){
    delete dict;
    delete techdict;
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

    //string st = toupper(s);

    map<string, int> vals = tagCounts->operator[](s); //st
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
        string str = getTag(word); //toupper(word)
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
                      s1 = toupper(s1);
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



string LanguageProcessor::toupper(string& s){
    string ret = "";
    for(auto& ch: s) ret += std::toupper(ch);
    return ret;
}

int LanguageProcessor::openTechDictionary(Repository& repo){
    repo.getTechDictionary(techdict);
    return -1;
}

int LanguageProcessor::applyTechDictionary(vector<pair<string,string>>& coll){
    for(auto& st: coll){
        cout << "Look for " << st.first << " in technical dictionary" << endl;

        map<string, string>::iterator it = techdict->find(st.first);

        if(it != techdict->end()) {
            cout << "found ... " << techdict->operator[](st.first) << endl;
            ////TODO: this does not work.  ned to make a new pair.
            st = make_pair(st.first, ("NN SUBTYPE=ID:" + techdict->operator[](st.first)));
         }
    }

    return 0;

}

/////////
/// \brief LanguageProcessor::getNounPhrases
/// \param text : the collections of <word, tag>
/// \param phrases : the colections of [<word, tag> ... <word, tag>]
/// \return error code
///
/// implements the following:
/// Note that this approach certainly works for more than just noun phrases, however, this is traditionally the focus of phrase detection: namely, the detection and tagging of noun phrases. For noun phrases, this pattern or regular expression is the following:
/// (Adjective | Noun)* (Noun Preposition)? (Adjective | Noun)* Noun
/// This regular expression is read in the following manner: Zero or more adjectives or nouns, followed by an option group of a noun and a preposition, followed again by zero or more adjectives or nouns, followed by a single noun. A sequence of tags matching this pattern ensures that the corresponding words make up a noun phrase.
/// In addition to simply pulling out the phrases, it is common to do some simple post processing to link variants together (For example, unpluralizing plural variants).
/// reference: https://files.ifi.uzh.ch/cl/hess/classes/ecl1/termerCIE.html
///
int LanguageProcessor::getNounPhrases(vector<pair<string,string>>& text, vector<vector<pair<string, string>>*>& phrases){

    /*
    Zero or more adjectives or nouns
    followed by an option group of a noun and a preposition
    followed again by zero or more adjectives or nouns
    followed by a single noun.
    */

    vector<pair<string,string>>* currPhrase = new vector<pair<string,string>>();

    vector<pair<string,string>>::iterator it = text.end();
    vector<pair<string,string>>::iterator it2;
    while (it != text.begin()){
         --it;
         //find the terminal noun
        if(isNoun(*it)){
            currPhrase->push_back(*it);

            //search backwards from the terminal noun
            it2-- = it;
            if(it2 != text.begin()){

                //search backwards for adjectives or nouns
                while(isAdjective(*it2) || isNoun(*it2)){
                    currPhrase->push_back(*it2);
                    --it2;
                }

                //option group of noun and preposition
                if(it2 != text.begin()){
                    if(isPreposition(*it2) && isNoun(*(it2-1))){
                        currPhrase->push_back(*it2);
                        currPhrase->push_back(*(it2-1));
                        it2 -= 2;
                    }
                }

                //all remaining adjectives or nouns
                while( it2 != text.begin() && (isNoun(*it) || isAdjective(*it2) ) ){
                    currPhrase->push_back(*it2);
                    --it2;
                }

            }

            //this is the end of the phrase.  Move the outer iterator backwards.
            phrases.push_back(currPhrase);
            currPhrase = new vector<pair<string,string>>();
            it = it2;
            if(it == text.begin()) return 0;
            it--;
        }
    }


    return 0;
}

/////
/// \brief LanguageProcessor::isNoun
/// \param the <text,tag> pair we are checking
/// \return true if the tag matches a noun tag
///
bool LanguageProcessor::isNoun(pair<string,string>& word){

    return (word.second == "NN" ||
            word.second == "NNS" ||               //   Noun, plural: bicycles, earthquake, zippers
            word.second == "NN_U" ||              //  Nouns that are always uncountable		#new tag - deviation from Penn, examples: admiration, Afrikaans
            word.second == "NN_UN"||              // Nouns that might be used in the plural form and with an indefinite article, depending on their meaning	#new tag - deviation from Penn, examples: establishment, wax, afternoon
            word.second == "NNP"  ||              //   Proper noun, singular: Denver, DORAN, Alexandra
            word.second == "NNPS"                 //Proper noun, plural: Buddhists, Englishmen
            );
}

/////
/// \brief LanguageProcessor::isAdjective
/// \param the <text,tag> pair we are checking
/// \return true if the tag matches a adjective tag
///
bool LanguageProcessor::isAdjective(pair<string,string>& word){
    return(word.second == "JJ" ||       //    Adjective: beautiful, large, inspectable
           word.second == "JJR" ||      //   Adjective, comparative: larger, quicker
           word.second == "JJS"         //   Adjective, superlative: largest, quickes
           );
}


/////
/// \brief LanguageProcessor::isPreposition
/// \param the <text,tag> pair we are checking"
/// \return true if the tag matches a preposition tag
///
bool LanguageProcessor::isPreposition(pair<string,string>& word){
    return(word.second == "IN" //    Preposition/subordinate conjunction: except, inside, across, on, through, beyond, with, without
           );
}




///////// SCRAP BELOW HERE

/*///////
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
}*/
