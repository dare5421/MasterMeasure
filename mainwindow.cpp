#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QGraphicsLineItem>

#include "tabview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {

        TabView *view = new TabView(fileName);

        ui->tabWidget->addTab(view,"someTab");

        QFileInfo fileInfo = fileName;
        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());

        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

     }
}


//closeEvent overrided to show a properiate message before close
void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton msg;
    msg = QMessageBox::warning(this,"Master Measure","Are you sure you want to close?",QMessageBox::Yes | QMessageBox::No);
    if (msg == QMessageBox::Yes){
        event->accept();
    }
    else
        event->ignore();

}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

