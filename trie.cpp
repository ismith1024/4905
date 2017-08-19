#include "trie.h"

Trie::Trie(){
    root = 0;
}

bool Trie::isEmpty(){
    return root == 0;
}

void Trie::setRoot(TrieNode* t){
    root = t;
}

TrieNode* Trie::getRoot(){return root;}

void Trie::insert(QString& dataString){

    /* if(isEmpty()) {
         DataADT data = new Data(".");
         TrieNode newRoot = new TrieNode(data);
         setRoot(newRoot);
            newRoot.setLocation(getOwner().getDefaultRootLocation());
     }

    /*Insert the characters one at a time.
     * If the character is already in the trie, traverse its children until a
     * character is not found.
     * /

    //O(h), where h is the tree height
    TrieNode traversalNode = (TrieNode) getRoot();

    for(int i = 0; i < dataString.length(); i++){
        // if the traversal node has a child node with the charat(i), skip to that node, if not, add it

        boolean nodeFound = false;

        //O(1): a node has maximum 26 children
        for(TreeNode t: traversalNode.getChildren()){
            if( t.getData().key().equals( (dataString.charAt(i)) + "") ){
                traversalNode = (TrieNode) t;
                nodeFound = true;
                }
        }

        if(nodeFound == false){
            DataADT data = new Data(dataString.charAt(i)+"");
            TrieNode newChildNode = new TrieNode(data);
            traversalNode.insertNode(newChildNode);
            Collections.sort(traversalNode.getChildren());
            newChildNode.setLocation(newChildNode.getParent().getLocation());
            traversalNode = newChildNode;
        }

    } // end for
    traversalNode.setEndOfString(true); */
} // end insert method




/*
/*************************
 * COMP2402B Assignment 3
 * Submitted by Ian Smith #100910972
 * 2014-03-24
 ************************ /

/***************************
 * This class represents a Trie data structure
 * Add, remove, and search methods have been written
 * Leaf nodes and nodes representing words are drawn as blue ellipses
 *   and have the word written sideways in a smaller font
 * Graphical creation and removal of nodes is disabled
**************************** /

package Assignment3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class Trie extends Tree{

    TrieNode root;

    public Trie(){
        super();
    }

    public void insert(String dataString){
    // does not allow graphical insertion

         if(isEmpty()) {
             DataADT data = new Data(".");
             TrieNode newRoot = new TrieNode(data);
             setRoot(newRoot);
                newRoot.setLocation(getOwner().getDefaultRootLocation());
         }

        /*Insert the characters one at a time.
         * If the character is already in the trie, traverse its children until a
         * character is not found.
         * /

        //O(h), where h is the tree height
        TrieNode traversalNode = (TrieNode) getRoot();

        for(int i = 0; i < dataString.length(); i++){
            // if the traversal node has a child node with the charat(i), skip to that node, if not, add it

            boolean nodeFound = false;

            //O(1): a node has maximum 26 children
            for(TreeNode t: traversalNode.getChildren()){
                if( t.getData().key().equals( (dataString.charAt(i)) + "") ){
                    traversalNode = (TrieNode) t;
                    nodeFound = true;
                    }
            }

            if(nodeFound == false){
                DataADT data = new Data(dataString.charAt(i)+"");
                TrieNode newChildNode = new TrieNode(data);
                traversalNode.insertNode(newChildNode);
                Collections.sort(traversalNode.getChildren());
                newChildNode.setLocation(newChildNode.getParent().getLocation());
                traversalNode = newChildNode;
            }

        } // end for
        traversalNode.setEndOfString(true);
    } // end insert method


    public void remove(String aKeyString){
    //remove nodes if nodes exist matching aKeyString
    //O(h), where h is tree height
        if(isEmpty()) return;
        TrieNode traversalNode = (TrieNode) getRoot();

        for(int i = 0; i < aKeyString.length(); i++){
            if(!traversalNode.children().isEmpty()){
                boolean charFound = false;
                //for loop is O(1) - maximum of 26 children
                for(TreeNodeADT t: traversalNode.children()){
                    if(t.getData().key().equals(aKeyString.charAt(i)+"" ) ){
                        traversalNode = (TrieNode) t;
                        charFound = true;
                    }
                }
                if(!charFound){
                    return;
                }
            }
        }

        // should now have last node in string
        // if there are children, then there are other strings in the trie and we need to keep the nodes
        // otherwise, delete nodes until a terminal node or root or node with children is found

        if(traversalNode.children().isEmpty()){ // is the end
            traversalNode.setEndOfString(false); // allows the following loop to iterate
            while(!traversalNode.getEndOfString() && traversalNode.children().isEmpty() && !traversalNode.isRoot()){
                traversalNode = removeNode((TreeNode) traversalNode);
            }
        } else {
            traversalNode.setEndOfString(false);
        }

    } // end remove method

    private TrieNode removeNode(TreeNode nodeToRemove){
        //private helper method
        //O(1)
        TrieNode nodeParent = (TrieNode) nodeToRemove.getParent();
        nodeParent.removeChildNode(nodeToRemove);
        return nodeParent;
    }


    public DataADT find(String aKeyString){
        //O(h), where h is the tree height
        clearSelections();

        if(isEmpty()) return null;

        TrieNode traversalNode = (TrieNode) getRoot();

        for(int i = 0; i < aKeyString.length(); i++){
            if(!traversalNode.children().isEmpty()){
                boolean charFound = false;
                // for loop is O(1) - maximum of 26 children
                for(TreeNodeADT t: traversalNode.children()){
                    if(t.getData().key().equals(aKeyString.charAt(i)+"" ) ){
                        traversalNode = (TrieNode) t;
                        charFound = true;
                    }
                }
                if(!charFound){
                    return null;
                }
            }
        }

        //go back and select the nodes
        //O(h), assuming Java String methods are O(n)
        String wordString = "";
        while(!traversalNode.isRoot() ){
            wordString += traversalNode.getData().key();
            traversalNode.setSelection(true);
            traversalNode = (TrieNode) traversalNode.parent();
        }

         String returnString = new StringBuilder(wordString).reverse().toString();
         DataADT returnADT = new Data(returnString);
         return returnADT;
    }

     public boolean allowsGraphicalDeletion(){
            //answer whether this tree allows graphical deletion of arbiratry nodes
            //this method should be over-ridden by trees that don't want this to be possible
            return false;
     }

} // end Trie class




*/
