#include "repository.h"
#include <iostream>
#include <QDebug>

Repository::Repository(){
    //facade = sf;

    // load the SQLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");

    //Define and open the SQLite database file
    QDir curr = QDir::current();
    curr.cdUp();
    QString dbLocation = curr.path() + "TF2.db";
    database.setDatabaseName(dbLocation);
    //database.open();
    //sf->setDB(database);
    qDebug() << "Open the database";

}

QSqlDatabase& Repository::getDatabase(){
    return database;
}

Repository::~Repository(){
    database.close();
}

void Repository::getComponents(vector<Component>& coll){
    query.exec("SELECT * FROM comps;");

    Component* newComp;

    while(query.next()){
        string mfr = query.value(0).toString();
        string mpn = query.value(1).toString();
        string desc = query.value(2).toString();
        string type = query.value(3).toString();

        newComp = new Component(mfr, mpn, desc, type);

        coll.push_back(newProj);
    }
}


