#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QGraphicsLineItem>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    maxTab = 100;
    tabsChromosomes = new chromosome* [maxTab];

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {

        tabView = new TabView(fileName);

        ui->tabWidget->addTab(tabView,"someTab");

        QFileInfo fileInfo = fileName;
        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());

        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);


    }
}


//closeEvent overrided to show a properiate message before close
void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton msg;
    msg = QMessageBox::warning(this,"Master Measure","Are you sure you want to close?"
                               ,QMessageBox::Yes | QMessageBox::No);
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


void MainWindow::on_showButton_clicked()
{

//    chromosome * myChro;
////    myChro = tabView->getSortedChromosomes();
//    myChro = ((TabView*)ui->tabWidget->currentWidget())->getChromosomes();
//    int numberOfChromosomes = ((TabView*)ui->tabWidget->currentWidget())->getNumberOfChromosomes();

//    QString str="";
//    for (int i=0; i<numberOfChromosomes;i++){
//        str += "chromosome "+QString::number(i)+": "+
//                QString::number(myChro[i].getChromosomeLength())+"\n";
//    }
//    QMessageBox::information(this, tr("Master Measure"),str);
//=======================
//    QString str="";
//    for (int i=0; i<=numberOfTabs;i++){
//        str += "tab "+QString::number(i)+": "+
//                QString::number(tabsChromosomes[i][0].getChromosomeLength())+"\n";
//    }
//    QMessageBox::information(this, tr("Master Measure"),str);
//    =====================


    for (int i=0; i< ui->tabWidget->count();i++){
        tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes();
    }

    QString str="";
    int numberOfChromosomes = ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();
    for (int i=0; i< ui->tabWidget->count();i++){
        str += "\n--------\ntab "+ QString::number(i)+"\n";
        for(int j=0; j< numberOfChromosomes; j++){
            str += "chromosome length"+QString::number(j)+": "+
                    QString::number(tabsChromosomes[i][j].getChromosomeLength())+"\n";
        }
    }
    QMessageBox::information(this, tr("Master Measure"),str);

}
