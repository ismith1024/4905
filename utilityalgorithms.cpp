#include "utilityalgorithms.h"

using namespace std;

/*
Implements the longest common subsequence, defined as follows:
function LCSubstr(S[1..m], T[1..n])
    L := array(1..m, 1..n)
    z := 0
    ret := {}
    for i := 1..m
        for j := 1..n
            if S[i] == T[j]
                if i == 1 or j == 1
                    L[i,j] := 1
                else
                    L[i,j] := L[i-1,j-1] + 1
                if L[i,j] > z
                    z := L[i,j]
                    ret := {S[i-z+1..i]}
                else
                if L[i,j] == z
                    ret := ret ∪ {S[i-z+1..i]}
            else
                L[i,j] := 0
    return ret
*/
int UtilityAlgorithms::longestCommonSS(string s1, string s2){

    int m = s1.length();
    int n = s2.length();
    int len[m][n];

    int z = 0;
    string ret = "";

    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            if(s1[i] == s2[j]){
                if(i == 0 || j == 0){
                    len[i][j] = 1;
                }
                else{
                    len[i][j] = len[i-1][j-1] + 1;
                }

                if(len[i][j] > z){
                     z = len[i][j];
                     ret = s1.substr(i-z+1,i);
                }
                else if (len[i][j] == z){
                    ret = ret + s1.substr(i-z+1,i);
                }
            } // if
            else len[i][j] = 0;
        } //for
    } //for
    return z;
}



/*
Implements the Levenshtein Dstance by means of the Wagner-Fischer algorithm, defined as follows:

int EditDistance(char s[1..m], char t[1..n])
   // For all i and j, d[i,j] will hold the Levenshtein distance between
   // the first i characters of s and the first j characters of t.
   // Note that d has (m+1) x (n+1) values.
   let d be a 2-d array of int with dimensions [0..m, 0..n]

   for i in [0..m]
     d[i, 0] ← i // the distance of any first string to an empty second string
                 // (transforming the string of the first i characters of s into
                 // the empty string requires i deletions)
   for j in [0..n]
     d[0, j] ← j // the distance of any second string to an empty first string

   for j in [1..n]
     for i in [1..m]
       if s[i] = t[j] then
         d[i, j] ← d[i-1, j-1]        // no operation required
       else
         d[i, j] ← minimum of
                    (
                      d[i-1, j] + 1,  // a deletion
                      d[i, j-1] + 1,  // an insertion
                      d[i-1, j-1] + 1 // a substitution
                    )

   return d[m,n]
*/
int UtilityAlgorithms::levDist(string s1, string s2){

    int m = s1.length();
    int n = s2.length();
    int dist[m+1][n+1];

    for(int i = 0; i <= m; ++i){
         dist[i][0] = i;
    }

    for(int j = 0; j <= n; ++j){
         dist[0][j] = j;
    }

    for(int j = 0; j <= n; ++j){
        for(int i = 0; i <= m; ++i){
            if(s1[i] == s2[j]){
                dist[i][j] = dist[i-1][j-1];
            } else{
                dist[i][j] = min( dist[i-1][j] + 1, min( dist[i][j-1] + 1, dist[i-1][j-1] + 1));
            }
        }
    }

    return dist[m][n];
}

//Function overloads to compare objects
int UtilityAlgorithms::longestCommonSS(Component& c1, Component& c2){
    return UtilityAlgorithms::longestCommonSS(c1.mpn, c2.mpn);
}


int UtilityAlgorithms::levDist(Component& c1, Component& c2){
    //cout << "Compare: " <<  c1.mpn << " , " << c2.mpn << " dist: " << levDist(c1.mpn, c2.mpn) << endl;
    return UtilityAlgorithms::levDist(c1.mpn, c2.mpn);
}

////////////
/// \brief UtilityAlgorithms::argmax
/// \return the <string, float> values of the largest element in the map
///
pair<string,float> UtilityAlgorithms::argmax(map<string,float>* theMap){
    if(theMap->size() > 0){
        auto choice = std::max_element(theMap->begin(), theMap->end(),
            [](const pair<string, float>& p1, const pair<string, float>& p2) {
                return p1.second < p2.second; });
    return *choice;
    }
    return make_pair("",0.0);
}

////////////
/// \brief UtilityAlgorithms::argmax
/// \return the <string, float> values of the largest element in the map
///
pair<int,float> UtilityAlgorithms::argmax(map<int,float>* theMap){
    if(theMap->size() > 0){
        auto choice = std::max_element(theMap->begin(), theMap->end(),
            [](const pair<int, float>& p1, const pair<int, float>& p2) {
                return p1.second < p2.second; });
    return *choice;
    }
    return make_pair(0,0.0);
}
