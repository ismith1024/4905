#ifndef TESTFILE_H
#define TESTFILE_H

class TestFile{
public:
    string filename;
    string text;
    vector<string> lines;
    vector<vector<pair<string,string>>*> nounPhrases;
    vector<vector<pair<string,string>>*> verbPhrases;
    map<pair<string, string>, string> collocations;
    vector<string> words;
    vector<pair<string, string>> tags;

    enum enums::TOPIC topic;
    TestFile* parent;

    friend std::ostream& operator<< (std::ostream& os, const TestFile& tf){
        os << endl << "TESTFILE: " << tf.filename << endl << "...................." << endl;
        os << "  text: " << endl << tf.text << endl;
        os << "  lines: " << endl;
        for(auto& entry: tf.lines) os << "    " << entry << endl;
        os << "  words: " << endl << "    ";
        for(auto& entry: tf.words) os << entry << "..";
        os << endl;
        os << "  tags: " << endl << "    ";
        for(auto& entry: tf.tags) os << entry.first << ", " << entry.second << "..";
        os << endl;
        os << "  collocations: " << endl << "    ";
        for(auto& entry: tf.collocations) os << "(" << entry.first.first << ", " << entry.first.second <<") - " << entry.second << "..";
        os << endl;
        os << "   " << tf.nounPhrases.size() << "  noun phrases: " << endl;
        for(auto& entry: tf.nounPhrases) {
            os << "<";
            for(auto& e1: *entry) os << e1.first << ": " << e1.second << "...";
            os << ">" << endl;
        }
        os << endl;
        os << "   " << tf.verbPhrases.size() << "  verb phrases: " << endl;
        for(auto& entry: tf.verbPhrases) {
            os << "<";
            for(auto& e1: *entry) os << e1.first << ": " << e1.second << "...";
            os << ">" << endl;
        }
        os << endl;

        return os;
    }



};


#endif // TESTFILE_H
