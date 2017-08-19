#include "parser.h"


Parser::Parser(){

}

QStringList&/*<QStringRef>*/ Parser::parse(QString& st, QString& ch){
    QStringList ret = st.split(ch);
    //QVector<QStringRef> pieces = st.split(ch);
    //QStringList<QStringRef> ret = pieces.toList();
    //delete pieces;
    return ret;
}
