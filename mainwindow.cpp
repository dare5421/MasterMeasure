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

    scaleDialog = new ScaleDialog(this);

    scaleDialog->show();

    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    maxTab = 100;
    tabsChromosomes = new chromosome* [maxTab];

    scene = new QGraphicsScene(this);
//    scene->addRect(0,0,10,10,QPen(Qt::blue));
//    scene->addRect(0,-20,10,20,QPen(Qt::red));
    scene->addLine(-500, 0, 500,0);
    scene->addLine(0, -500, 0,500);
    ui->graphicsView->setScene(scene);



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
    scale = scaleDialog->getScale();



    if((ui->tabWidget->currentIndex())>=0){
        for (int i=0; i< ui->tabWidget->count();i++){
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes();
        }

        QString str="";
        int numberOfChromosomes = ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();
        for (int i=0; i< ui->tabWidget->count();i++){
            str += "\n--------\ntab "+ QString::number(i)+"\n";
            for(int j=0; j< numberOfChromosomes; j++){
                str += "chromosome length"+QString::number(j)+": "+
                        QString::number(pixToMicro(tabsChromosomes[i][j].getChromosomeLength()))+"\n"+
                        QString::number((tabsChromosomes[i][j].getChromosomeLength()))+"\n";

                scene->addRect(j*40,0,20,tabsChromosomes[i][j].getChromosomeLength(),QPen(Qt::blue));
            }
        }
        QMessageBox::information(this, tr("Master Measure"),str);

//        scene->addRect(0,0,20,tabsChromosomes[0][0].getChromosomeLength(),QPen(Qt::blue));
    }

}

double MainWindow::pixToMicro(double pix){
    return pix / (scale*3);
}
