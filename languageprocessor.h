#ifndef LANGUAGEPROCESSOR_H
#define LANGUAGEPROCESSOR_H
#endif // LANGUAGEPROCESSOR_H

#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<map>
#include"QString"
#include"QStringList"
#include"repository.h"
#include"unordered_set"

using namespace std;

enum POS {
    /*These are mostly the tags of the Penn Treebank tagset as used by LanguageTool,
    with examples. See "new tag" for tags introduced by LanguageTool.
    For more details, also see http://www.cis.upenn.edu/~treebank/*/

    CC, //    Coordinating conjunction: and, or, either, if, as, since, once, neither, less
    CD, //    Cardinal number: one, two, twenty-four
    DT, //    Determiner: an, an, all, many, much, any, some, this
    EX, //    Existential there: there (no other words)
    FW, //    Foreign word: infinitum, ipso
    IN, //    Preposition/subordinate conjunction: except, inside, across, on, through, beyond, with, without
    JJ, //    Adjective: beautiful, large, inspectable
    JJR, //   Adjective, comparative: larger, quicker
    JJS, //   Adjective, superlative: largest, quickest
    LS, //    List item marker: not used by LanguageTool
    MD, //    Modal: should, can, need, must, will, would
    NN, //    Noun, singular or mass: bicycle, earthquake, zipper
    NNS, //   Noun, plural: bicycles, earthquake, zippers
    NN_U, //  Nouns that are always uncountable		#new tag - deviation from Penn, examples: admiration, Afrikaans
    NN_UN, // Nouns that might be used in the plural form and with an indefinite article, depending on their meaning	#new tag - deviation from Penn, examples: establishment, wax, afternoon
    NNP, //   Proper noun, singular: Denver, DORAN, Alexandra
    NNPS, //  Proper noun, plural: Buddhists, Englishmen
    PDT, //   Predeterminer: all, sure, such, this, many, half, both, quite
    POS, //   Possessive ending: s (as in: Peter's)
    PRP, //   Personal pronoun: everyone, I, he, it, myself
    PRP_, //  Possessive pronoun: its, our, their, mine, my, her, his, your
    RB, //    Adverb and negation: easily, sunnily, suddenly, specifically, not
    RBR, //   Adverb, comparative: better, faster, quicker
    RBS, //   Adverb, superlative: best, fastest, quickest
    RP, //    Particle: in, into, at, off, over, by, for, under
    SYM, //   Symbol: not used by LanguageTool
    TO, //    to: to (no other words)
    UH, //    Interjection: aargh, ahem, attention, congrats, help
    VB, //    Verb, base form: eat, jump, believe, be, have
    VBD, //   Verb, past tense: ate, jumped, believed
    VBG, //   Verb, gerund/present participle: eating, jumping, believing
    VBN, //   Verb, past participle: eaten, jumped, believed
    VBP, //   Verb, non-3rd ps. sing. present: eat, jump, believe, am (as in 'I am'), are
    VBZ, //   Verb, 3rd ps. sing. present: eats, jumps, believes, is, has
    WDT, //   wh-determiner: that, whatever, what, whichever, which (no other words)
    WP, //    wh-pronoun: that, whatever, what, whatsoever, whosoever, who, whom, whoever, which (no other words)
    WP$, //   Possessive wh-pronoun: whose (no other words)
    WRB, //   wh-adverb: however, how, wherever, where, when, why
    LQ, //    Left open double quote
    CM, //     Comma
    RQ, //    Right close double quote
    DOT, //  Sentence-final punctuation (in LanguageTool, use SENT_END instead)
    COLON, // Colon, semi-colon
    DOL, //   Dollar sign

    //////// Specific to this project
    NEG_PRE, //Prefix negation
    NEG_POST, //Postfix negation
    MAT, //Material
    MFR, //Supplier
    MPN, //Part number
    ALP, //Alphanumeric
};


////////////
/// \brief The LanguageProcessor class
/// Handles part-of-speech tagging
/// Builds and manages the tagged dictionary obtained the Brown Corpus
class LanguageProcessor{

private:
    vector<pair<string, string>>* dict;
    map<string,string>* techdict;
    map<string, map<string, int>>* tagCounts;
    string getTag(string);

public:

    LanguageProcessor();
    ~LanguageProcessor();

    int getXML();
    vector<pair<string,string>>& getDict();
    void tag(vector<string>&, vector<pair<string, string>>&);
    void countTags();
    //int getTestCase(vector<string>&);
    int openTechDictionary(Repository&);
    int applyTechDictionary(vector<pair<string,string>>&);

    string toupper(string&);

    int getNounPhrases(vector<pair<string,string>>&, vector<vector<pair<string,string>>*>&);
    int getVerbPhrases(vector<pair<string,string>>&, vector<vector<pair<string,string>>*>&);

    bool isNoun(pair<string,string>&);
    bool isAdjective(pair<string,string>&);
    bool isPreposition(pair<string,string>&);
    bool isVerb(pair<string,string>&);
    bool isAuxVerb(pair<string,string>&);
    bool isAdverb(pair<string,string>&);
    bool isDeterminer(pair<string,string>&);

    void dumpUnknownWords(vector<pair<string, string>>&, string);

};





