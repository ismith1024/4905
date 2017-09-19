#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QVariant>
#include "component.h"
#include <vector>

using namespace std;

class Repository
{
public:
    Repository();
    QSqlDatabase& getDatabase();
    ~Repository();

    void getComponents(vector<Component>&);


private:
    QSqlDatabase database;


};

#endif // REPOSITORY_H
