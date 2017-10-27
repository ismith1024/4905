#include "repository.h"
#include <iostream>
#include <QDebug>
#include <qstring.h>

Repository::Repository(){

    // load the SQLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");

    //Define and open the SQLite database file
    QDir curr = QDir::current();
    QString dbLocation =  "/home/ian/Data/tf2.db";
    database.setDatabaseName(dbLocation);
    database.open();
    qDebug() << "Open the database";

}

QSqlDatabase& Repository::getDatabase(){
    return database;
}

Repository::~Repository(){
    database.close();
}

void Repository::getComponents(vector<Component*>& coll){
    QSqlQuery query;
    //database.open();

    if (!query.exec("SELECT * FROM comps;")){
         qDebug() << "getComponents SQL error: "<< query.lastError().text() << endl;
         exit(-1);
    }
    else qDebug() << query.first();

    Component* newComp;

    while(query.next()){
        //cout << query.value(0).toString().toStdString();
        string mfr = query.value(0).toString().toStdString();
        string mpn = query.value(1).toString().toStdString();
        string desc = query.value(2).toString().toStdString();
        string type = query.value(3).toString().toStdString();

        newComp = new Component(mfr, mpn, desc, type);

        coll.push_back(newComp);
    }

    //database.close();

}

int Repository::getTechDictionary(map<string,string>* techdict){
    QSqlQuery query;
    //database.open();

    if (!query.exec("SELECT * FROM materialDictionary;")){
         qDebug() << "getTechDictionary SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        //cout << query.value(0).toString().toStdString();
        string st1 = query.value(0).toString().toStdString();
        string st2 = query.value(1).toString().toStdString();

        //cout << st1 << " : " << st2 << endl;

        techdict->operator[](st1) = st2; //push_back(make_pair(st1, st2));
    }

    //database.close();
    return 0;
}


/////
/// \brief Repository::getAllDwgTextFromDB
/// \param coll
/// \return
/// This collection houses the texts scraped from the training set of drawings, with topic analysis
int Repository::getAllDwgTextFromDB(vector<vector<string>*>& coll){

    QSqlQuery query;
    //database.open();

    if (!query.exec("SELECT * FROM text;")){
         qDebug() << "getAllDwgTextFromDB SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    vector<string>* currentLine = new vector<string>();

    while(query.next()){

        QString s1 = query.value(1).toString();

        QStringList pieces = s1.split('~');

        for(QString s: pieces){
            currentLine->push_back(s.toLower().toStdString());
        }

        coll.push_back(currentLine);

        currentLine = new vector<string>();

    }

    //database.close();
    return 0;

}

//////////
/// \brief Repository::getAllDescriptionsFromDB
/// \param coll
/// \return
/// This collection houses the components existing in the TF2 database
int Repository::getAllDescriptionsFromDB(vector<string>& coll){

    QSqlQuery query;

    if (!query.exec("SELECT desc FROM comps UNION SELECT desc FROM subassemblies UNION SELECT desc FROM generics;")){
         qDebug() << "getAllDescriptionsFromDB SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        QString s1 = query.value(0).toString();
        coll.push_back(s1.toStdString());
    }

    return 0;
}
