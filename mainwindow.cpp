#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QGraphicsLineItem>

#include <QDesktopWidget>

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
//    scene->addLine(-500, 0, 500,0);
//    scene->addLine(0, -500, 0,500);
    ui->graphicsView->setScene(scene);

//    dialogFlag = false;

    micro = 119;

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

    scene->clear();
    QDesktopWidget desktop;

    scene->addLine(-(desktop.width())/2+25, 0, (desktop.width())/2-25,0);


    if((ui->tabWidget->currentIndex())>=0){
        for (int i=0; i< ui->tabWidget->count();i++){
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes();
        }

        QString str="";
        int numberOfChromosomes =
                ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();
        for (int i=0; i< ui->tabWidget->count();i++){
            str += "\n--------\ntab "+ QString::number(i)+"\n";
            for(int j=0; j< numberOfChromosomes; j++){
                str += "chromosome length"+QString::number(j)+": "+
                        QString::number(pixToMicro(tabsChromosomes[i][j].getChromosomeLength()))+"\n"+
                        QString::number((tabsChromosomes[i][j].getChromosomeLength()))+"\n";
                drawChromosome(j*40,0,tabsChromosomes[i][j]);
            }
//            scene->addRect(i*40,0,20,tabsChromosomes[i]->getChromosomeLength(),QPen(Qt::blue));
        }
        QMessageBox::information(this, tr("Master Measure"),str);

//        scene->addRect(0,0,20,tabsChromosomes[0][0].getChromosomeLength(),QPen(Qt::blue));
    }

}

void MainWindow::drawChromosome(int x, int y, chromosome myChromosome){

    //add wing 1
    scene->addRect(x, y-myChromosome.getChromosomeWing1Length()
                   , 20 , myChromosome.getChromosomeWing1Length());

    //add head
    scene->addRect(x, y-myChromosome.getChromosomeWing1Length()
                   , 20 , myChromosome.getChromosomeHeadLength());

    //add wing 2
    scene->addRect(x, y
                   ,20, myChromosome.getChromosomeWing2Length());

    //add tail
    scene->addRect(x,y+myChromosome.getChromosomeWing2Length()-myChromosome.getChromosomeTailLength()
                   , 20, myChromosome.getChromosomeTailLength());

}

double MainWindow::pixToMicro(double pix){
    return pix / (scale*micro/40);
}


void MainWindow::on_calibrateButton_clicked()
{
    bool flag = false;
    double temp = 0;
    if (ui->tabWidget->count()== 0){
        this->on_actionOpen_triggered();
        flag = true;
    }

    if((ui->tabWidget->currentIndex())>=0){
        for (int i=0; i< ui->tabWidget->count();i++){
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes();
        }

        QString str="";
        int numberOfChromosomes =
                ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();

        for(int j=0; j< numberOfChromosomes; j++){
            str += "chromosome length"+QString::number(j)+": \n"+
                    QString::number((tabsChromosomes[ui->tabWidget->currentIndex()][j].getChromosomeLength()))+"\n";

            temp += (tabsChromosomes[ui->tabWidget->currentIndex()][j].getChromosomeLength());
        }
        micro = temp/numberOfChromosomes;
        if (!flag)
            QMessageBox::information(this, tr("Master Measure"),QString::number(micro));

    }



//    ui->calibrateButton->setVisible(false);

}
