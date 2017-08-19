#ifndef TRIE_H
#define TRIE_H

#include "trienode.h"


class Trie
{
public:
    Trie();
    void insert(QString&);
    bool isEmpty();
    void setRoot(TrieNode*);
    TrieNode* getRoot();

    TrieNode* root;

};

#endif // TRIE_H
