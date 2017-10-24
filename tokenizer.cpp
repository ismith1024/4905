#include"tokenizer.h"
#include<string>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <algorithm>
#include<stdio.h>
#include<QSqlDatabase>
#include<QString>
#include<QDir>
#include<QSqlQuery>
#include<QSqlError>

using namespace std;

Tokenizer::Tokenizer(string f){
    records = new vector<SQLrecord*>();
    workingFile = f;
}

Tokenizer::Tokenizer(){
    records = new vector<SQLrecord*>();
    workingFile = "/home/ian/Data/testcase.txt";
}

Tokenizer::~Tokenizer(){
    for(vector<SQLrecord*>::iterator it = records->begin(); it != records->end(); ++it) {
        delete *it;
    }

    delete records;
}

void Tokenizer::writeToSQL(){
    // load the SQLite driver
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");

    //Define and open the db file
    QDir curr = QDir::current();
    //curr.cdUp();
    QString dbLocation = curr.path() + "/4905.db";
    qDebug() << dbLocation << endl;
    database.setDatabaseName(dbLocation);

    //open the database and execute the query
    database.open();
    QSqlQuery query(database);


    for(vector<SQLrecord*>::iterator it = records->begin(); it != records->end(); ++it) {

        QString s1 = QString::fromStdString((*it)->fileName);
        QString s2 = QString::fromStdString((*it)->record); //.replace('\'', ' ');
        QString s3 = "test";

        //cout << "Binding" << s1.toStdString() << "  " << s2.toStdString();

        query.prepare("INSERT INTO records(fileName, tokens) VALUES(:FILE, :DATA);");
        query.bindValue(":FILE", s1); //, QSql::In);
        query.bindValue(":DATA", s2); //, QSql::In);

        //qDebug() << s1 << "," << s2 << endl;
        //query.prepare("INSERT INTO records(fileName, tokens) VALUES(?, ?);");
        //query.addBindValue(s1);
        //query.addBindValue(s2);

        qDebug() << query.lastQuery();

        bool res = query.exec();
        if(!res)    qDebug() << query.lastError();
    }

}

void Tokenizer::tokenize(){
    string line;
    ifstream infile(workingFile);
    string parsed = "";

    if (infile.is_open()) {
      //cout << "Parsing file " << workingFile;
      while ( getline (infile,line) ){
          //cout << ".";
          replaceStopCharacters(line);

          parsed.append(line);
          parsed.append("|");
      }
      infile.close();
      records->push_back(new SQLrecord(workingFile, parsed));
    } else{
        cout << "Unable to open file " << workingFile << endl;
        return;// 1;
    }

    //cout << endl << workingFile << " : " << parsed << endl;

    return; // 0;

}



void Tokenizer::removeStopCharacters(QString& line){
    line.remove(QChar('!'));
}

void Tokenizer::replaceStopCharacters(string& line){
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
}


