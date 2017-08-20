#include "parser.h"


Parser::Parser(){
    workingPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/files";

}

QStringList&/*<QStringRef>*/ Parser::parse(QString& st, QString& ch, QString& dir){
    QStringList ret = st.split(ch);
    //QVector<QStringRef> pieces = st.split(ch);
    //QStringList<QStringRef> ret = pieces.toList();
    //delete pieces;

    //open all files in the working directory
    //Given by paramater dir
    //QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);


    qDebug() << QDir::currentPath(); // directory;

    return ret;
}

void Parser::test(){
    qDebug() << "Parser test method" << "\n--------------------\n";
    qDebug() << workingPath;

}
