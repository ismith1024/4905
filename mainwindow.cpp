#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
//#include "parser.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    qDebug() << "Window is open";

    //control = Controller();

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    cout << "Tokenize text" << endl;
    control.handleTokenizeRequest();
}

void MainWindow::on_pushButton_10_clicked(){
    cout << "Bye!" << endl;
    exit(0);
}

void MainWindow::on_pushButton_2_clicked(){
    cout << "Testing topic extraction" << endl;
    control.testTopicExtraction();
}

void MainWindow::on_pushButton_3_clicked(){
    cout << "Testing string classification" << endl;
    control.testClassifyMatType();
}

void MainWindow::on_pushButton_4_clicked(){
    cout << "Testing finding collocations" << endl;
    control.testFindCollocations();
}

void MainWindow::on_pushButton_11_clicked(){
    cout << "Testing classifying a collocation" << endl;;
    control.testClassifyCollocations();
}

void MainWindow::on_pushButton_12_clicked(){
    cout << "Testing noun phrase extraction" << endl;
    control.testNounPhrases();
}

void MainWindow::on_pushButton_13_clicked(){
    cout << "Testing verb phrase extraction" << endl;
    control.testVerbPhrases();
}

void MainWindow::on_pushButton_14_clicked(){
    cout << "Testing supplier classification given part number and article type" << endl;
    control.testClassifySupplier();
}

void MainWindow::on_pushButton_15_clicked(){
    cout << "Testing entity deduplication" << endl;
    control.testGetMIM();
}

void MainWindow::on_pushButton_5_clicked(){
    int tcNum = ui->testCaseSpinBox->cleanText().toInt();
    cout << "Run full test case number " << tcNum << endl;
    control.runTestCase(tcNum);
}

void MainWindow::on_pushButton_6_clicked(){
    int tcNum = ui->testCaseSpinBox->cleanText().toInt();
    cout << "Clean test case number " << tcNum << endl;
    control.cleanTestCase(tcNum);
}

void MainWindow::on_pushButton_7_clicked()
{
    control.testParent();
}

void MainWindow::on_pushButton_8_clicked()
{
    control.runOneOf();
}

void MainWindow::on_pushButton_9_clicked()
{
    control.testBuildComponents();
}
