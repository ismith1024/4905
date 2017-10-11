#include "postagger.h"

PosTagger::PosTagger(){
    results = new vector<pair<string, POS>>();
}

PosTagger::~PosTagger(){
    delete results;
}

/////////
/// \brief PosTagger::buildDict
/// Builds a POS tagging dictionary from the corpus
///
int PosTagger::buildDict(){


    return -1;
}

/////////
/// \brief PosTagger::tag
/// Takes a reference to the tokenized string vector
/// Adds a <string, POS enum> entry to the tagged text collection for each word
///
int PosTagger::tag(vector<string>&, vector<pair<string, POS>>&){



    return -1;
}


/////////
/// \brief PosTagger::tokenize
/// Takes a raw text captured from design file
/// Removes stop words and symbols
/// Adds the words in order to the end of the string vector
///
int PosTagger::tokenize(string, vector<string>&){

    return -1;
}

