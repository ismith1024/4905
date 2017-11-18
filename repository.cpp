#include "repository.h"
#include <iostream>
#include <QDebug>
#include <qstring.h>

Repository::Repository(Tokenizer* tk){

    tok = tk;

    // load the SQLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");

    //Define and open the SQLite database file
    QDir curr = QDir::current();
    QString dbLocation =  "/home/ian/Data/tf2.db";
    database.setDatabaseName(dbLocation);
    database.open();
    qDebug() << "Open the database";

}



Repository::~Repository(){
    cout << "Closing database" << endl;
    database.close();
}

/////////////
/// \brief Repository::getComponentsIncludingGenerics
/// \param coll
/// Retrieves components, including generic components and materials
void Repository::getComponentsIncludingGenerics(vector<Component*>& coll){
    QSqlQuery query;
    //database.open();

    if (!query.exec("SELECT * FROM comps UNION ALL SELECT * FROM generics WHERE TYPE IS NOT NULL;")){
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
}

//////////////
/// \brief getParentTypes
/// Takes in a material-article type
/// Output parameter is a map with the % of parents of each material article type
/// For example: zin plate in, [(Steel_other: 0.45), (Steel_unknown: 0.55)] out
void Repository::getParentTypes(map<string, float>& values, string material){
    QSqlQuery query;

    QString qs = QString::fromStdString("SELECT source.MaterialType, pars.MaterialType FROM (ContractsComponents natural join components) as source, (ContractsComponents natural join components) as pars WHERE source.parent = pars.lineitem AND pars.MaterialType IS NOT NULL AND source.MaterialType = '" + material + "';");

    if (!query.exec(qs)){
         qDebug() << "getComponents SQL error: "<< query.lastError().text() << endl;
         exit(-1);
    }

    //cout << qs.toStdString();

    while(query.next()){
        string s = query.value(1).toString().toStdString();
        values[s]++;
        //cout << query.value(0).toString().toStdString() << " : " << query.value(1).toString().toStdString() << endl;
    }

}

////////////
/// \brief Repository::getComponents
/// \param coll
/// Revrieves all components in the database, and creates  pointers in the collection
void Repository::getComponents(vector<Component*>& coll){
    QSqlQuery query;

    map<string, int> aliases = map<string, int>();

    getSupplierNumbers(aliases);


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
        newComp->supplierNumber = aliases[newComp->mfr];

        coll.push_back(newComp);
    }
}

///////////
/// \brief Repository::getTechDictionary
/// \param techdict
/// \return
/// Maps the technical dictionary of strings to material-article type
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

//////////
/// \brief Repository::getSupplierNumbers
/// \param numbers
/// \return success or failure
///  Populates the collection with <supplier, supplier number> pairs
int Repository::getSupplierNumbers(map<string, int>& numbers){

    QSqlQuery query;

    if (!query.exec("SELECT * FROM suppliers;")){
         qDebug() << "getSupplierNumbers SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        string s1 = query.value(0).toString().toStdString();
        int s2 = query.value(1).toInt();
        numbers[s1] = s2;
    }

    return 0;

}


/////////
/// \brief repository::getTopicCounts
/// \return
/// counts the frequency of words in known instances of a topic
int Repository::getTopicCounts(map<string, int>& counts, enum enums::TOPIC topic){
    QSqlQuery query;
    QString queryString = "";
    switch(topic){
        case enums::METAL:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Metal'";
            break;
        case enums::PLASTIC:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Plastic'";
            break;
        case enums::CABLE:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Cable'";
            break;
        case enums::ASSEMBLY:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Assembly'";
            break;
        case enums::OTHER:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Other'";
            break;
        case enums::PCBA:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'PCBA'";
            break;
        case enums::LABEL:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Label'";
            break;
        case enums::ELECTRONICS:
            queryString = "SELECT  text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Electronics'";
            break;
        case enums::PACKAGING:
            queryString = "SELECT text.txt FROM topicAnalysis join text on topicAnalysis.filename = text.file WHERE docType = 'Packaging'";
            break;
    }

    if(!query.exec(queryString)){
        cout << "getTopicCounts query failed : " << query.lastError().text().toStdString() << endl;
        return -1;
    }


    //cout << "Repository::getTopicCounts() -- " << queryString.toStdString() << endl;
    while(query.next()){
        QStringList pieces = query.value(0).toString().split(QChar('~'));
        for(auto& entry: pieces){
            tok->removeStopCharacters(entry);
            string s1 = entry.toLower().toStdString();
            //cout << s1 << endl;
            counts[s1]++;
        }
    }

    return 0;
}

//////////
/// \brief Repository::getTopicsByNumber
/// \param counts
/// \return
/// Counts the number of times each document type appears
int Repository::getTopicsByNumber(map<enums::TOPIC, int>& counts){
    QSqlQuery query;
    QString queryString = "SELECT DISTINCT docType, COUNT(docType) FROM topicAnalysis GROUP BY docType;";

    if(!query.exec(queryString)){
        cout << "getTopicsByNumber query failed : " << query.lastError().text().toStdString() << endl;
        return -1;
    }

    while(query.next()){
        string resultType = query.value(0).toString().toStdString();
        if(resultType == "Assembly") counts[enums::ASSEMBLY] = query.value(1).toInt();
        else if(resultType == "Cable") counts[enums::CABLE] = query.value(1).toInt();
        else if(resultType == "Electronics") counts[enums::ELECTRONICS] = query.value(1).toInt();
        else if(resultType == "Label") counts[enums::LABEL] = query.value(1).toInt();
         else if(resultType == "Metal") counts[enums::METAL] = query.value(1).toInt();
         else if(resultType == "Other") counts[enums::OTHER] = query.value(1).toInt();
         else if(resultType == "PCBA") counts[enums::PCBA] = query.value(1).toInt();
         else if(resultType == "Packaging") counts[enums::PACKAGING] = query.value(1).toInt();
         else if(resultType == "Plastic") counts[enums::PLASTIC] = query.value(1).toInt();
    }

    return 0;
}

////////////
/// \brief Repository::countOfStringGivenTopic
/// \param txt
/// \param topic
/// \return
/// Counts the number of documents by topic that a string appears in
int Repository::countOfStringGivenTopic(string txt, enums::TOPIC topic){
    QSqlQuery query;
    QString queryString = "SELECT topicAnalysis.docType, count(docType) from topicAnalysis JOIN text ON text.file = topicAnalysis.filename WHERE topicAnalysis.docType = ";

    switch(topic){
    case enums::METAL:
        queryString += "'Metal'";
        break;
    case enums::PLASTIC:
        queryString += "'Plastic'";
        break;
    case enums::CABLE:
        queryString += "'Cable'";
        break;
    case enums::ASSEMBLY:
        queryString += "'Assembly'";
        break;
    case enums::OTHER:
        queryString += "'Other'";
        break;
    case enums::PCBA:
        queryString += "'PCBA'";
        break;
    case enums::LABEL:
        queryString += "'Label'";
        break;
    case enums::ELECTRONICS:
        queryString += "'Electronics'";
        break;
    case enums::PACKAGING:
        queryString += "'Packaging'";
        break;

    }

    queryString = queryString + QString::fromStdString("AND (text.txt LIKE '%~" + txt + "~%' OR text.txt LIKE '" + txt + "~%' OR text.txt LIKE '%~" + txt + "');");

    if(!query.exec(queryString)){
        cout << "countOfStringGivenTopic query failed : " << query.lastError().text().toStdString() << endl;
        return -1;
    }

    while(query.next()){
        int ret = query.value(1).toInt();
        //cout << queryString.toStdString() << endl;
        //cout << topic << " has " << ret << " documents with '" << txt << "'" << endl;
        return ret;
    }


}


////////////
/// \brief Repository::getContractsComponentsDescriptionsFromDB
/// \param coll
/// \return
/// Retrieves the non-null description text from compoents in the database (in ContactsComponents table)
int Repository::getContractsComponentsDescriptionsFromDB(vector<string>& coll){
    QSqlQuery query;

    if (!query.exec("SELECT Description FROM contractsComponents WHERE Description IS NOT NULL;")){
         qDebug() << "getContractsComponentsDescriptionsFromDB SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        QString s1 = query.value(0).toString();
        coll.push_back(s1.toStdString());
    }

    return 0;
}

/////////////
/// \brief Repository::getSupplierNames
/// \param coll
/// \return
/// Retrieves the collecction of suppleir names from the database
int Repository::getSupplierNames(vector<string>& coll){
    QSqlQuery query;

    if (!query.exec("SELECT suppName FROM suppliers;")){
         qDebug() << "getSupplierNames SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        QString s1 = query.value(0).toString().toLower();
        coll.push_back(s1.toStdString());
    }

    return 0;
}

/////////
/// \brief Repository::getWordsFromMaterialDictionary
/// One-of function, used to retrieve single words from the material dictionary (for manual inspection)
void Repository::getWordsFromMaterialDictionary(){
    QSqlQuery query;

    if (!query.exec("SELECT * FROM materialDictionary;")){
         qDebug() << "getWordsFromMaterialDictionary SQL error: "<< query.lastError().text() << endl;
         return;
    }

    map<string, string> results = map<string,string>();

    while(query.next()){
        QStringList res1 = query.value(0).toString().split(' ');
        string res2 = query.value(1).toString().toStdString();
        for(auto& entry: res1){
            results[entry.toStdString()] = res2;
           }
    }


    ofstream myfile;
      myfile.open ("/home/ian/Data/revisedDict.sql");

    for(auto& entry: results){
        myfile << "INSERT INTO materialDictionary VALUES '" << entry.first << "','" << entry.second << "';" << endl;
//        cout << entry.first << " |" << entry.second << endl;

        }
    myfile.close();

}


////////
/// \brief Repository::getDatabase
/// \return
///Getter for a reference to the database
/// I don't remmber what this was for.
QSqlDatabase& Repository::getDatabase(){
    return database;
}
