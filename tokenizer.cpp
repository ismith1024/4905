#include"tokenizer.h"
#include<string>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <algorithm>
#include<stdio.h>

using namespace std;

int main(int argc, char* argv[]){

    if(argc < 2){
        //qDebug() << "File name required";
        cout << "File name required";
        return 1;
    }

    string line;
    ifstream infile(argv[1]);
    string parsed = "";

    if (infile.is_open()) {
      cout << "Parsing file " << argv[1];
      while ( getline (infile,line) ){
          cout << ".";
          replace(line.begin(), line.end(), '\n', '|');
          replace(line.begin(), line.end(), ' ', '|');
          replace(line.begin(), line.end(), '\r', '|');
          replace(line.begin(), line.end(), '\xA0', '|');
          replace(line.begin(), line.end(), '\t', '|');
          replace(line.begin(), line.end(), '\x1680', '|');
          replace(line.begin(), line.end(), '\x180E', '|');
          replace(line.begin(), line.end(), '\x2000', '|');
          replace(line.begin(), line.end(), '\x2001', '|');
          replace(line.begin(), line.end(), '\x2002', '|');
          replace(line.begin(), line.end(), '\x2003', '|');
          replace(line.begin(), line.end(), '\x2004', '|');
          replace(line.begin(), line.end(), '\x2005', '|');
          replace(line.begin(), line.end(), '\x2006', '|');
          replace(line.begin(), line.end(), '\x2007', '|');
          replace(line.begin(), line.end(), '\x2008', '|');
          replace(line.begin(), line.end(), '\x2009', '|');
          replace(line.begin(), line.end(), '\x200A', '|');
          replace(line.begin(), line.end(), '\x200B', '|');
          replace(line.begin(), line.end(), '\x202F', '|');
          replace(line.begin(), line.end(), '\x205F', '|');
          replace(line.begin(), line.end(), '\x3000', '|');
          replace(line.begin(), line.end(), '\xFEFF', '|');
          replace(line.begin(), line.end(), '\xC2', '|');

          //unknown garbage characters that were found in files
          replace(line.begin(), line.end(), (char) -79, '|');
          replace(line.begin(), line.end(), (char) -83, '|');
          replace(line.begin(), line.end(), (char) -108, '|');
          replace(line.begin(), line.end(), (char) -109, '|');
          replace(line.begin(), line.end(), (char) -87, '|');
          replace(line.begin(), line.end(), (char) -30, '|');
          replace(line.begin(), line.end(), (char) -51, '|');
          replace(line.begin(), line.end(), (char) -66, '|');
          replace(line.begin(), line.end(), (char) -30, '|');
          replace(line.begin(), line.end(), (char) -80, '|');

          parsed.append(line);
          parsed.append("|");

      }
      infile.close();
    } else{
        cout << "Unable to open file " << argv[1] << endl;
        return 1;
    }

    cout << endl << argv[1] << " : " << parsed << endl;

    return 0;

}
