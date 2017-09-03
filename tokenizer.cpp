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

    Tokenizer* tok = new Tokenizer(argv[1]);
    tok->tokenize();
    tok->writeToSQL();

    delete tok;
}



Tokenizer::Tokenizer(string f){
    records = new vector<SQLrecord>();
    workingFile = f;
}

Tokenizer::~Tokenizer(){
    for(vector<SQLRecord*>::iterator it = records->begin(); it != records->end(); ++it) {
        delete *it;
    }

    delete records;
}

void Tokenizer::writeToSQL(){
    // load the SQLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");

    //Define and open the db file
    QDir curr = QDir::current();
    //curr.cdUp();
    QString dbLocation = curr.path() + "4905.db";
    database.setDatabaseName(dbLocation);

    //open the database and execute the query
    database.open();
    QSqlQuery query;
    query.prepare("INSERT INTO records(fileName, tokens)"
                  "VALUES (:ifile, :itok)");

    for(vector<SQLRecord*>::iterator it = records->begin(); it != records->end(); ++it) {

        query.bindValue(":file", it->fileName);
        query.bindValue(":data", it->record);

        bool res = query.exec();
        if(!res)    qDebug() << query.lastError();
    }
    //return res;

}

void Tokenizer::tokenize(){
    string line;
    ifstream infile(workingFile);
    string parsed = "";

    if (infile.is_open()) {
      cout << "Parsing file " << workingFile;
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
          records->push_back(new SQLrecord(workingFile, parsed));

      }
      infile.close();
    } else{
        cout << "Unable to open file " << workingFile << endl;
        return 1;
    }

    cout << endl << workingFile << " : " << parsed << endl;

    return 0;

}


