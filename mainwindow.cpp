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

    createActions();

    manualFlag = false;
    ui->actionAuto->setChecked(true);

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
//    menu.
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

// =============================== show bottom things ===========================
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
            tabsChromosomes[i] = ((TabView*)ui->tabWidget->widget(i))->getSortedChromosomes(manualFlag);
        }


        int numberOfChromosomes =
                ((TabView*)ui->tabWidget->widget(ui->tabWidget->currentIndex()))->getNumberOfChromosomes();

        double avgWing1[1000]={0};
        double avgWing2[1000]={0};
        double satellite1[1000]={0};
        double satellite2[1000]={0};
        double avgTotalLength[1000]={0};

        double minTotalLength[1000]={0};
        double maxTotalLength[1000]={0};

        double allTotalLength[1000]={0};
        double allShortLength[1000]={0};
        double sigmaWing1[1000]={0};
        double sigmaWing2[1000]={0};
        double sigmaTotalLength[1000]={0};
        double errorBarTotalLength[1000]={0};

        QTableWidget *myTable = ui->tableWidget;
        myTable->setRowCount(0);

        if(!manualFlag)
        {
            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){
                    avgWing1[j/2] += tabsChromosomes[i][j].getChromosomeWing1Length();
                    avgWing2[j/2] += tabsChromosomes[i][j].getChromosomeWing2Length();

                    avgTotalLength[j/2] += tabsChromosomes[i][j].getChromosomeLength();

                    if(j%2 == 0)
                        minTotalLength[j/2]= tabsChromosomes[i][j].getChromosomeLength();


                    if(tabsChromosomes[i][j].getChromosomeLength() < minTotalLength[j/2]){
                        minTotalLength[j/2]= tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeLength() > maxTotalLength[j/2]){
                        maxTotalLength[j/2]= tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeHeadLength()!= 0){
                        satellite1[j/2] += tabsChromosomes[i][j].getChromosomeHeadLength();
                    }
                    if(tabsChromosomes[i][j].getChromosomeTailLength()!= 0){
                        satellite2[j/2] += tabsChromosomes[i][j].getChromosomeTailLength();
                    }
                }
            }



            for(int j=0; j< numberOfChromosomes/2; j++){

                allShortLength[j] = avgWing1[j];

                avgWing1[j]/= (2*ui->tabWidget->count());
                avgWing2[j]/= (2*ui->tabWidget->count());

                allTotalLength[j] = avgTotalLength[j];

                avgTotalLength[j]/=(2*ui->tabWidget->count());

                satellite1[j]/= (2*ui->tabWidget->count());
                satellite2[j]/= (2*ui->tabWidget->count());
            }




            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){
                    sigmaWing1[j] += qPow((tabsChromosomes[i][j].getChromosomeWing1Length() - avgWing1[j]),2);
                    sigmaWing2[j] += qPow((tabsChromosomes[i][j].getChromosomeWing2Length() - avgWing2[j]),2);

                    sigmaTotalLength[j] += qPow((tabsChromosomes[i][j].getChromosomeLength() - avgTotalLength[j]),2);
                }
            }

            for(int j=0; j< numberOfChromosomes; j++){
                if(ui->tabWidget->count()<2){
                    sigmaWing1[j] = 0;
                    sigmaWing2[j] = 0;

                    sigmaTotalLength[j] = 0;
                }
                else{
                    sigmaWing1[j] /= ((2*ui->tabWidget->count())-1);
                    sigmaWing2[j] /= ((2*ui->tabWidget->count())-1);

                    sigmaTotalLength[j] /= ((2*ui->tabWidget->count())-1);
                }
            }


            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){
                    errorBarWing1[j] = qSqrt(sigmaWing1[j]) / qSqrt((2*ui->tabWidget->count()));
                    errorBarWing2[j] = qSqrt(sigmaWing2[j]) / qSqrt((2*ui->tabWidget->count()));

                    errorBarTotalLength[j] = qSqrt(sigmaTotalLength[j]) / qSqrt((2*ui->tabWidget->count()));
                }
            }

            //draw in bottom
            for(int j=0; j< numberOfChromosomes/2; j++){
                if (j>0)
                    scene->addLine((j-1)*70+15,0,j*70+5,0);
                drawChromosome(j*70,0, avgWing1[j],avgWing2[j], errorBarWing1[j], errorBarWing2[j], satellite1[j], satellite2[j]);

                //============ add chromosomes to table ================
                myTable->insertRow(myTable->rowCount());


                myTable->setItem(j, 0,new QTableWidgetItem("Chromosome "+QString::number(j+1)) );

                myTable->setItem(j, 1, new QTableWidgetItem(QString::number(avgWing1[j])+" " +177+" " + QString::number(errorBarWing1[j])));

                myTable->setItem(j, 2, new QTableWidgetItem(QString::number(avgWing2[j])+" " +177+" " + QString::number(errorBarWing2[j])));

                myTable->setItem(j, 3, new QTableWidgetItem(QString::number(avgWing2[j]+avgWing1[j])+" " +177+" " + QString::number(errorBarTotalLength[j])));

                myTable->setItem(j, 4, new QTableWidgetItem(QString::number(avgWing2[j] / avgWing1[j])));

                myTable->setItem(j, 5, new QTableWidgetItem(QString::number(avgWing1[j] / avgWing2[j])));

                myTable->setItem(j, 6, new QTableWidgetItem(QString::number(allTotalLength[j])));

                myTable->setItem(j, 7, new QTableWidgetItem("% "+QString::number((avgWing2[j]+avgWing1[j])*100.0/allTotalLength[j])));

                myTable->setItem(j, 8, new QTableWidgetItem("% "+QString::number(allShortLength[j]*100.0/allTotalLength[j])));

                myTable->setItem(j, 9, new QTableWidgetItem("% "+QString::number(minTotalLength[j]*100.0/maxTotalLength[j])));
            }


        }
        else{ // =================== manual ==============================

            int countChromosome[1000]={0};
            int numChro = 0;

            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numberOfChromosomes; j++){

                    avgWing1[tabsChromosomes[i][j].getIndex()-1] +=
                            tabsChromosomes[i][j].getChromosomeWing1Length();

                    avgWing2[tabsChromosomes[i][j].getIndex()-1] +=
                            tabsChromosomes[i][j].getChromosomeWing2Length();

                    avgTotalLength[tabsChromosomes[i][j].getIndex()-1] +=
                            tabsChromosomes[i][j].getChromosomeLength();

                    countChromosome[tabsChromosomes[i][j].getIndex()-1]++;
                    if(tabsChromosomes[i][j].getIndex()>numChro)
                        numChro = tabsChromosomes[i][j].getIndex();


                    if(j == 0 || (tabsChromosomes[i][j].getIndex() != tabsChromosomes[i][j-1].getIndex()))
                        minTotalLength[tabsChromosomes[i][j].getIndex()-1]= tabsChromosomes[i][j].getChromosomeLength();


                    if(tabsChromosomes[i][j].getChromosomeLength() < minTotalLength[tabsChromosomes[i][j].getIndex()-1]){
                        minTotalLength[tabsChromosomes[i][j].getIndex()-1]= tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeLength() > maxTotalLength[tabsChromosomes[i][j].getIndex()-1]){
                        maxTotalLength[tabsChromosomes[i][j].getIndex()-1]= tabsChromosomes[i][j].getChromosomeLength();
                    }

                    if(tabsChromosomes[i][j].getChromosomeHeadLength()!= 0){
                        satellite1[tabsChromosomes[i][j].getIndex()-1] += tabsChromosomes[i][j].getChromosomeHeadLength();
                    }
                    if(tabsChromosomes[i][j].getChromosomeTailLength()!= 0){
                        satellite2[tabsChromosomes[i][j].getIndex()-1] += tabsChromosomes[i][j].getChromosomeTailLength();
                    }

                }
            }

            for(int j=0; j< numChro; j++){

                allShortLength[j] = avgWing1[j];

                avgWing1[j]/= (countChromosome[j]);
                avgWing2[j]/= (countChromosome[j]);

                allTotalLength[j] = avgTotalLength[j];

                avgTotalLength[j]/=(countChromosome[j]);

                satellite1[j]/= countChromosome[j];
                satellite2[j]/= countChromosome[j];

            }

            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numChro; j++){
                    sigmaWing1[j] += qPow((tabsChromosomes[i][j].getChromosomeWing1Length() - avgWing1[j]),2);
                    sigmaWing2[j] += qPow((tabsChromosomes[i][j].getChromosomeWing2Length() - avgWing2[j]),2);

                    sigmaTotalLength[j] += qPow((tabsChromosomes[i][j].getChromosomeLength() - avgTotalLength[j]),2);
                }
            }

            for(int j=0; j< numChro; j++){

                sigmaWing1[j] /= (countChromosome[j]-1);
                sigmaWing2[j] /= (countChromosome[j]-1);

                sigmaTotalLength[j] /= (countChromosome[j]-1);

            }

            for (int i=0; i< ui->tabWidget->count();i++){
                for(int j=0; j< numChro; j++){
                    errorBarWing1[j] = qSqrt(sigmaWing1[j]) / qSqrt(countChromosome[j]);
                    errorBarWing2[j] = qSqrt(sigmaWing2[j]) / qSqrt(countChromosome[j]);

                    errorBarTotalLength[j] = qSqrt(sigmaTotalLength[j]) / qSqrt(countChromosome[j]);
                }
            }

            //draw in bottom
            for(int j=0; j< numChro; j++){
                if (j>0)
                    scene->addLine((j-1)*70+15,0,j*70+5,0);
                drawChromosome(j*70,0, avgWing1[j],avgWing2[j], errorBarWing1[j], errorBarWing2[j], satellite1[j], satellite2[j]);

                //============ add chromosomes to table ================
                myTable->insertRow(myTable->rowCount());


                myTable->setItem(j, 0,new QTableWidgetItem("Chromosome "+QString::number(j+1)) );

                myTable->setItem(j, 1, new QTableWidgetItem(QString::number(avgWing1[j])+" " +177+" " + QString::number(errorBarWing1[j])));

                myTable->setItem(j, 2, new QTableWidgetItem(QString::number(avgWing2[j])+" " +177+" " + QString::number(errorBarWing2[j])));

                myTable->setItem(j, 3, new QTableWidgetItem(QString::number(avgWing2[j]+avgWing1[j])+" " +177+" " + QString::number(errorBarTotalLength[j])));

                myTable->setItem(j, 4, new QTableWidgetItem(QString::number(avgWing2[j] / avgWing1[j])));

                myTable->setItem(j, 5, new QTableWidgetItem(QString::number(avgWing1[j] / avgWing2[j])));

                myTable->setItem(j, 6, new QTableWidgetItem(QString::number(allTotalLength[j])));

                myTable->setItem(j, 7, new QTableWidgetItem("% "+QString::number((avgWing2[j]+avgWing1[j])*100.0/allTotalLength[j])));

                myTable->setItem(j, 8, new QTableWidgetItem("% "+QString::number(allShortLength[j]*100.0/allTotalLength[j])));

                myTable->setItem(j, 9, new QTableWidgetItem("% "+QString::number(minTotalLength[j]*100.0/maxTotalLength[j])));
            }



        }



    }


}

void MainWindow::drawChromosome(int x, int y, double wing1, double wing2,double errorWing1,double errorWing2, double satellite1, double satellite2){

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
    scene->addRect(x, y-wing1-5+satellite1, 20 , wing1-satellite1);

    //add satellite wing1
    if(satellite1 >0){
        QPolygonF *polygon = new QPolygonF();
        polygon->append((QPoint(x+0,-wing1+satellite1-15)));
        polygon->append((QPoint(x+20,-wing1+satellite1-15)));
        polygon->append((QPoint(x+10,-wing1+satellite1-10)));
        polygon->append((QPoint(x+0,-wing1+satellite1-5)));
        polygon->append((QPoint(x+20,-wing1+satellite1-5)));

        scene->addPolygon(*polygon,QPen(Qt::black),QBrush(Qt::black));

        scene->addRect(x, y-wing1-15, 20 , satellite1);
    }

    textWing1->setPos(wing1Pos);
    scene->addItem(textWing1);

    // add error bar wing1
    if(satellite1>0){
        scene->addLine(x+8, y-wing1-15-errorWing1, x+12, y-wing1-15-errorWing1);
        scene->addLine(x+10, y-wing1-15-errorWing1, x+10, y-wing1-15+errorWing1);
        scene->addLine(x+8, y-wing1-15+errorWing1, x+12, y-wing1-15+errorWing1);
    }else{
        scene->addLine(x+8, y-wing1-5-errorWing1, x+12, y-wing1-5-errorWing1);
        scene->addLine(x+10, y-wing1-5-errorWing1, x+10, y-wing1-5+errorWing1);
        scene->addLine(x+8, y-wing1-5+errorWing1, x+12, y-wing1-5+errorWing1);
    }

    //add wing 2 is down
    scene->addRect(x, y+5, 20, wing2-satellite2);

    //add satellite wing1
    if(satellite2 >0){
        QPolygonF *polygon = new QPolygonF();
        polygon->append((QPoint(x+0,wing2-satellite2+5)));
        polygon->append((QPoint(x+20,wing2-satellite2+5)));
        polygon->append((QPoint(x+10,wing2-satellite2+10)));
        polygon->append((QPoint(x+0,wing2-satellite2+15)));
        polygon->append((QPoint(x+20,wing2-satellite2+15)));

        scene->addPolygon(*polygon);

        scene->addRect(x, y+wing2-satellite2+15, 20, satellite2);
    }


    textWing2->setPos(wing2Pos);
    scene->addItem(textWing2);

    // add error bar wing2
    if(satellite2>0){
        scene->addLine(x+8, y+wing2+15-errorWing2, x+12, y+wing2+15-errorWing2);
        scene->addLine(x+10, y+wing2+15-errorWing2, x+10, y+wing2+15+errorWing2);
        scene->addLine(x+8, y+wing2+15+errorWing2, x+12, y+wing2+15+errorWing2);
    }else{
        scene->addLine(x+8, y+wing2+5-errorWing2, x+12, y+wing2+5-errorWing2);
        scene->addLine(x+10, y+wing2+5-errorWing2, x+10, y+wing2+5+errorWing2);
        scene->addLine(x+8, y+wing2+5+errorWing2, x+12, y+wing2+5+errorWing2);
    }



    //    //add head
    //    scene->addRect(x, y-myChromosome.getChromosomeWing1Length()
    //                   , 20 , myChromosome.getChromosomeHeadLength());



    //    //add tail
    //    scene->addRect(x,y+myChromosome.getChromosomeWing2Length()-myChromosome.getChromosomeTailLength()
    //                   , 20, myChromosome.getChromosomeTailLength());

}

void MainWindow::createActions()
{
    saveAction = new QAction(tr("&Save Idiogram"), this);
    saveAction->setIcon(QIcon(":/image/floppy.png"));
    connect(saveAction, SIGNAL(triggered()),this, SLOT(on_actionSave_triggered()));

//    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
//    undoAction->setShortcuts(QKeySequence::Undo);

//    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
//    redoAction->setShortcuts(QKeySequence::Redo);
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
