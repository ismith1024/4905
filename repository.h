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

    void getComponents(vector<Component*>&);
    int getTechDictionary(map<string,string>*);

    int getAllDwgTextFromDB(vector<vector<string>*>&);

    int getAllDescriptionsFromDB(vector<string>&);


private:
    QSqlDatabase database;


};

#endif // REPOSITORY_H
