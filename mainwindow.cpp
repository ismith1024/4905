#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
//#include "parser.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    qDebug() << "Window is open";

    control = Controller();

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    control.handleTokenizeRequest();
}

void MainWindow::on_pushButton_10_clicked(){
    exit(0);
}

void MainWindow::on_pushButton_2_clicked(){
    control.testTopicExtraction();
}

void MainWindow::on_pushButton_3_clicked(){
    control.testClassifyingString();
}

void MainWindow::on_pushButton_4_clicked(){
    control.testFindCollocations();
}

void MainWindow::on_pushButton_11_clicked(){
    control.testClassifyCollocations();
}

void MainWindow::on_pushButton_12_clicked(){
    control.testNounPhrases();
}

void MainWindow::on_pushButton_13_clicked(){
    control.testVerbPhrases();
}

void MainWindow::on_pushButton_14_clicked(){
    control.testClassifySupplier();
}

void MainWindow::on_pushButton_15_clicked(){
    control.testEntityDeduplication();
}
