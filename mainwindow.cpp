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

#include <QSvgGenerator>

#include <QColorDialog>
#include <QInputDialog>

#include <QtCore/QTextStream>
#include <QtCore/QFile>

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

    ui->graphicsView->setScene(scene);

    micro = 11.9;

//    errorBarWing1 = new double [max_size]();
//    errorBarWing2 = new double [max_size]();

    createActions();

//    manualFlag = false;
//    ui->actionAuto->setChecked(true);
    manualFlag = true;
    ui->actionManual->setChecked(true);


    //disable all action, except open file or open tab
    ui->actionSave->setEnabled(false);
    ui->actionLine_Color->setEnabled(false);
    ui->actionLine_Width->setEnabled(false);
    ui->actionSave_Tab->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (!fileName.isEmpty()) {

        tabView = new TabView(fileName, micro);

        ui->tabWidget->addTab(tabView,"someTab");

        QFileInfo fileInfo = fileName;
        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());

        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

        tabView->setManualFlag(manualFlag);

        //Enable all actions by open first tab
        ui->actionSave->setEnabled(true);
        ui->actionLine_Color->setEnabled(true);
        ui->actionLine_Width->setEnabled(true);
        ui->actionSave_Tab->setEnabled(true);

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

void MainWindow::contextMenuEvent(QContextMenuEvent  *event)
{
    QMenu menu;

//    menu.addAction("this is it");
    menu.addAction(saveAction);
//    menu.addAction(centerAction);
//    menu.addAction(endAction);

    menu.exec(event->globalPos());
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

// =============================== show bottom things ===========================
void MainWindow::on_showButton_clicked()
{

    const int max_size = 500;
    scale = scaleDialog->getScale();

    scene->clear();

    if((ui->tabWidget->currentIndex())>=0){
        for (int i=0; i< ui->tabWidget->count();i++){
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes(manualFlag);
        }


        int numberOfChromosomes =
                ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();

//        double avgWing1[max_size]={0};
//        double avgWing2[max_size]={0};

        double avgWing1[26][max_size]={0};
        double avgWing2[26][max_size]={0};

        double errorBarWing1[26][max_size]={0};
        double errorBarWing2[26][max_size]={0};

        double satellite1[26][max_size]={0};
        double satellite2[26][max_size]={0};
        double avgTotalLength[26][max_size]={0};

        double minTotalLength[26][max_size]={0};
        double maxTotalLength[26][max_size]={0};

        QList<double> wing1[26][max_size];
        QList<double> wing2[26][max_size];
        QList<double> chromosomeLength[26][max_size];

        double allTotalLength[26][max_size]={0};    // sum of lengths of chromosomes
        double allShortLength[26][max_size]={0};
        double sigmaWing1[26][max_size]={0};
        double sigmaWing2[26][max_size]={0};
        double sigmaTotalLength[26][max_size]={0};
        double errorBarTotalLength[26][max_size]={0};



        QTableWidget *myTable = ui->tableWidget;
        myTable->setRowCount(0);

        double maxChromosomeLength = 0;

        if(!manualFlag)//============================ Auto =========================
        {
            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){
                    avgWing1[0][j/2] += tabsChromosomes[i][j].getChromosomeWing1Length();
                    avgWing2[0][j/2] += tabsChromosomes[i][j].getChromosomeWing2Length();

                    maxChromosomeLength = tabsChromosomes[i][j].getChromosomeLength()>maxChromosomeLength?
                                tabsChromosomes[i][j].getChromosomeLength():maxChromosomeLength;

                    avgTotalLength[0][j/2] += tabsChromosomes[i][j].getChromosomeLength();

                    if(j%2 == 0)
                        minTotalLength[0][j/2]= tabsChromosomes[i][j].getChromosomeLength();


                    if(tabsChromosomes[i][j].getChromosomeLength() < minTotalLength[0][j/2]){
                        minTotalLength[0][j/2]= tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeLength() > maxTotalLength[0][j/2]){
                        maxTotalLength[0][j/2]= tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeHeadLength()!= 0){
                        satellite1[0][j/2] += tabsChromosomes[i][j].getChromosomeHeadLength();
                    }
                    if(tabsChromosomes[i][j].getChromosomeTailLength()!= 0){
                        satellite2[0][j/2] += tabsChromosomes[i][j].getChromosomeTailLength();
                    }
                }
            }



            for(int j=0; j< numberOfChromosomes/2; j++){

                allShortLength[0][j] = avgWing1[0][j];

                avgWing1[0][j]/= (2*ui->tabWidget->count());
                avgWing2[0][j]/= (2*ui->tabWidget->count());

                allTotalLength[0][j] = avgTotalLength[0][j];

                avgTotalLength[0][j]/=(2*ui->tabWidget->count());

                satellite1[0][j]/= (2*ui->tabWidget->count());
                satellite2[0][j]/= (2*ui->tabWidget->count());
            }




            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){
                    sigmaWing1[0][j] += qPow((tabsChromosomes[i][j].getChromosomeWing1Length() - avgWing1[0][j]),2);
                    sigmaWing2[0][j] += qPow((tabsChromosomes[i][j].getChromosomeWing2Length() - avgWing2[0][j]),2);

                    sigmaTotalLength[0][j] += qPow((tabsChromosomes[i][j].getChromosomeLength() - avgTotalLength[0][j]),2);
                }
            }

            for(int j=0; j< numberOfChromosomes; j++){
                if(ui->tabWidget->count()<2){
                    sigmaWing1[0][j] = 0;
                    sigmaWing2[0][j] = 0;

                    sigmaTotalLength[0][j] = 0;
                }
                else{
                    sigmaWing1[0][j] /= ((2*ui->tabWidget->count())-1);
                    sigmaWing2[0][j] /= ((2*ui->tabWidget->count())-1);

                    sigmaTotalLength[0][j] /= ((2*ui->tabWidget->count())-1);
                }
            }


            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){
                    errorBarWing1[0][j] = qSqrt(sigmaWing1[0][j]) / qSqrt((2*ui->tabWidget->count()));
                    errorBarWing2[0][j] = qSqrt(sigmaWing2[0][j]) / qSqrt((2*ui->tabWidget->count()));

                    errorBarTotalLength[0][j] = qSqrt(sigmaTotalLength[0][j]) / qSqrt((2*ui->tabWidget->count()));
                }
            }

            //draw in bottom
            for(int j=0; j< numberOfChromosomes/2; j++){
                if (j>0)
                    scene->addLine((j-1)*70+15,0,j*70+5,0);
                drawChromosome(j*70,0,0,
                               avgWing1[0][j] * 150 / maxChromosomeLength,avgWing2[0][j]* 150 / maxChromosomeLength,
                               errorBarWing1[0][j], errorBarWing2[0][j],
                               (satellite1[0][j] > satellite2[0][j])?satellite1[0][j]:satellite2[0][j] ,
                               (satellite1[0][j] > satellite2[0][j])?true:false  );

                //============ add chromosomes to table ================
                myTable->insertRow(myTable->rowCount());


                myTable->setItem(j, 0,new QTableWidgetItem("Chromosome "+QString::number(j+1)) );

                myTable->setItem(j, 1, new QTableWidgetItem(QString::number(avgWing1[0][j])+" " +177+" " + QString::number(errorBarWing1[0][j])));

                myTable->setItem(j, 2, new QTableWidgetItem(QString::number(avgWing2[0][j])+" " +177+" " + QString::number(errorBarWing2[0][j])));

                myTable->setItem(j, 3, new QTableWidgetItem(QString::number(avgWing2[0][j]+avgWing1[0][j])+" " +177+" " + QString::number(errorBarTotalLength[0][j])));

                myTable->setItem(j, 4, new QTableWidgetItem(QString::number(avgWing2[0][j] / avgWing1[0][j])));

                myTable->setItem(j, 5, new QTableWidgetItem(QString::number(avgWing1[0][j] / avgWing2[0][j])));

                myTable->setItem(j, 6, new QTableWidgetItem(QString::number(allTotalLength[0][j])));

                myTable->setItem(j, 7, new QTableWidgetItem("% "+QString::number((avgWing2[0][j]+avgWing1[0][j])*100.0/allTotalLength[0][j])));

                myTable->setItem(j, 8, new QTableWidgetItem("% "+QString::number(allShortLength[0][j]*100.0/allTotalLength[0][j])));

                myTable->setItem(j, 9, new QTableWidgetItem("% "+QString::number(minTotalLength[0][j]*100.0/maxTotalLength[0][j])));
            }


        }
        else{ // =================== manual ==============================

            int countChromosome[26][max_size]={0};//count chromosomes with same index
            int numChro = 0;

            QSet<int> chromosomesIndex; //number of different index

            QSet<int> chromosomesGenome;//number of defferent genome

            QSet<int> chromosomesTopIndex;//max number of chromosomes in genomes


            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){

                    maxChromosomeLength = tabsChromosomes[i][j].getChromosomeLength()> maxChromosomeLength?
                                tabsChromosomes[i][j].getChromosomeLength():maxChromosomeLength;// !!!!!! آیا نباید از آرایه 26 تایی استفاده کرد !!!!!

                    int index = tabsChromosomes[i][j].getIndex();

                    avgWing1[index / 1000 - 1][index % 1000 - 1] +=
                            tabsChromosomes[i][j].getChromosomeWing1Length();

                    avgWing2[index / 1000 - 1][index % 1000 - 1] +=
                            tabsChromosomes[i][j].getChromosomeWing2Length();

                    avgTotalLength[index / 1000 - 1][index % 1000 - 1] +=
                            tabsChromosomes[i][j].getChromosomeLength();

                    countChromosome[index / 1000 - 1][index % 1000 - 1]++;

                    wing1[index / 1000 - 1][index % 1000 - 1]<<tabsChromosomes[i][j].getChromosomeWing1Length();
                    wing2[index / 1000 - 1][index % 1000 - 1]<<tabsChromosomes[i][j].getChromosomeWing2Length();
                    chromosomeLength[index / 1000 - 1][index % 1000 - 1]<<tabsChromosomes[i][j].getChromosomeLength();

//                    if( (index)%1000 > numChro)
//                        numChro =  (index)%1000;

                    chromosomesIndex << index;
                    chromosomesGenome << (index / 1000);
                    chromosomesTopIndex << (index % 1000);


                    if(j == 0 || ( index % 1000 != tabsChromosomes[i][j-1].getIndex()))
                        minTotalLength[index / 1000 - 1][index % 1000 - 1] = tabsChromosomes[i][j].getChromosomeLength();


                    if(tabsChromosomes[i][j].getChromosomeLength() < minTotalLength[index / 1000 - 1][index % 1000 - 1]){
                        minTotalLength[index / 1000 - 1][index % 1000 - 1] = tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeLength() > maxTotalLength[index / 1000 - 1][index % 1000 - 1]){
                        maxTotalLength[index / 1000 - 1][index % 1000 - 1] = tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeHeadLength()!= 0){
                        satellite1[index / 1000 - 1][index % 1000 - 1] += tabsChromosomes[i][j].getChromosomeHeadLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeTailLength()!= 0){
                        satellite2[index / 1000 - 1][index % 1000 - 1] += tabsChromosomes[i][j].getChromosomeTailLength();
                    }

                }
            }

            numChro = chromosomesIndex.size();
            int numberOfGenome = chromosomesGenome.size();
            int topIndexOfGenomes = chromosomesTopIndex.size();


//!!!!!!!! ATTENTION TO THIS PART. IT NEED MORE ATTENTION !!!!!!!!!

            for (int i=0; i<numberOfGenome;i++){
                for(int j=0; j< topIndexOfGenomes; j++){

                    if(avgWing1[i][j] && avgWing2[i][j] && countChromosome[i][j]){

                        allShortLength[i][j] = avgWing1[i][j]; // what about long wings ??????????!!!!!!!!!

                        allTotalLength[i][j] = avgTotalLength[i][j];

                        avgWing1[i][j] /= countChromosome[i][j];
                        avgWing2[i][j] /= countChromosome[i][j];

                        avgTotalLength[i][j]/= countChromosome[i][j];

                        satellite1[i][j]/= countChromosome[i][j];
                        satellite2[i][j]/= countChromosome[i][j];


                        errorBarWing1[i][j] = standardError(countChromosome[i][j],wing1[i][j]);
                        errorBarWing2[i][j] = standardError(countChromosome[i][j],wing2[i][j]);
                        errorBarTotalLength[i][j] = standardError(countChromosome[i][j],chromosomeLength[i][j]);


                    }

                }
            }

            //////////////////////////////////////////////////////

//            for (int i=0; i < ui->tabWidget->count();i++){
//                for(int j=0; j < numChro; j++){

//                    sigmaWing1[0][j] += qPow((tabsChromosomes[i][j].getChromosomeWing1Length() - avgWing1[0][j]),2);
//                    sigmaWing2[0][j] += qPow((tabsChromosomes[i][j].getChromosomeWing2Length() - avgWing2[0][j]),2);

//                    sigmaTotalLength[0][j] += qPow((tabsChromosomes[i][j].getChromosomeLength() - avgTotalLength[0][j]),2);
//                }
//            }


            //////////////////////////////////////////////////////

//            for(int j=0; j< numChro; j++){

//                sigmaWing1[0][j] /= (countChromosome[0][j]-1);
//                sigmaWing2[0][j] /= (countChromosome[0][j]-1);

//                sigmaTotalLength[0][j] /= (countChromosome[0][j]-1);

//            }

//            for (int i=0; i< ui->tabWidget->count();i++){
//                for(int j=0; j< numChro; j++){
//                    errorBarWing1[0][j] = qSqrt(sigmaWing1[0][j]) / qSqrt(countChromosome[0][j]);
//                    errorBarWing2[0][j] = qSqrt(sigmaWing2[0][j]) / qSqrt(countChromosome[0][j]);

//                    errorBarTotalLength[0][j] = qSqrt(sigmaTotalLength[0][j]) / qSqrt(countChromosome[0][j]);
//                }
//            }

//!!!!!!!!!!!!!!!!!

//            draw in bottom


            int genomeLine = -1;
//            QMessageBox::information(this, tr("Master Measure"),QString::number(numberOfGenome)
//                                     +"  "+QString::number(numChro)
//                                      +"  "+QString::number(topIndex)
//                                     );


//            for(int i=0; i<numberOfGenome; i++)
            for(int i=0; i<26; i++)
            for(int j=0; j< topIndexOfGenomes ; j++){
                if(avgWing1[i][j]+avgWing2[i][j]){
    //                genome = (tabsChromosomes[0][j].getIndex())/1000-1;

//                    QMessageBox::information(this, tr("Master Measure"),QString::number(i)+"  "+QString::number(j));
                    if (j>0)
    //                if((tabsChromosomes[0][j].getIndex())%1000 > 1)
                        scene->addLine((j-1)*70+15,genomeLine * 300,j*70+5,genomeLine * 300);
                    else genomeLine++;

                    drawChromosome(j*70,genomeLine * 300,i,
                                   avgWing1[i][j] * 150 / maxChromosomeLength, avgWing2[i][j]* 150 / maxChromosomeLength,
                                   errorBarWing1[0][j], errorBarWing2[0][j],
                                   (satellite1[i][j] > satellite2[i][j])?satellite1[i][j]:satellite2[i][j],
                                   (satellite1[i][j] > satellite2[i][j])?true:false);

    //                drawChromosome(j*70,i * 300,
    //                               avgWing1[i][j] * 150 / maxChromosomeLength, avgWing2[i][j]* 150 / maxChromosomeLength,
    //                               0, 0,
    //                               0,
    //                               false  );



    //                if (j>0)
    //                    scene->addLine((j-1)*70+15,0,j*70+5,0);
    //                drawChromosome(j*70,0,
    //                               avgWing1[j] * 150 / maxChromosomeLength,avgWing2[j]* 150 / maxChromosomeLength,
    //                               errorBarWing1[0][j], errorBarWing2[0][j],
    //                               (satellite1[j] > satellite2[j])?satellite1[j]:satellite2[j],
    //                               (satellite1[j] > satellite2[j])?true:false  );


                    //============ add chromosomes to table ================


                    myTable->insertRow(myTable->rowCount());


//                    myTable->setItem(j, 0,new QTableWidgetItem("Chromosome "+QString::number(j+1)) );
                    QChar genC = 'A' + i;
                    myTable->setItem(j, 0,new QTableWidgetItem(QString::number(j+1)+ genC ) );

                    myTable->setItem(j, 1, new QTableWidgetItem(QString::number(avgWing1[i][j])
                                                                +" " +177+" " + QString::number(errorBarWing1[i][j])));

    //                myTable->setItem(j, 2, new QTableWidgetItem(QString::number(avgWing2[genome][j])+" " +177+" " + QString::number(errorBarWing2[0][j])));

    //                myTable->setItem(j, 3, new QTableWidgetItem(QString::number(avgWing2[genome][j]+avgWing1[genome][j])+" " +177+" " + QString::number(errorBarTotalLength[j])));

    //                myTable->setItem(j, 4, new QTableWidgetItem(QString::number(avgWing2[genome][j] / avgWing1[genome][j])));

    //                myTable->setItem(j, 5, new QTableWidgetItem(QString::number(avgWing1[genome][j] / avgWing2[genome][j])));

    //                myTable->setItem(j, 6, new QTableWidgetItem(QString::number(allTotalLength[j])));

    //                myTable->setItem(j, 7, new QTableWidgetItem("% "+QString::number((avgWing2[genome][j]+avgWing1[genome][j])*100.0/allTotalLength[j])));

    //                myTable->setItem(j, 8, new QTableWidgetItem("% "+QString::number(allShortLength[j]*100.0/allTotalLength[j])));

    //                myTable->setItem(j, 9, new QTableWidgetItem("% "+QString::number(minTotalLength[j]*100.0/maxTotalLength[j])));

                }
            }



        }



    }


}

double MainWindow::standardError(int n, QList<double> x){

    if (n<=1)
        return 0;

    double avg = 0;
    for(int i=0; i<n; i++)
        avg+=x[i];

    avg/=n;

    double sum = 0;
    for(int i=0; i<n; i++)
        sum += qPow(x[i]-avg, 2);

    return sum / (n*(n-1));

}

void MainWindow::drawChromosome(int x, int y,int yy, double wing1, double wing2,
                                double errorWing1,double errorWing2,
                                double satellite, bool isSatUp){

    // draw centromere
    QPolygonF *polygon = new QPolygonF();
    polygon->append((QPoint(x+0,y-5)));
    polygon->append((QPoint(x+20,y-5)));
    polygon->append((QPoint(x+10,y)));
    polygon->append((QPoint(x+0,y+5)));
    polygon->append((QPoint(x+20,y+5)));

    scene->addPolygon(*polygon,QPen(Qt::black),QBrush(Qt::black));


    // draw wings lengths next to them
    QGraphicsTextItem *textWing1 = new QGraphicsTextItem;
    QGraphicsTextItem *textWing2 = new QGraphicsTextItem;
    textWing1->setPlainText(QString::number(pixToMicro(wing1)));
    textWing2->setPlainText(QString::number(pixToMicro(wing2)));

    QPointF wing1Pos= QPointF(x+20,y-30);
    QPointF wing2Pos= QPointF(x+20,y+10);

    textWing1->setPos(wing1Pos);
    scene->addItem(textWing1);

    textWing2->setPos(wing2Pos);
    scene->addItem(textWing2);


    //add wing 1 to the upon of centromere
    scene->addRect(x, y-wing1-5+satellite, 20 , wing1-satellite);

    //add satellite for wing1
    if(isSatUp && satellite){

        QPolygonF *polygon = new QPolygonF();
        polygon->append((QPoint(x+0,-wing1+satellite-15)));
        polygon->append((QPoint(x+20,-wing1+satellite-15)));
        polygon->append((QPoint(x+10,-wing1+satellite-10)));
        polygon->append((QPoint(x+0,-wing1+satellite-5)));
        polygon->append((QPoint(x+20,-wing1+satellite-5)));

        scene->addPolygon(*polygon,QPen(Qt::red),QBrush(Qt::red));

        scene->addRect(x, y-wing1-15, 20 , satellite);
    }    

    // add error bar wing1
    if(isSatUp){
        scene->addLine(x+8, y-wing1-15-errorWing1, x+12, y-wing1-15-errorWing1);
        scene->addLine(x+10, y-wing1-15-errorWing1, x+10, y-wing1-15+errorWing1);
        scene->addLine(x+8, y-wing1-15+errorWing1, x+12, y-wing1-15+errorWing1);
    }else{
        scene->addLine(x+8, y-wing1-5-errorWing1, x+12, y-wing1-5-errorWing1);
        scene->addLine(x+10, y-wing1-5-errorWing1, x+10, y-wing1-5+errorWing1);
        scene->addLine(x+8, y-wing1-5+errorWing1, x+12, y-wing1-5+errorWing1);
    }

    //add wing2 to underneath of centromere
    scene->addRect(x, y+5, 20, wing2-satellite);

    //add satellite to wing2
    if(!isSatUp  &&  satellite){
        QPolygonF *polygon = new QPolygonF();
        polygon->append((QPoint(x+0,wing2-satellite+5)));
        polygon->append((QPoint(x+20,wing2-satellite+5)));
        polygon->append((QPoint(x+10,wing2-satellite+10)));
        polygon->append((QPoint(x+0,wing2-satellite+15)));
        polygon->append((QPoint(x+20,wing2-satellite+15)));

        scene->addPolygon(*polygon, QPen(Qt::yellow),QBrush(Qt::yellow));

        scene->addRect(x, y+wing2-satellite+15, 20, satellite);
    }


    // add error bar wing2
    if(!isSatUp){
        scene->addLine(x+8, y+wing2+15-errorWing2, x+12, y+wing2+15-errorWing2);
        scene->addLine(x+10, y+wing2+15-errorWing2, x+10, y+wing2+15+errorWing2);
        scene->addLine(x+8, y+wing2+15+errorWing2, x+12, y+wing2+15+errorWing2);
    }else{
        scene->addLine(x+8, y+wing2+5-errorWing2, x+12, y+wing2+5-errorWing2);
        scene->addLine(x+10, y+wing2+5-errorWing2, x+10, y+wing2+5+errorWing2);
        scene->addLine(x+8, y+wing2+5+errorWing2, x+12, y+wing2+5+errorWing2);
    }

    // draw genomes and Index next to them
    QGraphicsTextItem *genome = new QGraphicsTextItem;
//    int gen= y/300;
    QChar cGen = 'A'+yy;
    genome->setPlainText(cGen + QString::number(x/70+1));


    QPointF genomePos= QPointF(x,y+wing2-satellite+20);


    genome->setPos(genomePos);
    scene->addItem(genome);



}

void MainWindow::createActions()
{
    saveAction = new QAction(tr("&Save Idiogram"), this);
    saveAction->setIcon(QIcon(":/image/floppy.png"));
    connect(saveAction, SIGNAL(triggered()),this, SLOT(on_actionSave_triggered()));
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
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes(false);
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
//    QMessageBox::information(this, tr("Master Measure"),QString::number(ui->tableWidget->columnCount()));

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

//    save table in a text file
    QFile f( "table.csv" );

    if( f.open( QIODevice::WriteOnly ) ){
        QTextStream ts( &f );
        QStringList strList;

        strList.clear();
        for(int i=0;i< ui->tableWidget->columnCount();i++){
            strList<<"\""+ ui->tableWidget->horizontalHeaderItem(i)->text()+"\"";
        }
        ts<<strList.join( "," )+"\n";

        for( int r = 0; r < ui->tableWidget->rowCount(); ++r ){
            strList.clear();

            for( int c = 0; c < ui->tableWidget->columnCount(); ++c ){
                strList << "\""+ ui->tableWidget->item( r, c )->text()+"\"";
            }
            ts << strList.join( "," )+"\n";
        }
        f.close();
    }


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
                 "<p>Dariush Zandi </p>"
                 "<p>"
                    "<a href=\"mailto:dariush.zandi.n@gmail.com\"><img src=\":/about/email.png\"></a>"
                    "<a href=\"http://twitter.com/dariushzandi\"><img src=\":/about/twitter.png\"></a>"
                    "<a href=\"http://ir.linkedin.com/pub/dariush-zandi/19/789/31b\"><img src=\":/about/linkedin.png\"></a>"
                    "<a href=\"http://dariushzandi.ir\"><img src=\":/about/www.png\"></a>"
                 "</p>""<p>Ghader Mirzaghaderi </p>"
                 "<p>"
                    "<a href=\"mailto:gh.mirzaghaderi@uok.ac.ir\"><img src=\":/about/email.png\"></a>"
                    "<a href=\"https://www.facebook.com/ghader.mirzaghaderi\"><img src=\":/about/facebook.png\"></a>"
                    "<a href=\"http://agri.uok.ac.ir/mirzaghaderi/\"><img src=\":/about/www.png\"></a>"
                 "</p>""<p> Khaled Mirzae</p>"
                 "<p>"
                    "<a href=\"mailto:khaled.mirzayi@gmail.com\"><img src=\":/about/email.png\"></a>"
                    "<a href=\"http://www.linkedin.com/pub/khaled-mirzaei/54/a54/a3b\"><img src=\":/about/linkedin.png\"></a>"
                 "</p>"
                 "<p>(C) 2013 </p>")   );
}

void MainWindow::on_actionLine_Color_triggered()
{
    QColor newColor = QColorDialog::getColor(tabView->getLinePenColor());
    if (newColor.isValid())
        tabView->setLinePenColor(newColor);
}

void MainWindow::on_actionLine_Width_triggered()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        tabView->getLinePenWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        tabView->setLinePenWidth(newWidth);
}

void MainWindow::on_actionScale_Bar_Color_triggered()
{
    QColor newColor = QColorDialog::getColor(tabView->getScaleBarPenColor());
    if (newColor.isValid())
        tabView->setScaleBarPenColor(newColor);
//    tabView->removeScaleBar();
    tabView->drawScaleBar(micro);
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{

}

void MainWindow::on_actionAuto_triggered()
{
    ui->actionManual->setChecked(false);
    manualFlag = false;
    tabView->setManualFlag(false);
}

void MainWindow::on_actionManual_triggered()
{
    ui->actionAuto->setChecked(false);
    manualFlag = true;
    tabView->setManualFlag(true);
}

void MainWindow::on_actionSave_Tab_triggered()
{
    TabView *tab = currentTabView();
    if (tab == 0)
        return;

    for (;;) {
//        QString fileName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
        QString fileName = QFileDialog::getSaveFileName(this, "Save Tab", ui->tabWidget->tabText(ui->tabWidget->currentIndex()), "master measure tab (*.mmt)");

//        if (fileName.isEmpty())
//            fileName = QFileDialog::getSaveFileName(this);
        if (fileName.isEmpty())
            break;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this,
                                tr("File error"),
                                tr("Failed to open\n%1").arg(fileName));
//            tab->setFileName(QString());
        } else {
            QDataStream stream(&file);
            tab->save(stream);
//            tab->setFileName(fileName);

            int index = ui->tabWidget->indexOf(tab);
//            Q_ASSERT(index != -1);
            ui->tabWidget->setTabText(index, fileName);

            break;
        }
    }

}

TabView* MainWindow::currentTabView()const
{
    return (TabView*)ui->tabWidget->currentWidget();
}

void MainWindow::on_actionOpen_Saved_Tab_triggered()
{

//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
//    if (!fileName.isEmpty()) {

//        tabView = new TabView(fileName, micro);

//        ui->tabWidget->addTab(tabView,"someTab");

//        QFileInfo fileInfo = fileName;
//        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());

//        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

//        tabView->setManualFlag(manualFlag);

//    }
//    =========================================
    QString fileName = QFileDialog::getOpenFileName(this, "Open Saved Tab","","master measure tab (*.mmt)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                            tr("File error"),
                            tr("Failed to open\n%1").arg(fileName));
        return;
    }
    QDataStream stream(&file);

    tabView = new TabView(fileName, micro, stream);
    ui->tabWidget->addTab(tabView,"someTab");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

//    if (!tab->load(stream)) {
//        QMessageBox::warning(this,
//                            tr("Parse error"),
//                            tr("Failed to parse\n%1").arg(fileName));
//        delete tab;
//        return;
//    }
//    ============================================

}
