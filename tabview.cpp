#include "tabview.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>

#include <QMouseEvent>
#include <qmath.h>

TabView::TabView(QString fileName)
{
    scene = new QGraphicsScene;

    QImage image(fileName);

    if (image.isNull()) {
       QMessageBox::information(this, tr("Master Measure"),
                                tr("Cannot load %1.").arg(fileName));
       return;
    }

    scene->setBackgroundBrush(QBrush(Qt::lightGray));
    scene->addPixmap(QPixmap::fromImage(image));
    this->setScene(scene);

    isEndPoint = false;
    isDraw = true;

    maxNumberOfChromosomes = 100; //danger
    numberOfChromosomes = 0;

    chromosomes = new chromosome[maxNumberOfChromosomes];

    flag_start = false;
    flag_head = false;
    flag_center = false;
    flag_tail = 0;
    flag_end = false;
}

void TabView::mousePressEvent(QMouseEvent *event)
{
    QPen penDot(Qt::black);
    QPen penLine(Qt::red);
    QBrush brush(Qt::red);
    penDot.setWidth(6);
    penLine.setWidth(2);
    if(isDraw){
        if(isEndPoint){
            endPoint = event->pos();
            endPoint = mapToScene(endPoint.x(),endPoint.y());
            scene->addEllipse(endPoint.x(),endPoint.y(),2,2,penDot,brush);
            scene->addLine(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y(),penLine);

            chromosomes[numberOfChromosomes-1].setChromosomeLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()+lineLength(startPoint, endPoint));

            if(flag_center){
                chromosomes[numberOfChromosomes-1].setChromosomeWing1Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
                flag_center = false;
            }
            if(flag_head){
                chromosomes[numberOfChromosomes-1].setChromosomeHeadLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
                flag_head = false;
            }
            if(flag_tail == 1){
                chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
                flag_tail = 2;
            }


            startPoint = endPoint;
            lastPoint = endPoint;

        }else {
            startPoint = event->pos();
            startPoint = mapToScene(startPoint.x(),startPoint.y());
            scene->addEllipse(startPoint.x(),startPoint.y(),2,2,penDot,brush);
            isEndPoint = true;
            lastPoint = startPoint;
            numberOfChromosomes++;
            flag_start = true;
            flag_head = false;
            flag_center = false;
            flag_tail = 0;
            flag_end = false;
        }
    }
}


void TabView::keyPressEvent(QKeyEvent * event){

    QPen penCenter(Qt::blue);
    QPen penHead(Qt::green);
    QPen penTail(Qt::cyan);

    QBrush brushCenter(Qt::blue);
    QBrush brushHead(Qt::green);
    QBrush brushTail(Qt::yellow);


    int key = event->key();
    switch(key){
    case Qt::Key_C:
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penCenter,brushCenter);
        flag_center = true;
        break;

    case Qt::Key_H:
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penHead,brushHead);
        flag_head = true;
        break;

    case Qt::Key_T:
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penTail,brushTail);
        flag_tail = 1;
        break;

    case Qt::Key_E:
        isDraw = false;
        isEndPoint = false;
        flag_end = true;
        chromosomes[numberOfChromosomes-1].setChromosomeWing2Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeWing1Length());
        if(flag_tail == 2){
            chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeTailLength());
            flag_tail = false;
        }
        break;

    case Qt::Key_S:
        isDraw = true;
        break;

    case Qt::Key_P:
        QMessageBox::information(this, tr("Master Measure"),
                                 QString::number(numberOfChromosomes)+"\nlenght:"+
                                 QString::number(chromosomes[numberOfChromosomes-1].getChromosomeLength())+"\nhead:"+
                                 QString::number(chromosomes[numberOfChromosomes-1].getChromosomeHeadLength())+"\nwing1:"+
                                 QString::number(chromosomes[numberOfChromosomes-1].getChromosomeWing1Length())+"\ntail:"+
                                 QString::number(chromosomes[numberOfChromosomes-1].getChromosomeTailLength())+"\nwing2:"+
                                 QString::number(chromosomes[numberOfChromosomes-1].getChromosomeWing2Length())
                                 );
        break;

    case Qt::Key_Q:
        QString str="";
        for (int i=0; i<numberOfChromosomes;i++){
            str += QString::number(i)+" "+
                   QString::number(chromosomes[i].getChromosomeLength())+"\n";
        }
        QMessageBox::information(this, tr("Master Measure"),str);
    }

}

double TabView::lineLength(QPointF startPoint, QPointF endPoint){

    return sqrt(pow(startPoint.x()-endPoint.x(), 2) + pow(startPoint.y() - endPoint.y(), 2));

}

chromosome* TabView::getChromosomes(){
    return chromosomes;
}
