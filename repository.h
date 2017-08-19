#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QVariant>

using namespace std;

class Repository
{
public:
    Repository();
    QSqlDatabase& getDatabase();
    ~Repository();

private:
    QSqlDatabase database;


};

#endif // REPOSITORY_H
