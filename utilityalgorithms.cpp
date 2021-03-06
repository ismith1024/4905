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
//////////
/// \brief UtilityAlgorithms::lcs
/// \param s1
/// \param s2
/// \return
///
int UtilityAlgorithms::lcs(string s1, string s2){

    int m = s1.length();
    int n = s2.length();
    int c[m+1][n+1];
    for(int i = 0; i < m+1; ++i){
        c[i][0] = 0;
    }
    for(int i = 0; i < n+1; ++i){
        c[0][i]=0;
    }
    for(int i = 1; i <= m; ++i){
        for(int j = 1; j <= n; ++j){
            if(s1.at(i-1) == s2.at(j-1)){
                c[i][j] = 1 + c[i-1][j-1];
            } else{
                c[i][j] = max(c[i-1][j], c[i][j-1]);
            }
        }
    }
    return c[m][n];
}


//////////
/// \brief UtilityAlgorithms::isAlphanumeric
/// \param str
/// \return
/// checks if a letter and a number are in the string
bool UtilityAlgorithms::isAlphanumeric(string& str){
    bool letter = false;
    bool number = false;
    for(auto& c: str){
        if(c >= 'a' && c <= 'z') letter = true;
        if(c >= 'A' && c <= 'Z') letter = true;
        if(c >= '0' && c <= '9') number = true;
    }

    bool ret = (letter && number) || (number && str.length() >= 5);

    //std::cout << str << "is alphanumeric: " << ret << endl;

    return ret;
}


////////
/// \brief UtilityAlgorithms::endsWith
/// \param str
/// \param endStr
/// \return
///Does the first string end with the second?
bool UtilityAlgorithms::endsWith(string& str, string& endStr){
    string::reverse_iterator it1 = str.rbegin();
    string::reverse_iterator it2 = endStr.rbegin();

    for(; it1 != str.rend() && it2 != endStr.rend(); ++it1, ++it2){
        if(*it1 != *it2) return false;
    }
    return true;
}


bool UtilityAlgorithms::isAncestor(Component* c1, Component* c2){
    if(c1->parent == NULL) return false;
    if(c1->parent == c2) return true;
    return(isAncestor(c1->parent, c2));
}

bool UtilityAlgorithms::mapContainsKey(map<string, int>& m, string& s){

    map<string, int>::iterator it = m.find(s);
    if(it != m.end()) return true;
    return false;

}

bool UtilityAlgorithms::mapContainsKey(map<string, string>& m, string& s){

    map<string, string>::iterator it = m.find(s);
    if(it != m.end()) return true;
    return false;

}

bool UtilityAlgorithms::mapContainsKey(map<string, float>& m, string& s){
    map<string, float>::iterator it = m.find(s);
    if(it != m.end()) return true;
    return false;

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
int UtilityAlgorithms::lcs(Component& c1, Component& c2){
    return UtilityAlgorithms::lcs(c1.mpn, c2.mpn);
}


int UtilityAlgorithms::levDist(Component& c1, Component& c2){
    //cout << "Compare: " <<  c1.mpn << " , " << c2.mpn << " dist: " << levDist(c1.mpn, c2.mpn) << endl;
    return UtilityAlgorithms::levDist(c1.mpn, c2.mpn);
}


bool UtilityAlgorithms::containsSubst(string& str, string& subs){

    size_t found = str.find(subs);
    return (found!=std::string::npos);

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
