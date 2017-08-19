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


