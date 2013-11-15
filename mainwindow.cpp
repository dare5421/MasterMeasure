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

//    standardErrorWing1 = new double [max_size]();
//    standardErrorWing2 = new double [max_size]();

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
void MainWindow::createArray(double ** &array,  int max_size,  int genome_size)
{
    array = new double *[genome_size];
    for(int i=0; i<genome_size; i++)
        array[i] = new double[max_size]();

//    for(int i=0; i<genome_size; i++)
//        for(int j=0; j<max_size; j++)
//            array[i][j] = 0;

}

void MainWindow::deleteArray(double **&array, int genome_size)
{
    for(int i=0; i<genome_size; i++)
        delete []array[i];
    delete []array;
}

void MainWindow::on_showButton_clicked()
{

    int max_size = 100;
    int genome_size = 26;
    scale = scaleDialog->getScale();

    scene->clear();

    if((ui->tabWidget->currentIndex())>=0){
        for (int i=0; i< ui->tabWidget->count();i++){
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes(manualFlag);
        }


        int numberOfChromosomes =
                ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();


//        double avgWing1[genome_size][max_size]={0};
//        double avgWing2[genome_size][max_size]={0};

        double **avgWing1;
        createArray(avgWing1, max_size, genome_size);

        double **avgWing2;
        createArray(avgWing2, max_size, genome_size);

//        QMessageBox::information(this, tr("Master Measure"),QString::number(avgWing1[0][0]));

//        double standardErrorTotalLength[genome_size][max_size]={0};
//        double standardErrorWing1[genome_size][max_size]={0};
//        double standardErrorWing2[genome_size][max_size]={0};
//        double standardErrorLS[genome_size][max_size]={0};
//        double standardErrorSL[genome_size][max_size]={0};
        double **standardErrorTotalLength;
        createArray(standardErrorTotalLength, max_size, genome_size);

        double **standardErrorWing1;
        createArray(standardErrorWing1, max_size, genome_size);

        double **standardErrorWing2;
        createArray(standardErrorWing2, max_size, genome_size);

        double **standardErrorLS;
        createArray(standardErrorLS, max_size, genome_size);

        double **standardErrorSL;
        createArray(standardErrorSL, max_size, genome_size);

        double **standardErrorCI;
        createArray(standardErrorCI, max_size, genome_size);


//        double **standardErrorRL;
//        createArray(standardErrorRL, max_size, genome_size);


//        double satellite1[genome_size][max_size]={0};
//        double satellite2[genome_size][max_size]={0};
//        double avgTotalLength[genome_size][max_size]={0};
        double **satellite1;
        createArray(satellite1, max_size, genome_size);

        double **satellite2;
        createArray(satellite2, max_size, genome_size);

        double **avgTotalLength;
        createArray(avgTotalLength, max_size, genome_size);


//        double minTotalLength[genome_size][max_size]={0};
//        double maxTotalLength[genome_size][max_size]={0};
        double **minTotalLength;
        createArray(minTotalLength, max_size, genome_size);

        double **maxTotalLength;
        createArray(maxTotalLength, max_size, genome_size);


        QList<double> chromosomeLength[genome_size][max_size];
        QList<double> wing1[genome_size][max_size];
        QList<double> wing2[genome_size][max_size];
        QList<double> LS[genome_size][max_size];
        QList<double> SL[genome_size][max_size];
        QList<double> CI[genome_size][max_size];


//        double allTotalLength[genome_size][max_size]={0};    // sum of lengths of chromosomes
//        double allShortLength[genome_size][max_size]={0};
//        double sigmaWing1[genome_size][max_size]={0};
//        double sigmaWing2[genome_size][max_size]={0};
//        double sigmaTotalLength[genome_size][max_size]={0};

        double **allTotalLength;
        createArray(allTotalLength, max_size, genome_size);

        double **allShortLength;
        createArray(allShortLength, max_size, genome_size);

        double **sigmaWing1;
        createArray(sigmaWing1, max_size, genome_size);

        double **sigmaWing2;
        createArray(sigmaWing2, max_size, genome_size);

        double **sigmaTotalLength;
        createArray(sigmaTotalLength, max_size, genome_size);

        double HCL = 0;
        double TF = 0;
        double AsK = 0;
        double CL_min = 0;
        double CL_max = 0;
        double X_ci = 0;
        double A = 0;
        double A1 = 0;

//        double **RL;
//        createArray(RL, max_size, genome_size);




        QTableWidget *myTable = ui->tableWidget;
        myTable->setRowCount(0);

        myTable->horizontalHeader()->setVisible(true);

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
                    standardErrorWing1[0][j] = qSqrt(sigmaWing1[0][j]) / qSqrt((2*ui->tabWidget->count()));
                    standardErrorWing2[0][j] = qSqrt(sigmaWing2[0][j]) / qSqrt((2*ui->tabWidget->count()));

                    standardErrorTotalLength[0][j] = qSqrt(sigmaTotalLength[0][j]) / qSqrt((2*ui->tabWidget->count()));
                }
            }

            //draw in bottom
            for(int j=0; j< numberOfChromosomes/2; j++){
                if (j>0)
                    scene->addLine((j-1)*70+15,0,j*70+5,0);
                drawChromosome(j*70,0,0,
                               avgWing1[0][j] * 150 / maxChromosomeLength,avgWing2[0][j]* 150 / maxChromosomeLength,
                               standardErrorWing1[0][j], standardErrorWing2[0][j],
                               (satellite1[0][j] > satellite2[0][j])?satellite1[0][j]:satellite2[0][j] ,
                               (satellite1[0][j] > satellite2[0][j])?true:false  );

                //============ add chromosomes to table ================
                myTable->insertRow(myTable->rowCount());


                myTable->setItem(j, 0,new QTableWidgetItem("Chromosome "+QString::number(j+1)) );

                myTable->setItem(j, 1, new QTableWidgetItem(QString::number(avgWing1[0][j])+" " +177+" " + QString::number(standardErrorWing1[0][j])));

                myTable->setItem(j, 2, new QTableWidgetItem(QString::number(avgWing2[0][j])+" " +177+" " + QString::number(standardErrorWing2[0][j])));

                myTable->setItem(j, 3, new QTableWidgetItem(QString::number(avgWing2[0][j]+avgWing1[0][j])+" " +177+" " + QString::number(standardErrorTotalLength[0][j])));

                myTable->setItem(j, 4, new QTableWidgetItem(QString::number(avgWing2[0][j] / avgWing1[0][j])));

                myTable->setItem(j, 5, new QTableWidgetItem(QString::number(avgWing1[0][j] / avgWing2[0][j])));

                myTable->setItem(j, 6, new QTableWidgetItem(QString::number(allTotalLength[0][j])));

                myTable->setItem(j, 7, new QTableWidgetItem("% "+QString::number((avgWing2[0][j]+avgWing1[0][j])*100.0/allTotalLength[0][j])));

                myTable->setItem(j, 8, new QTableWidgetItem("% "+QString::number(allShortLength[0][j]*100.0/allTotalLength[0][j])));

                myTable->setItem(j, 9, new QTableWidgetItem("% "+QString::number(minTotalLength[0][j]*100.0/maxTotalLength[0][j])));
            }


        }
        else{ // =================== manual ==============================

            int countChromosome[500][26]={0};//count chromosomes with same index


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

                    chromosomeLength[index / 1000 - 1][index % 1000 - 1] << tabsChromosomes[i][j].getChromosomeLength();
                    wing1[index / 1000 - 1][index % 1000 - 1] << tabsChromosomes[i][j].getChromosomeWing1Length();
                    wing2[index / 1000 - 1][index % 1000 - 1] << tabsChromosomes[i][j].getChromosomeWing2Length();
                    SL[index / 1000 - 1][index % 1000 - 1] << tabsChromosomes[i][j].getChromosomeWing1Length()/tabsChromosomes[i][j].getChromosomeWing2Length();
                    LS[index / 1000 - 1][index % 1000 - 1] << tabsChromosomes[i][j].getChromosomeWing2Length()/tabsChromosomes[i][j].getChromosomeWing1Length();
                    CI[index / 1000 - 1][index % 1000 - 1] << tabsChromosomes[i][j].getChromosomeWing1Length()/tabsChromosomes[i][j].getChromosomeLength();

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
            int flag = 0,counter = 0, ARg2_counter = 0;

            for (int i=0; i<numberOfGenome;i++){
                for(int j=0; j< topIndexOfGenomes; j++){

                    if(avgWing1[i][j] + avgWing2[i][j] && countChromosome[i][j]){

                        allShortLength[i][j] = avgWing1[i][j]; // what about long wings ??????????!!!!!!!!!

                        allTotalLength[i][j] = avgTotalLength[i][j];

                        avgWing1[i][j] /= countChromosome[i][j];
                        avgWing2[i][j] /= countChromosome[i][j];

                        avgTotalLength[i][j]/= countChromosome[i][j];

                        satellite1[i][j]/= countChromosome[i][j];
                        satellite2[i][j]/= countChromosome[i][j];


                        standardErrorTotalLength[i][j] = standardError(countChromosome[i][j],chromosomeLength[i][j]);
                        standardErrorWing1[i][j] = standardError(countChromosome[i][j],wing1[i][j]);
                        standardErrorWing2[i][j] = standardError(countChromosome[i][j],wing2[i][j]);
                        standardErrorLS[i][j] = standardError(countChromosome[i][j],LS[i][j]);
                        standardErrorSL[i][j] = standardError(countChromosome[i][j],SL[i][j]);
                        standardErrorCI[i][j] = standardError(countChromosome[i][j],CI[i][j]);

                        HCL += avgWing1[i][j] + avgWing2[i][j];

                        TF += avgWing1[i][j];//it's not TF still

                        AsK += avgWing2[i][j];

                        CL_max = avgWing1[i][j]+avgWing2[i][j] > CL_max ? avgWing1[i][j]+avgWing2[i][j]: CL_max;

                        if (!flag){
                            CL_min = avgWing1[i][j]+avgWing2[i][j];
                            flag = 1;
                        }

                        CL_min = avgWing1[i][j]+avgWing2[i][j] < CL_min ? avgWing1[i][j]+avgWing2[i][j]: CL_min;

                        X_ci += avgWing1[i][j]/(avgWing1[i][j]+avgWing2[i][j]);

                        counter++;

                        A += (avgWing1[i][j]-avgWing2[i][j])/(avgWing1[i][j]-avgWing2[i][j]);

                        if(avgWing2[i][j]/avgWing1[i][j]>2)
                            ARg2_counter++;

                        A1 += avgWing1[i][j] /avgWing2[i][j];

                    }

                }
            }

            double CV_CL = 0, CV_CI = 0;
            double avgCL = HCL / counter;
            double avgCI = X_ci / counter;

            for (int i=0; i<numberOfGenome;i++){
                for(int j=0; j< topIndexOfGenomes; j++){
                    if(avgWing1[i][j] + avgWing2[i][j] && countChromosome[i][j]){
                        CV_CL += qPow( avgWing1[i][j] + avgWing2[i][j] - avgCL , 2);

                        CV_CI += qPow( avgWing1[i][j]/(avgWing1[i][j] + avgWing2[i][j]) - avgCI , 2);
                    }
                }
            }

            CV_CL /= (counter-1);
            CV_CL = qSqrt(CV_CL);
            CV_CL /= avgCL;
            CV_CL *= 100.0;

            CV_CI /= (counter-1);
            CV_CI = qSqrt(CV_CI);
            CV_CI /= avgCI;
            CV_CI *= 100.0;

            QString stebbin;
            if(CL_max/CL_min<2){
                if(ARg2_counter*100.0/counter == 100){stebbin = "4A";}
                else if(ARg2_counter*100.0/counter > 50){stebbin = "3A";}
                else if(ARg2_counter*100.0/counter > 1){stebbin = "2A";}
                else{stebbin = "1A";}
            }else if(CL_max/CL_min<4){
                if(ARg2_counter*100.0/counter == 100){stebbin = "4B";}
                else if(ARg2_counter*100.0/counter > 50){stebbin = "3B";}
                else if(ARg2_counter*100.0/counter > 1){stebbin = "2B";}
                else{stebbin = "1B";}
            }else{
                if(ARg2_counter*100.0/counter == 100){stebbin = "4C";}
                else if(ARg2_counter*100.0/counter > 50){stebbin = "3C";}
                else if(ARg2_counter*100.0/counter > 1){stebbin = "2C";}
                else{stebbin = "1C";}
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
//                    standardErrorWing1[0][j] = qSqrt(sigmaWing1[0][j]) / qSqrt(countChromosome[0][j]);
//                    standardErrorWing2[0][j] = qSqrt(sigmaWing2[0][j]) / qSqrt(countChromosome[0][j]);

//                    standardErrorTotalLength[0][j] = qSqrt(sigmaTotalLength[0][j]) / qSqrt(countChromosome[0][j]);
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
            for(int i=0; i<genome_size; i++)
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
                                   standardErrorWing1[0][j], standardErrorWing2[0][j],
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
    //                               standardErrorWing1[0][j], standardErrorWing2[0][j],
    //                               (satellite1[j] > satellite2[j])?satellite1[j]:satellite2[j],
    //                               (satellite1[j] > satellite2[j])?true:false  );


                    //============ add chromosomes to table ================

                    int tableRow = myTable->rowCount();
                    myTable->insertRow(tableRow);


//                    myTable->setItem(j, 0,new QTableWidgetItem("Chromosome "+QString::number(j+1)) );
                    QChar genC = 'A' + i;
                    myTable->setItem(tableRow, 0,new QTableWidgetItem(QString::number(j+1)+ genC ) );
                    // 177 is an ascii code for +/-
                    myTable->setItem(tableRow, 1, new QTableWidgetItem(QString::number(avgWing2[i][j])+" " +177+" " + QString::number(standardErrorWing2[i][j])));

                    myTable->setItem(tableRow, 2, new QTableWidgetItem(QString::number(avgWing1[i][j])+" " +177+" " + QString::number(standardErrorWing1[i][j])));

                    myTable->setItem(tableRow, 3, new QTableWidgetItem(QString::number(avgWing2[i][j]+avgWing1[i][j])+" " +177+" " + QString::number(standardErrorTotalLength[i][j])));

                    myTable->setItem(tableRow, 4, new QTableWidgetItem(QString::number(avgWing2[i][j] / avgWing1[i][j])+" " +177+" " + QString::number(standardErrorLS[i][j])));

                    myTable->setItem(tableRow, 5, new QTableWidgetItem(QString::number(avgWing1[i][j] / avgWing2[i][j])
                                                                +" " +177+" " + QString::number(standardErrorSL[i][j])));

                    myTable->setItem(tableRow, 6, new QTableWidgetItem("% "+QString::number((avgWing2[i][j]+avgWing1[i][j])*100.0/HCL)
                                                                +" " +177+" " + QString::number(standardErrorTotalLength[i][j]*100.0/HCL)));

                    myTable->setItem(tableRow, 7, new QTableWidgetItem("% "+QString::number(avgWing1[i][j]*100.0/HCL)
                                                                +" " +177+" " + QString::number(standardErrorWing1[i][j]*100.0/HCL)));

                    myTable->setItem(tableRow, 8, new QTableWidgetItem(QString::number(avgWing1[i][j]/(avgWing2[i][j]+avgWing1[i][j]))
                                                                +" " +177+" " + QString::number(standardErrorCI[i][j])));

                    double slp = avgWing1[i][j] *100.0 / avgWing2[i][j];
                    myTable->setItem(tableRow, 9, new QTableWidgetItem( slp > 0 ?(slp>12.5?(slp>25?(slp>39.5 ? (slp == 50? "M": "m") : "sm"):"st") : "t"):"T"));

                }
            }

            //add to the end of Table
            int tableRow = myTable->rowCount();
            myTable->insertRow(tableRow++);
            myTable->insertRow(tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "HCL" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(HCL) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "% TF" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(TF*100.0/HCL) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "% AsK" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(AsK*100.0/HCL) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "% S" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(CL_min*100.0/CL_max) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "X_CI" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(X_ci/counter) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "A" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(A/counter) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "X_CA" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(A*100.0/counter) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "CV_CL" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(CV_CL) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "CV_CI" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(CV_CI) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "AI" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(CV_CI*CV_CL/100.0) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "Stebbin's category of asymmetry" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( stebbin ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "A1" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(1 - A1 / counter) ) );

            myTable->insertRow(++tableRow);
            myTable->setItem(tableRow, 0,new QTableWidgetItem( "A2" ) );
            myTable->setItem(tableRow, 1,new QTableWidgetItem( QString::number(CV_CL/100.0) ) );





        }

        deleteArray(avgWing1, genome_size);
        deleteArray(avgWing2, genome_size);

        deleteArray(standardErrorTotalLength, genome_size);
        deleteArray(standardErrorWing1, genome_size);
        deleteArray(standardErrorWing2, genome_size);
        deleteArray(standardErrorLS, genome_size);
        deleteArray(standardErrorSL, genome_size);

        deleteArray(satellite1, genome_size);
        deleteArray(satellite2, genome_size);
        deleteArray(avgTotalLength, genome_size);

        deleteArray(minTotalLength, genome_size);
        deleteArray(maxTotalLength, genome_size);

        deleteArray(allTotalLength, genome_size);
        deleteArray(allShortLength, genome_size);
        deleteArray(sigmaWing1, genome_size);
        deleteArray(sigmaWing2, genome_size);
        deleteArray(sigmaTotalLength, genome_size);



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
