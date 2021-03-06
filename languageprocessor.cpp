#include "languageprocessor.h"
#include <unordered_set>



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


void LanguageProcessor::applySupplierNames(vector<pair<string,string>>& tags, map<string, int>& sns){
    //Get the supplier aliases
    //map<string, int> supplierNumbers = map<string, int>();
    //repo.getSupplierNumbers(supplierNumbers);

    for(auto& i: tags){
        //cout << "Check Supplier: " << i.first << endl;
        if(UtilityAlgorithms::mapContainsKey(sns, i.first)){
            int n = sns[i.first];
            //cout << "Tag supplier " << i.first;
            i.second = "NNP subtype=supp" + to_string(n);
        }
    }
}




//////
/// \brief LanguageProcessor::getVerbStem
/// \param instr
/// \return
/// Removes terminal "ed", "s", or "ing" from a verb and checks of it is still a word
string LanguageProcessor::getVerbStem(string& instr){
    string ret = "";

    string s = "s";
    string es = "es";
    string ed = "ed";
    string ing = "ing";

    bool stem = false;

    if(UtilityAlgorithms::endsWith(instr, es)){
        stem = true;
        for(int i = 0; i < instr.length() -2; ++i){
            ret += instr.at(i);
        }
    } else if(UtilityAlgorithms::endsWith(instr, s)){
        stem = true;
        for(int i = 0; i < instr.length() -1; ++i){
            ret += instr.at(i);
        }
    } else if(UtilityAlgorithms::endsWith(instr, ed)){
        stem = true;
        for(int i = 0; i < instr.length() -2; ++i){
            ret += instr.at(i);
        }
    } else if(UtilityAlgorithms::endsWith(instr, ing)){
        stem = true;
        for(int i = 0; i < instr.length() -3; ++i){
            ret += instr.at(i);
        }
    }

    if(!stem) return instr;

    cout << ret <<  " -- verb stem: " << instr << " - " << ret << " " << getTag(ret) << " " << getTag(ret + "e") << endl;


    if(getTag(ret) != "???")
        return ret;
    else if(getTag(ret + "e") != "???"){
        ret += "e";
        return ret;
    }

    return instr;
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

    if(vals.size() == 0){
        if(UtilityAlgorithms::isAlphanumeric(s))
            return "NN subtype=MPN";
        else return "???";
    }

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
/// \brief tag
/// QStringlist version of tag()
void LanguageProcessor::tag(QStringList& inputText, vector<pair<string, string>>& outText){
    for(auto& word: inputText){
        string s = word.toStdString();
        string str = getTag(s); //toupper(word)
        outText.push_back(make_pair(s, str));
    }
}

///////
/// \brief LanguageProcessor::getXML
/// \return Error code
/// Opens the Brown Corpus XML file, parses the XML, and populates the dict collection
/// Does the same for the augmented vocabulary
int LanguageProcessor::getXML(){
    //open the XML file -- static location for now
    ifstream xmlfile("/home/ian/Data/Corpus.xml");
    ifstream xml2("/home/ian/Data/techwords.xml");
    string line = "";

    cout << "LanguageProcessor :: Open Brown Corpus " << endl;

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
                      string s1 = s.replace("w type=\"", "").replace("\"","").toUpper().toStdString();
                      s1 = toupper(s1);
                      string s2 = pieces2.at(1).toStdString();
                      //count++;
                      //cout << s2 << " : " << s1 << endl;
                      dict->push_back(make_pair(s2, s1));
                  }
              }
          }
        }

        cout << "LanguageProcessor :: Open the technical dictionary" << endl;
        //this gets the augmented vocabulary
        while ( getline (xml2,line) ) {
          QString qs = QString::fromStdString(line);
          QStringList pieces = qs.split("<");

          for(QString st: pieces){
              if(st.size() > 0 && st.at(0) == 'w'){
                  QStringList pieces2 = st.split('>');

                  if(pieces2.size() > 1 ){
                      QString s = pieces2.at(0);
                      string s1 = s.replace("w type=\"", "").replace("w type=", "").replace("\"","").toUpper().toStdString();
                      //s1 = tolower(s1);
                      string s2 = pieces2.at(1).toStdString();
                      //count++;
                      cout << s2 << " : " << s1 << endl;
                      dict->push_back(make_pair(s2, s1));
                  }
              }
          }
        }

        xmlfile.close();
        xml2.close();
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

int LanguageProcessor::applyTechDictionary(vector<pair<string,string>>& coll, map<string,string>& defns){
    for(auto& st: coll){
        //cout << "Look for " << st.first << " in technical dictionary" << endl;

        map<string, string>::iterator it = techdict->find(st.first);

        if(it != techdict->end()) {
            //cout << "found ... " << techdict->operator[](st.first) << endl;
            st = make_pair(st.first, ("NN SUBTYPE=ID:" + techdict->operator[](st.first)));
         }

        if(defns[st.first] != ""){
            st.second = defns[st.first];
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
/// This regular expression is read in the following manner:
///     Zero or more adjectives or nouns
///     followed by an option group of a noun and a preposition
///     followed again by zero or more adjectives or nouns
///     followed by a single noun.
///
/// A sequence of tags matching this pattern ensures that the corresponding words make up a noun phrase.
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

    vector<pair<string,string>>::reverse_iterator it = text.rbegin();
    vector<pair<string,string>>::reverse_iterator it2;

    vector<vector<pair<string, string>>*> workingPhrases = vector<vector<pair<string, string>>*>();
    vector<pair<string,string>> cp2  = vector<pair<string,string>>();

    //cout << "Start of getNounPhrases" << endl;

    while (it != text.rend()){

        //cout << "Check - " << it->first << "," << it->second << endl;

        //starts with the terminal noun
        if(isNoun(*it)){
            //cout << "Terminal noun found" << endl;
            cp2.push_back(*it);

            //search backwards from the terminal noun
            //cout << "Start back search" << endl;
            it2 = it;
            it2++;
            if(it2 != text.rend()){

                //search backwards for adjectives or nouns
                while( it2 != text.rend() && (isAdjective(*it2) || isNoun(*it2))){
                    //cout << "Push adj or noun : " << it2->first << endl;
                    cp2.push_back(*it2);
                    ++it2;
                }

                //option group of noun and preposition
                if(it2 < text.rend() - 1 ){
                    if(isPreposition(*it2) && isNoun(*(it2+1))){
                        cp2.push_back(*it2);
                        ++it2;
                        cp2.push_back(*it2);
                        ++it2;
                    }
                }

                //all remaining adjectives or nouns
                while( it2 != text.rend() && (isNoun(*it2) || isAdjective(*it2) ) ){
                    cp2.push_back(*it2);
                    ++it2;
                }

            }

            //this is the end of the phrase.  Move the outer iterator backwards.

            vector<pair<string,string>>* curr = new vector<pair<string,string>>();

            workingPhrases.push_back(curr);

            for(auto& entry: cp2){
                curr->insert(curr->begin(), entry);
            }

            cp2.clear();
            it = it2;
        }

        it++;

        if(it >= text.rend()) {
            for(auto& e2: workingPhrases){
                phrases.insert(phrases.begin(), e2);
            }
            return 0;
        }

    }

    return 0;
}


//////////
/// \brief LanguageProcessor::getVerbPhrases
/// \param text : the collection of <word, tag> we are checking
/// \param phrases : the collection of (collection of <word, tag>) verb phrases that were found
/// \return error code
///

/// Verb-Preposition
/// Preposition-Verb-Preposition
/// Preposition-Verb-Prepositional Phrase
/// Auxiliary Verb(s)-Verb
/// Auxiliary Verb(s)-Adverb-Verb
/// Auxiliary Verb(s)-Verb-Adverb Phrase
/// Auxiliary Verb(s)-Adverb Phrase-Verb-Preposition
/// Auxiliary Verb(s)-Adverb Phrase-Verb-Prepositional Phrase
/// Adverb Phrase-Verb-Preposition
/// Determiner-Verb-Preposition
///
/// NOTE: For this applciation, we do not want to parse nouns as part of adverb phrases
///         These need to be parsed as noun phrases.
///
/// Reference: http://www.brighthubeducation.com/esl-lesson-plans/49623-structure-of-a-verb-phrase/
///
int LanguageProcessor::getVerbPhrases(vector<pair<string,string>>& text, vector<vector<pair<string,string>>*>& phrases){

    vector<pair<string,string>>::iterator it = text.begin();
    vector<pair<string,string>>::iterator it2;

    vector<vector<pair<string, string>>*> workingPhrases = vector<vector<pair<string, string>>*>();
    vector<pair<string,string>> cp2  = vector<pair<string,string>>();

    //cout << "Start of getVerbPhrases" << endl;

    while(it != text.end()){

        if(isVerb(*it)){
            do{

                cp2.push_back(make_pair(getVerbStem((*it).first), (*it).second));
                it++;
            } while(isVerb(*it) || isPreposition(*it));

        } else if (isPreposition(*it)){
            it2 = it;
            it2++;
            if(isVerb(*it2)){
                it2++;
                if(isPreposition(*it2)){
                    do{
                        cp2.push_back(*it);
                        it++;
                    } while(it != it2);
                }
            }

            it = it2;

        } else if(isAuxVerb(*it)){

        } else if(isAdverb(*it)){
            it2 = it;
            it2++;
            if(isVerb(*it2)){
                it2++;
                if(isPreposition(*it2)){
                    do{
                        cp2.push_back(*it);
                        it++;
                    } while(it != it2);
                }
            }

            it = it2;

        } else if (isDeterminer(*it)){
            it2 = it;
            it2++;
            if(isVerb(*it2)){
                it2++;
                if(isPreposition(*it2)){
                    do{
                        cp2.push_back(*it);
                        it++;
                    } while(it != it2);
                }
            }

            it = it2;

        } else {
            ++it;

            if(it >= text.end()) {
                for(auto& e2: workingPhrases){
                    phrases.push_back(e2);
                }

                return 0;
            } else continue;
        }

        workingPhrases.push_back(new vector<pair<string,string>>());

        vector<pair<string,string>>* curr = workingPhrases.back();
        for(auto& entry: cp2){
            curr->push_back(entry);
        }

        if(curr->size() == 0) workingPhrases.pop_back();

        cp2 = vector<pair<string,string>>();

        if(it >= text.end()) {
            //for(it2 = workingPhrases.rbegin(); it2 != workingPhrases.rend(); ++ it2){
            for(auto& e2: workingPhrases){
                if(e2->size() > 0) phrases.push_back(e2);
            }

            return 0;
        }
        ++it;
    }
}

///////////////
/// \brief LanguageProcessor::findCollocationMetrics
/// \param inStrings - collection of space-delimited words  -- ["Hi there", "It's a nice day", ... "Bye for now!"]
/// \param singles   - occurrances of single words : <foo, x>
/// \param pairs     - occurrances of word pairs: <<bar, baz>, y>
/// Finds the metrics that will be used by Mutulal Information Measure to identify word collocations
void LanguageProcessor::findCollocationMetrics(vector<string>& inStrings, map<string, int>& singles, map<pair<string,string>, int>& pairs, Tokenizer& tok){
    const int SEARCH_DIST = 4;

    vector<string> stopWords = vector<string>();

    stopWords.push_back("");
    stopWords.push_back("x");
    stopWords.push_back("z");
    stopWords.push_back("t");
    stopWords.push_back("s");
    stopWords.push_back("l");
    stopWords.push_back("per");
    stopWords.push_back("for");
    stopWords.push_back("f");
    stopWords.push_back("e");
    stopWords.push_back("with");
    stopWords.push_back("t7");
    //stopWords.push_back("");


    //the strings are delimited words.
    vector<QStringList> splitWords = vector<QStringList>();
    QStringList::iterator it, it2;

    for(auto& entry: inStrings){
        splitWords.push_back(QString::fromStdString(entry).split(' '));
    }

    //Count the single words
    for(auto& entry: splitWords){
        for(auto& e2: entry){
            tok.removeStopCharacters(e2);
        }
        for(it = entry.begin(); it != entry.end(); it++){
            singles[(it->toLower().toStdString())]++;
            for(it2 = it+1; it2 <= it+SEARCH_DIST && it2 != entry.end(); it2++){
                if(find(stopWords.begin(), stopWords.end(), it->toLower().toStdString()) == stopWords.end() &&
                        find(stopWords.begin(), stopWords.end(), it2->toLower().toStdString()) == stopWords.end()){
                    if(it->length() > 1 || it2->length() > 1){
                        pair<string, string> pr = make_pair(it->toLower().toStdString(), it2->toLower().toStdString());
                        pairs[pr]++;
                    }
                }
            }
        }
    }
}


///////////
/// \brief LanguageProcessor::mimForCollocations
/// \param singles -- counts of single word occurrances
/// \param pairs -- counts of word pair occurrances
/// \param collocations -- the collocations the function finds
///  Uses point wise mutual information on the single and pair counts to find word collocations in technical corpus text
void LanguageProcessor::mimForCollocations(map<string, int>& singles, map<pair<string,string>, int>& pairs, vector<pair<string, string>>& collocations){
    //TODO: Adjust this threshold
    const float THRESHOLD = -7.0;
    const int MIN_SUPP = 10;

    for(auto& entry: pairs){
        int pairCount = entry.second;
        int s1Count = singles[entry.first.first];
        int s2Count = singles[entry.first.second];

        float mim = log((float) pairCount / (s1Count * s2Count));

        if(mim < THRESHOLD && entry.second >= MIN_SUPP) collocations.push_back(entry.first);
    }

}


/////
/// \brief LanguageProcessor::isNoun
/// \param the <text,tag> pair we are checking
/// \return true if the tag matches a noun tag
///
bool LanguageProcessor::isNoun(pair<string,string>& word){

    if(word.second.size() >= 2 && word.second.at(0) == 'N' && word.second.at(1) == 'N') return true;
    if(word.second.size() >= 2 && word.second.at(0) == 'C' && word.second.at(1) == 'D') return true;
    if(word.second.size() >= 2 && word.second.at(0) == 'n' && word.second.at(1) == 'n') return true;


    /*if (word.second == "NN" ||
            word.second == "NNS" ||               //   Noun, plural: bicycles, earthquake, zippers
            word.second == "NN_U" ||              //  Nouns that are always uncountable		#new tag - deviation from Penn, examples: admiration, Afrikaans
            word.second == "NN_UN"||              // Nouns that might be used in the plural form and with an indefinite article, depending on their meaning	#new tag - deviation from Penn, examples: establishment, wax, afternoon
            word.second == "NNP"  ||              //   Proper noun, singular: Denver, DORAN, Alexandra
            word.second == "NNPS"                 //Proper noun, plural: Buddhists, Englishmen
            ) return true;*/
    return false;

}

/////
/// \brief LanguageProcessor::isAdjective
/// \param the <text,tag> pair we are checking
/// \return true if the tag matches a adjective tag
///
bool LanguageProcessor::isAdjective(pair<string,string>& word){

    if(word.second == "JJ" ||       //    Adjective: beautiful, large, inspectable
           word.second == "JJR" ||      //   Adjective, comparative: larger, quicker
           word.second == "JJS" ||        //   Adjective, superlative: largest, quickes
            word.second == "jj" ||       //    Adjective: beautiful, large, inspectable
            word.second == "jjr" ||      //   Adjective, comparative: larger, quicker
            word.second == "jjs"        //   Adjective, superlative: largest, quickes
           ) return true;

    return false;
}


/////
/// \brief LanguageProcessor::isPreposition
/// \param the <text,tag> pair we are checking"
/// \return true if the tag matches a preposition tag
///
bool LanguageProcessor::isPreposition(pair<string,string>& word){
    if (word.second == "IN" //    Preposition/subordinate conjunction: except, inside, across, on, through, beyond, with, without
           ) return true;
    return false;
}

//////
/// \brief LanguageProcessor::isVerb
/// \param word : the <text,tag> pair we are checking
/// \return true if it matches one of the tags
///
bool LanguageProcessor::isVerb(pair<string,string>& word){
    if (word.second == "TO" || // "To" is in here for now for infinitives
        word.second == "MD" || //    Modal: should, can, need, must, will, would
        word.second == "VB" || //    Verb, base form: eat, jump, believe, be, have
        word.second == "VBD" || //   Verb, past tense: ate, jumped, believed
        word.second == "VBG" || //   Verb, gerund/present participle: eating, jumping, believing
        word.second == "VBN" || //   Verb, past participle: eaten, jumped, believed
        word.second == "VBP" || //   Verb, non-3rd ps. sing. present: eat, jump, believe, am (as in 'I am'), are
        word.second == "VBZ" ||   //   Verb, 3rd ps. sing. present: eats, jumps, believes, is, has
        word.second == "to" || // "To" is in here for now for infinitives
        word.second == "md" || //    Modal: should, can, need, must, will, would
        word.second == "vb" || //    Verb, base form: eat, jump, believe, be, have
        word.second == "vbd" || //   Verb, past tense: ate, jumped, believed
        word.second == "vbg" || //   Verb, gerund/present participle: eating, jumping, believing
        word.second == "vbn" || //   Verb, past participle: eaten, jumped, believed
        word.second == "vbp" || //   Verb, non-3rd ps. sing. present: eat, jump, believe, am (as in 'I am'), are
        word.second == "vbz"    //   Verb, 3rd ps. sing. present: eats, jumps, believes, is, has


        ) return true;

    return false;
}

//////
/// \brief LanguageProcessor::isAuxVerb
/// \param word : the <text,tag> pair we are checking
/// \return true if it matches one of the tags
///
/// Note: Corpus does not appear to distinguish modal vs other verbs.
///
bool LanguageProcessor::isAuxVerb(pair<string,string>& word){
    /*if (

        ) return true;
*/
    return false;
}

//////
/// \brief LanguageProcessor::isAdverb
/// \param word : the <text,tag> pair we are checking
/// \return true if it matches one of the tags
///
bool LanguageProcessor::isAdverb(pair<string,string>& word){
    if (word.second == "RB" || //    Adverb and negation: easily, sunnily, suddenly, specifically, not
        word.second == "RBR" || //   Adverb, comparative: better, faster, quicker
        word.second == "RBS" || //   Adverb, superlative: best, fastest, quickest
        word.second == "WRB" ||//   wh-adverb: however, how, wherever, where, when, why
        word.second == "rb" || //    Adverb and negation: easily, sunnily, suddenly, specifically, not
        word.second == "rbr" || //   Adverb, comparative: better, faster, quicker
        word.second == "rbs" || //   Adverb, superlative: best, fastest, quickest
        word.second == "wrb" //   wh-adverb: however, how, wherever, where, when, why
        ) return true;

    return false;
}

//////
/// \brief LanguageProcessor::isDeterminer
/// \param word : the <text,tag> pair we are checking
/// \return true if it matches one of the tags
///
bool LanguageProcessor::isDeterminer(pair<string,string>& word){
    if (word.second == "DT" //    Determiner: an, an, all, many, much, any, some, this
       ) return true;

    return false;
}

////////
/// \brief LanguageProcessor::dumpUnknownWords
/// \param words
/// Used to find untagged words in text
void LanguageProcessor::dumpUnknownWords(vector<pair<string, string>>& words, string loc){

    ofstream outFile;
    unordered_set<string> set = unordered_set<string>();

    string file = "/home/ian/Data/" + loc;

    outFile.open(file);
    for(auto& entry: words){
        if(entry.second == "???"){
            //outFile << entry.first << endl;
            set.insert(entry.first);
        }
    }

    for(auto& entry: set){
        outFile << entry << endl;
    }


    outFile.close();
    return;
}


//////////
/// \brief LanguageProcessor::containsNumbers
/// \param str
/// \return true if there ae [0-9] characters in the string
///
bool LanguageProcessor::containsNumbers(string str){

    for(char& c: str){
        if(c >= '0' && c <= '9') return true;
    }

    return false;
}

bool LanguageProcessor::containsNumbers(QString& str){

    for(QChar& c: str){
        if(c >= '0' && c <= '9') return true;
    }

    return false;
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
