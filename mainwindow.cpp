#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
//#include "parser.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    qDebug() << "Window is open";

    control = Controller();

    ////This section is for testing
    /// parser class is deprecated
    //Parser* parser = new Parser();
    //parser->test();


}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    control.handleTokenizeRequest();
}
