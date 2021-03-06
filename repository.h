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
#include "enums.h"
#include "tokenizer.h"
#include <iostream>
#include <fstream>

using namespace std;

class Repository{

public:

    Repository(Tokenizer*);
    QSqlDatabase& getDatabase();
    ~Repository();

    void getComponents(vector<Component*>&);
    void getComponentsIncludingGenerics(vector<Component*>&);
    int getTechDictionary(map<string,string>*);

    int getAllDwgTextFromDB(vector<vector<string>*>&);

    int getAllDescriptionsFromDB(vector<string>&);
    int getContractsComponentsDescriptionsFromDB(vector<string>&);

    int getTopicCounts(map<string, int>&, enum enums::TOPIC);
    int getTopicsByNumber(map<enums::TOPIC, int>&);
    int countOfStringGivenTopic(string, enums::TOPIC);
    int getSupplierNames(vector<string>&);

    int getSupplierNumbers(map<string, int>&);
    int getMaterialTypes(map<string, string>&);

    void getWordsFromMaterialDictionary();
    void getParentTypes(map<string, float>&, string);

    void getFunctionalTestComponents(vector<Component*>&);

    int getTechKeywords(vector<pair<string,string>>&);

    void replaceParents();

    int getParentPairs(map<pair<string,string>,int>&);

private:
    QSqlDatabase database;
    Tokenizer* tok;

};

#endif // REPOSITORY_H
