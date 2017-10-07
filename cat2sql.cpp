#include <iostream>
#include <fstream>
#include<string>
#include<vector>

using namespace std;

/************************
 * This is a utility to take the results from
 *
 * more * | cat >> out.txt
 *
 * and formats it as a SQL script for the schema
 *
 * CREATE TABLE IF NOT EXISTS text(
 *   file    TEXT PRIMARY KEY NOT NULL,
 *   txt    TEXT NOT NULL
 * );
 *
 * where file is the name of a text file and txt is the file's contents
 *
 * */

//function is based on the forums at cplusplus.com
string replaceStrChar(string str, const string& replace, char ch) {

  size_t found = str.find_first_of(replace);

  while (found != string::npos) {
    str[found] = ch;
    found = str.find_first_of(replace, found+1);
  }

  return str;
}


int main(int argc, char *argv[]){

    //if(argv[0] == 0) return -1;

    ifstream infile("/home/ian/Documents/zz2.txt");//argv[1]);
    ofstream outfile;
    outfile.open ("/home/ian/Documents/out.sql");

    string filename = "";
    string txt = "";
    string inString;
    vector<string> longFileLog = vector<string>();

    while (getline(infile, inString)) {

        //istringstream inString(line);

        if(filename.compare("") ==0)
            filename = inString;
        else{
           if(inString.compare("::::::::::::::") == 0){
               outfile /*cout*/ << "INSERT INTO text(file, txt) VALUES( '" << filename << "', '" << txt << "');" << endl;
               if(txt.length() > 10000) longFileLog.push_back(filename);
               txt = "";
               filename = "";
           } else{
               inString = replaceStrChar(inString, " ", '~');
               inString = replaceStrChar(inString, "\'", '`');
               txt += inString + "~";
           }
        }
    }

    //outfile << "Long Files:" <<endl;
    //for(string s: longFileLog) outfile << s << endl;
    outfile.close();
     return 0;


    return 0;
}


