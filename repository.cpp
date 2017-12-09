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
        string type = query.value(3).toString().toLower().toStdString();

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
        string s = query.value(1).toString().toLower().toStdString();
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
        string type = query.value(3).toString().toLower().toStdString();

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
        string s1 = query.value(0).toString().toLower().toStdString();
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
/// \brief Repository::getParentPairs
/// \param coll
/// \return
/// counts the number of times a <source, parent> pair appears in the corpus
int Repository::getParentPairs(map<pair<string,string>,int>& coll){
    QSqlQuery query;

    if (!query.exec("SELECT s_mat, par_mat FROM parents2 WHERE par_mat != 'Subassembly';")){
         qDebug() << "getParentPairs SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        string s1 = query.value(0).toString().toLower().toStdString();
        string s2 = query.value(1).toString().toLower().toStdString();

        coll[make_pair(s1,s2)]++;
    }

    return 0;
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

/////////
/// \brief Repository::getTechKeywords
/// \param words
/// \return
/// finds the technical keywords in the database
int Repository::getTechKeywords(vector<pair<string,string>>& words){
    QSqlQuery query;

    const float MIN_CONF = 0.51;
    const int MIN_SUPP = 3;

    if (!query.exec("SELECT * from keywords;")){
         qDebug() << "getTechKeywords SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    map<string, int> counts = map<string, int>();
    map<pair<string,string>, int> relationCounts = map<pair<string,string>,int>();

    while(query.next()){
        QStringList pieces = query.value(0).toString().split(' ');
        for(auto& entry: pieces){
            QString s = entry.toLower().trimmed();
            tok->removeStopCharacters(s);
            string s1 = s.toStdString();
            string s2 = query.value(1).toString().toLower().toStdString();
            counts[s1]++;
            relationCounts[make_pair(s1,s2)]++;
        }
    }

    for(auto& entry: relationCounts){
        if(    (entry.second / counts[entry.first.first]) >= MIN_CONF
                && counts[entry.first.first] > MIN_SUPP
                && entry.first.second != "complex article"
                && entry.first.second != "not identified" ){
            words.push_back(make_pair(entry.first.first, entry.first.second));
        }
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

//////////////
/// \brief Repository::getMaterialTypes
/// \return
/// Gets the material tags to apply to words
int Repository::getMaterialTypes(map<string, string>& collection){

    QSqlQuery query, query2;

    if (!query.exec("SELECT * FROM techwords;")){
         qDebug() << "getMaterialTypes SQL error: "<< query.lastError().text() << endl;
         return -1;
    }

    while(query.next()){
        string s1 = query.value(0).toString().toLower().toStdString();
        string s2 = query.value(1).toString().toLower().toStdString();
        collection[s1] = s2;
    }

    if (!query2.exec("SELECT * FROM materialDictionary;")){
         qDebug() << "getMaterialTypes dictionary SQL error: "<< query2.lastError().text() << endl;
         return -1;
    }

    while(query2.next()){
        string s1 = query2.value(0).toString().toLower().toStdString();
        string s2 = query2.value(1).toString().toLower().toStdString();

        string cu = "Copper, known alloy";
        string mag = "Magnetic material";
        string sk = "Steel, known alloy";
        string ss = "Steel, stainless";
        string ni = "Nickel alloy, known";
        string ot = "Metal, other";
        string al ="Aluminum, known alloy";
        string zn = "Zinc plate, clear or blue";

        if(s2 == "Alloy Steel"){
            collection[s1] = sk;
        }
        else if(s2 =="Stainless Steel"){
            collection[s1] = ss;
        }
        else if(s2 =="Tool Steel"){
            collection[s1] = sk;
        }
        else if(s2 =="Nickel Alloy"){
            collection[s1] = ni;
        }
        else if(s2 =="BeCu"){
            collection[s1] = cu;
        }
        else if(s2 =="Bronze"){
            collection[s1] = cu;
        }
        else if(s2 =="Brass"){
            collection[s1] = cu;
        }
        else if(s2 =="Magnetic Material"){
            collection[s1] = mag;
        }
        else if(s2 =="Titanium"){
            collection[s1] = ot;
        }
        else if(s2 =="Aluminum"){
            collection[s1] = al;
        }
        else if(s2 =="Cast Iron"){
            collection[s1] = sk;
        }
        else if(s2 =="Unalloyed Copper"){
            collection[s1] = cu;
        }
        else if(s2 =="Zinc plate, clear or blue"){
            collection[s1] = zn;
        }

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


void Repository::getFunctionalTestComponents(vector<Component*>& comps){

    //Component construction signature:
    //Component(string mfr, string mpn, string description, string type

    /*  LineItem	MFR	MPN	Desc	Type*/
        comps.push_back(new Component("FabCo", "1341534", "Final Assembly", "Subassembly"));
        comps.push_back(new Component("PCBCo", "625623", "PCBA", "PWB substrate - laminate, other"));
        comps.push_back(new Component("AMP", "822275-1", "SKT PLCC-44 SMT", "Connector, other"));
        comps.push_back(new Component("AVX", "CR21-102J-T", "RES 1K 5% 0805 REV B", "Resistor, chip-type"));
        comps.push_back(new Component("DIODES_INC", "MMBD914-7", "DIODE MMBD914 SMT REV A", "Diode, other"));
        comps.push_back(new Component("HUBER&SUHNER", "85SMB-50-0-1/133N", "CONN 90DEG PC MOUNT CRIMP SMB REV B", "Connector, other"));
        comps.push_back(new Component("MAXIM", "MAX3430CSA", "IC MAX3430 RS-485 TRANSCEIVER 3.3V SOIC-8 REV C",	"IC or BGA"));
        comps.push_back(new Component("NSC", "LM324M", "IC LM324 OP-AMP SMT SO14 REV B", "IC or BGA"));
        comps.push_back(new Component("ON SEMI", "BAV70LT1", "DIODE BAV70LT1 SOT23 SMT REV B", "Diode, other"));
        comps.push_back(new Component("PANASONIC", "ERJ-6ENF1783V", "RES 178K 1% 0805 SMT", "Resistor, chip-type"));
        comps.push_back(new Component("VISHAY", "CRCW0805-102J", "RES 1K 5% 0805 REV B", "Resistor, chip-type"));
        comps.push_back(new Component("MetalCo", "SAE 1020", "CRS 1020", "Steel, known alloy"));
        comps.push_back(new Component("ANY SUPPLIER", "Galvanized",	"Galvanized", "Galvanized"));
        comps.push_back(new Component("ANY SUPPLIER", "Cardinal",	"PMS Blue", "Powder Coat"));
        comps.push_back(new Component("ANY SUPPLIER", "Aluminum 2011",	"AL 2011", "Aluminum, known alloy"));
        comps.push_back(new Component("ANY SUPPLIER", "BRASS 36000", "BRASS 360", "Copper, known alloy"));
        comps.push_back(new Component("ANY SUPPLIER", "Nylon washer", "NYLON", "Nylon"));
        comps.push_back(new Component("SABIC",	"LEXAN", "LEXAN FMR604", "Polycarbonate"));
        comps.push_back(new Component("ANY SUPPLIER", "Green", "COLORANT GREEN", "Colorant, green"));
        comps.push_back(new Component("ANY SUPPLIER", "Flame Retardant", "UL 94", "Flame retardant"));
        comps.push_back(new Component("ANY SUPPLIER", "UV Stabilizer", "UV 202", "UV Stabilizer"));
    /**/


}

////////
/// \brief Repository::getDatabase
/// \return
///Getter for a reference to the database
/// I don't remmber what this was for.
QSqlDatabase& Repository::getDatabase(){
    return database;
}

////////
/// \brief Repository::replaceParents
/// SQLite does not support UPDATE on a joined table -- will do this programatically
/// Fixes some parent-child issues in the database
void Repository::replaceParents(){
    vector<pair<string,string>> contractParents = vector<pair<string,string>>();
    vector<string> types = vector<string>();

    types.push_back("Capacitor, chip-type");
    types.push_back("Capacitor, electrolytic");
    types.push_back("Capacitor, variable");
    types.push_back("Connector, Military - aerospace type");
    types.push_back("Connector, molded, RF");
    types.push_back("Connector, other");
    types.push_back("Connector, red, orange or yellow plastic");
    types.push_back("Connector, Zn or Al contacts ");
    types.push_back("Diode, other");
    types.push_back("Flame sensor");
    types.push_back("Fuse");
    types.push_back("IC or BGA");
    types.push_back("Inductor, coil");
    types.push_back("Inductor, ferrite");
    types.push_back("Laser diode");
    types.push_back("Optical filter");
    types.push_back("Oscillator, ceramic");
    types.push_back("Oscillator, metal");
    types.push_back("Oscillator, plastic");
    types.push_back("Phororesistor");
    types.push_back("Photodiode");
    types.push_back("Phototransistor");
    types.push_back("Photovoltaic cell");
    types.push_back("Piezoelectric transducer");
    types.push_back("Potentiometer");
    types.push_back("Relay, electromagnetic");
    types.push_back("Resistor, chip-type");
    types.push_back("Resistor, other");
    types.push_back("Solder, non-RoHS");
    types.push_back("Solder, RoHS");
    types.push_back("Solenoid valve");
    types.push_back("Surge protection device");
    types.push_back("Switch, mechanical");
    types.push_back("Thermal cut-off");
    types.push_back("Thermal imaging semiconductor");
    types.push_back("Thermistor");
    types.push_back("Thermocouple");
    types.push_back("Thermostat");
    types.push_back("Transformer");
    types.push_back("Transistor");

    contractParents.push_back(make_pair("CA-160308A-02","318096G208"));
    contractParents.push_back(make_pair("CA-160223C-01","SNP00033482-100"));
    contractParents.push_back(make_pair("CA-160223C-02","SNP00033895-100"));
    contractParents.push_back(make_pair("BA-50821C-01","150-0156-01"));
    contractParents.push_back(make_pair("BA-50821C-02","150-0152-01"));
    contractParents.push_back(make_pair("BA-50821C-03","150-0155-01"));
    contractParents.push_back(make_pair("CA-160223C-02B","SNP00033895-100"));
    contractParents.push_back(make_pair("CA-160420A-01","655505-0001"));
    contractParents.push_back(make_pair("CA-160420A-02","655212-0001"));
    contractParents.push_back(make_pair("BA-160412C","690253-01"));
    contractParents.push_back(make_pair("CP-160609B-05","LRB2-PR_A_1"));
    contractParents.push_back(make_pair("CA-160624A","10740-001"));
    contractParents.push_back(make_pair("CP-161007I","5002-1307D-01"));
    contractParents.push_back(make_pair("CA-160407A","2727006"));
    contractParents.push_back(make_pair("CP-160622C","174-10000-03"));
    contractParents.push_back(make_pair("CA-160422B-Full","XCU010A-M04-166806G5"));
    contractParents.push_back(make_pair("CA-160308A-Full-2","DA150DP1MT755S-182954G54"));
    contractParents.push_back(make_pair("BA-160726A","5002-1307D-01"));
    contractParents.push_back(make_pair("CA-160505D","105321"));
    contractParents.push_back(make_pair("BA-160524A-001","A000001-77"));
    contractParents.push_back(make_pair("BA-160524A-002","A000070-01-2"));
    contractParents.push_back(make_pair("BA-160524A-003","A000029-01-2"));
    contractParents.push_back(make_pair("CT-51203A","690253-01"));
    contractParents.push_back(make_pair("CA-160909C","1000015"));
    contractParents.push_back(make_pair(" BA-160218A-01","009-9369-001"));
    contractParents.push_back(make_pair("BA-160601D","DWG-0500"));
    contractParents.push_back(make_pair("CA-160304A","062MEA00108"));
    contractParents.push_back(make_pair("CA-160530C-01","11175E"));
    contractParents.push_back(make_pair("CA-160530C-02","11024B"));
    contractParents.push_back(make_pair("CA-160530C-03","11097E"));
    contractParents.push_back(make_pair("CA-160530C-04","11159B"));
    contractParents.push_back(make_pair("CA-160530C-05","10630"));
    contractParents.push_back(make_pair("CA-160530C-06","11037D"));
    contractParents.push_back(make_pair("CA-160530C-07","11171B"));
    contractParents.push_back(make_pair("CA-160530C-08","10626B"));
    contractParents.push_back(make_pair("CA-160530C-09","05329I"));
    contractParents.push_back(make_pair("CA-160530C-10","11154C"));
    contractParents.push_back(make_pair("CA-160530C-11","11262"));
    contractParents.push_back(make_pair("CA-160530C-12","11281B"));
    contractParents.push_back(make_pair("CA-160530C-13","11132B"));
    contractParents.push_back(make_pair("CA-160530C-14","11028B"));
    contractParents.push_back(make_pair("CA-160126A-02","1030443"));
    contractParents.push_back(make_pair("CA-160126A-03","1027700"));
    contractParents.push_back(make_pair("RH-170113A","MTD03"));
    contractParents.push_back(make_pair("BA-160921A","PC8000R4"));
    contractParents.push_back(make_pair("BA-160902B","410-10002-01"));
    contractParents.push_back(make_pair("BA-161125B-01","EP120-LSI00194"));
    contractParents.push_back(make_pair("BA-161125B-02","EP120-LSI00194"));
    contractParents.push_back(make_pair("BA-170216A","04650-005"));
    contractParents.push_back(make_pair("BA-170418B-INTERIM","104189"));
    contractParents.push_back(make_pair("BA-170109A-01","10791"));
    contractParents.push_back(make_pair("BA-170109A-02","10390"));
    contractParents.push_back(make_pair("BA-170109A-03","10390"));
    contractParents.push_back(make_pair("RH-161101B","2-SL-02-300000-21"));
    contractParents.push_back(make_pair("CA-160608A-01","2-51-10391-0"));
    contractParents.push_back(make_pair("CA-160608A-02","3-80-00200-4"));
    contractParents.push_back(make_pair("CA-160505D-01","106835"));
    contractParents.push_back(make_pair("CA-160505D-02","106836"));
    contractParents.push_back(make_pair("CA-160505D-03","105321"));
    contractParents.push_back(make_pair("BA-170418B","104189"));
    contractParents.push_back(make_pair("BA-170308B","X-311052"));
    contractParents.push_back(make_pair("BA-170308B-03","9A-50160002"));
    contractParents.push_back(make_pair("BA-170308B-04","9A-30160010"));
    contractParents.push_back(make_pair("BA-170308B-05","X-370239"));
    contractParents.push_back(make_pair("BA-170308B-06","X-370249"));
    contractParents.push_back(make_pair("BA-170125A","11-534-104-GEN-30"));
    contractParents.push_back(make_pair("BA-161205A-01","E166202-1"));

    QSqlQuery readQuery;
    QSqlQuery writeQuery;
    QString rQstring;
    QString wQstring;

    for(auto& e1: contractParents){
        for(auto& e2: types){
            string s = "SELECT MFR, MPN, contractTitle FROM ContractsComponents NATURAL JOIN Components ";
            s += "WHERE ContractsComponents.contractTitle = '";
            s += e1.first;
            s += "' AND Components.MaterialType = '";
            s += e2;
            s += "';";
            rQstring = QString::fromStdString(s);

            if (!readQuery.exec(rQstring)){
                 qDebug() << "updateParents READ SQL error: "<< readQuery.lastError().text() << endl;
                 return;
            }

            while(readQuery.next()){
                wQstring = QString::fromStdString("UPDATE ContractsComponents SET Parent = '" + e1.second + "' WHERE "
                        + "MFR = '" + readQuery.value(0).toString().toStdString() + "' AND "
                        + "MPN = '" + readQuery.value(1).toString().toStdString() + "' AND "
                        + "contractTitle = '" + readQuery.value(2).toString().toStdString() + "';");
                qDebug() << wQstring;

                if(!writeQuery.exec(wQstring)){
                    qDebug() << "updateParents write SQL error: "<< writeQuery.lastError().text() << endl;
                    return;
                }

            }

        }
    }





}
