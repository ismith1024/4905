#include "repository.h"
#include <iostream>
#include <QDebug>
#include <qstring.h>

Repository::Repository(){
    //facade = sf;

    // load the SQLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");

    //Define and open the SQLite database file
    QDir curr = QDir::current();
    QString dbLocation =  curr.path() + "/tf2.db";
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

void Repository::getComponents(vector<Component>& coll){
    QSqlQuery query;

    if (!query.exec("SELECT * FROM comps;")){
         qDebug() << "SQL error: "<< query.lastError().text() << endl;
         exit(-1);
    }
    else qDebug() << query.first();

    Component* newComp;

    while(query.next()){
        cout << query.value(0).toString().toStdString();
        string mfr = query.value(0).toString().toStdString();
        string mpn = query.value(1).toString().toStdString();
        string desc = query.value(2).toString().toStdString();
        string type = query.value(3).toString().toStdString();

        newComp = new Component(mfr, mpn, desc, type);

        coll.push_back(*newComp);
    }
}


