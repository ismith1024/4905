#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringRef>
#include <QVector>
#include <QList>

using namespace std;

class Parser{

public:
    Parser();
    QStringList& parse(QString&, QString&); //breaks a text stream into tokens

};




#endif // PARSER_H
