#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <iostream>
#include "utilityalgorithms.h"
#include "repository.h"
#include "hcluster.h"
#include<map>
#include<list>
#include "bayesianstringclassifier.h"
#include"languageprocessor.h"
#include"controller.h"
//#include "component.h"

using namespace std;

//code for the GUI app
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
