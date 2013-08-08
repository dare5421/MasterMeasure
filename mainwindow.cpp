#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>



#include <QDesktopWidget>
#include <QPolygonF>
#include <QGraphicsTextItem>
#include <QPointF>

#include <QtMath>
//#include <QPixmap>
//#include <QImage>
#include <QSvgGenerator>

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
    //    ui->graphicsView->setSceneRect(0,0,1000,1000);

    //    dialogFlag = false;

    micro = 11.9;

    errorBarWing1 = new double [1000]();
    errorBarWing2 = new double [1000]();


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {

        tabView = new TabView(fileName, micro);

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

    for(int j=0; j< 1000; j++){
        errorBarWing1[j] = 0;
        errorBarWing2[j] = 0;
    }

    scale = scaleDialog->getScale();

    scene->clear();


    if((ui->tabWidget->currentIndex())>=0){
        for (int i=0; i< ui->tabWidget->count();i++){
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes();
        }


        int numberOfChromosomes =
                ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();

        double avgWing1[1000]={0};
        double avgWing2[1000]={0};
        for (int i=0; i< ui->tabWidget->count();i++){
            for(int j=0; j< numberOfChromosomes; j++){
                avgWing1[j] += tabsChromosomes[i][j].getChromosomeWing1Length();
                avgWing2[j] += tabsChromosomes[i][j].getChromosomeWing2Length();
            }
        }

        for(int j=0; j< numberOfChromosomes; j++){
            avgWing1[j]/= ui->tabWidget->count();
            avgWing2[j]/= ui->tabWidget->count();
        }


        double sigmaWing1[1000]={0};
        double sigmaWing2[1000]={0};
        for (int i=0; i< ui->tabWidget->count();i++){
            for(int j=0; j< numberOfChromosomes; j++){
                sigmaWing1[j] += qPow((tabsChromosomes[i][j].getChromosomeWing1Length() - avgWing1[j]),2);
                sigmaWing2[j] += qPow((tabsChromosomes[i][j].getChromosomeWing2Length() - avgWing2[j]),2);
            }
        }

        for(int j=0; j< numberOfChromosomes; j++){
            if(numberOfChromosomes == 1){
                sigmaWing1[j] = 0;
                sigmaWing2[j] = 0;
            }
            else{
                sigmaWing1[j] /= (numberOfChromosomes-1);
                sigmaWing2[j] /= (numberOfChromosomes-1);
            }
        }

        for (int i=0; i< ui->tabWidget->count();i++){
            for(int j=0; j< numberOfChromosomes; j++){
                errorBarWing1[j] = qSqrt(sigmaWing1[j]) / qSqrt(numberOfChromosomes);
                errorBarWing2[j] = qSqrt(sigmaWing2[j]) / qSqrt(numberOfChromosomes);
            }
        }


        //draw in bottom
        for(int j=0; j< numberOfChromosomes; j++){
            if (j>0)
                scene->addLine((j-1)*70+15,0,j*70+5,0);
            drawChromosome(j*70,0, avgWing1[j],avgWing2[j], errorBarWing1[j], errorBarWing2[j]);

            //            QMessageBox::information(this, tr("Master Measure"),QString::number(errorBarWing1[j]));

        }


        //        QMessageBox::information(this, tr("Master Measure"),str);

        //        scene->addRect(0,0,20,tabsChromosomes[0][0].getChromosomeLength(),QPen(Qt::blue));
    }

}

void MainWindow::drawChromosome(int x, int y, double wing1, double wing2,double errorWing1,double errorWing2){

    QPolygonF *polygon = new QPolygonF();
    polygon->append((QPoint(x+0,-5)));
    polygon->append((QPoint(x+20,-5)));
    polygon->append((QPoint(x+10,0)));
    polygon->append((QPoint(x+0,5)));
    polygon->append((QPoint(x+20,5)));

    scene->addPolygon(*polygon,QPen(Qt::black),QBrush(Qt::black));

    QGraphicsTextItem *textWing1 = new QGraphicsTextItem;
    QGraphicsTextItem *textWing2 = new QGraphicsTextItem;
    textWing1->setPlainText(QString::number(pixToMicro(wing1)));
    textWing2->setPlainText(QString::number(pixToMicro(wing2)));
    //    textWing1->setPlainText(QString::number((wing1)));
    //    textWing2->setPlainText(QString::number((wing2)));

    QPointF wing1Pos= QPointF(x+20,-30);
    QPointF wing2Pos= QPointF(x+20,10);


    //add wing 1 is up
    scene->addRect(x, y-wing1-5, 20 , wing1);

    textWing1->setPos(wing1Pos);
    scene->addItem(textWing1);

    // add error bar wing1
    scene->addLine(x+8, y-wing1-5-errorWing1, x+12, y-wing1-5-errorWing1);
    scene->addLine(x+10, y-wing1-5-errorWing1, x+10, y-wing1-5+errorWing1);
    scene->addLine(x+8, y-wing1-5+errorWing1, x+12, y-wing1-5+errorWing1);


    //add wing 2 is down
    scene->addRect(x, y+5, 20, wing2);

    textWing2->setPos(wing2Pos);
    scene->addItem(textWing2);

    // add error bar wing2
    scene->addLine(x+8, y+wing2+5-errorWing2, x+12, y+wing2+5-errorWing2);
    scene->addLine(x+10, y+wing2+5-errorWing2, x+10, y+wing2+5+errorWing2);
    scene->addLine(x+8, y+wing2+5+errorWing2, x+12, y+wing2+5+errorWing2);



    //    //add head
    //    scene->addRect(x, y-myChromosome.getChromosomeWing1Length()
    //                   , 20 , myChromosome.getChromosomeHeadLength());



    //    //add tail
    //    scene->addRect(x,y+myChromosome.getChromosomeWing2Length()-myChromosome.getChromosomeTailLength()
    //                   , 20, myChromosome.getChromosomeTailLength());

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
        micro /= 10;
        if (!flag)
            QMessageBox::information(this, tr("Master Measure"),QString::number(micro));

    }



    //    ui->calibrateButton->setVisible(false);

}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Scene", "", "vector image (*.svg)");
    //    QPixmap pixMap = QPixmap::grabWidget(ui->graphicsView);
    //    pixMap.save(fileName);

    //===========================
    //    scene->clearSelection();
    //    scene->setSceneRect(scene->itemsBoundingRect());
    //    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    //    image.fill(Qt::transparent);

    //    QPainter painter(&image);
    //    scene->render(&painter);
    //    image.save(fileName);
    //===========================

    QSvgGenerator generator;
    generator.setFileName(fileName );
    generator.setSize(QSize(200, 200));
    generator.setViewBox(QRect(0, 0, 200, 200));
    generator.setTitle(tr("SVG Generator Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                "Chromosome Drawing."));
    QPainter painter(&generator);

    scene->render(&painter);


}

double MainWindow::getMicro() const
{
    return micro;
}

void MainWindow::setMicro(double value)
{
    micro = value;
}

void MainWindow::on_actionAbout_triggered()
{
//    QMessageBox::about(this,"About MasterMeasure", "MasterMeasure v0.9\n\n(c) 2013 Dariush Zandi");
    QMessageBox::about(this, tr("About Master Measure"),
              tr("<p><b>Master Measure</b> v0.9 </p>"
                 "<p>(C) 2013 Dariush Zandi </p>"
                 "<p>"
                    "<a href=\"mailto:dariush.zandi.n@gmail.com\"><img src=\":/about/email.png\"></a>"
                    "<a href=\"http://twitter.com/dariushzandi\"><img src=\":/about/twitter.png\"></a>"
                    "<a href=\"http://ir.linkedin.com/pub/dariush-zandi/19/789/31b\"><img src=\":/about/linkedin.png\"></a>"

                 "</p>"));
}
