#include "trienode.h"

TrieNode::TrieNode(){
    children = new QList<TrieNode*>();
    parent = 0;
    value = "";
    endOfString = false;
}

TrieNode::~TrieNode(){
    delete children;
}

void TrieNode::setParent(TrieNode* par){parent = par;}

void TrieNode::addChild(TrieNode* ch){children->append(ch);}

QString& TrieNode::getValue(){return value;}

void TrieNode::setValue(QString& v){value = v;}

bool TrieNode::isEndOfString(){return endOfString;}

void TrieNode::setEndOfString(bool b){endOfString = b;}

bool TrieNode::isRoot(){return parent == 0;}

bool TrieNode::isLeaf(){return children->size() == 0;}


/*
/*************************
 * COMP2402B Assignment 3
 * Submitted by Ian Smith #100910972
 * 2014-03-24
 ************************ /

/*************************
 * This class represents a Trie node
 * It inherits from TreeNode
 * A boolean endOfString is added, representing the terminal node in a string, including get/set methods
 * The draw() and setPenColor() methods are overridden to draw leaf nodes differently
 *************************** /


public class TrieNode extends TreeNode{

    private boolean endOfString;

    public TrieNode(){
        super();
        endOfString = false;
    }

    public TrieNode(DataADT aDataItem){
        super(aDataItem);
    }

    public boolean getEndOfString(){return endOfString;}

    public void setEndOfString(boolean b){endOfString = b;}

    //@Override
    protected void setPenColor(Graphics2D aPen){
        if(endOfString)
            aPen.setColor(Color.blue);
        else
            aPen.setColor(Color.black);
    }




} // end TrieNode class





*/
