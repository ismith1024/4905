#ifndef TRIENODE_H
#define TRIENODE_H

#include <QString>
#include <QList>


class TrieNode
{
public:
    TrieNode();
    ~TrieNode();

    void setParent(TrieNode*);
    void addChild(TrieNode*);
    QString& getValue();
    void setValue(QString&);
    bool isEndOfString();
    void setEndOfString(bool);
    bool isRoot();
    bool isLeaf();

protected:
    QList<TrieNode*>* children;
    TrieNode* parent;
    QString value;
    bool endOfString;

};

#endif // TRIENODE_H
