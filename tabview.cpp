#include "tabview.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>

#include <QMouseEvent>
#include <qmath.h>

#include <QGraphicsLineItem>
#include <QMenu>

TabView::TabView(QString fileName, double micro)
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

    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::yellow);

    QGraphicsLineItem *hLine;
    QGraphicsLineItem *vLine1;
    QGraphicsLineItem *vLine2;
    QGraphicsTextItem *text;
    text = scene->addText("10 micrometer");
    text->setPos(10,5);
    text->setDefaultTextColor(Qt::yellow);
    //    text->setFont(QFont::);
    hLine = scene->addLine(10,25,micro*10,25,pen);
    vLine1 = scene->addLine(10,23,10,27,pen);
    vLine2 = scene->addLine(micro*10,23,micro*10,27,pen);

    QList<QGraphicsItem*> groupItems;
    groupItems.append(hLine); // add more items if you want
    groupItems.append(vLine1);
    groupItems.append(vLine2);
    groupItems.append(text);
    // Finally  construct the group
    QGraphicsItemGroup * cliGroup;
    cliGroup = scene->createItemGroup(groupItems);
    cliGroup->setFlag(QGraphicsItem::ItemIsMovable, true);

    //    scene->addLine(10,-10,micro*10,-10,pen)->setFlag(QGraphicsItem::ItemIsMovable, true);


    this->setScene(scene);

    isEndPoint = false;
    isDraw = false;

    createActions();

    maxNumberOfChromosomes = 100; //danger
    numberOfChromosomes = 0;

    chromosomes = new chromosome[maxNumberOfChromosomes];

    flag_start = false;
    flag_head = false;
    flag_center = false;
    flag_tail = 0;
    flag_end = false;

    linePenColor = Qt::red;
    linePenWidth = 2;
}

void TabView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPen penDot(linePenColor.lighter(150));
        QPen penLine(linePenColor);
        QBrush brush(linePenColor.darker(200));
        penDot.setWidth(6);
        penLine.setWidth(linePenWidth);
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

        //    case Qt::Key_H:
        //        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penHead,brushHead);
        //        flag_head = true;
        //        break;

        //    case Qt::Key_T:
        //        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penTail,brushTail);
        //        flag_tail = 1;
        //        break;

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

void TabView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    //    menu.addAction(cutAct);
    //    menu.addAction(copyAct);
    //    menu.addAction(pasteAct);
    //    menu.addAction("this is it");
    menu.addAction(startAction);
    menu.addAction(centerAction);
    menu.addAction(endAction);

    menu.exec(event->globalPos());
}

void TabView::createActions()
{
    startAction = new QAction(tr("&Start drawing"), this);
    //    newAct->setShortcuts(QKeySequence::New);
    //    newAct->setStatusTip(tr("Create a new drawing"));
    connect(startAction, SIGNAL(triggered()),this, SLOT(start()));

    endAction = new QAction(tr("&End drawing"), this);
    connect(endAction, SIGNAL(triggered()),this, SLOT(end()));

    centerAction = new QAction(tr("&Set Centromere"), this);
    connect(centerAction, SIGNAL(triggered()),this, SLOT(center()));
}

void TabView::start()
{
    isDraw = true;
    //    QMessageBox::information(this,"hi", "aha ");
}

void TabView::end()
{
    isDraw = false;
    isEndPoint = false;
    flag_end = true;
    chromosomes[numberOfChromosomes-1].setChromosomeWing2Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeWing1Length());
    if(flag_tail == 2){
        chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeTailLength());
        flag_tail = false;
    }
}

void TabView::center()
{
    QPen penCenter(Qt::blue);

    QBrush brushCenter(Qt::blue);

    scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penCenter,brushCenter);
    flag_center = true;
}

double TabView::lineLength(QPointF startPoint, QPointF endPoint){

    return sqrt(pow(startPoint.x()-endPoint.x(), 2) + pow(startPoint.y() - endPoint.y(), 2));

}


chromosome* TabView::getChromosomes(){
    double temp =0;
    for(int i=0; i<numberOfChromosomes; i++){
        if(chromosomes[i].getChromosomeWing1Length() > chromosomes[i].getChromosomeWing2Length()){
            temp = chromosomes[i].getChromosomeWing2Length();
            chromosomes[i].setChromosomeWing2Length(chromosomes[i].getChromosomeWing1Length());
            chromosomes[i].setChromosomeWing1Length(temp);
        }
    }
    return chromosomes;
}

int TabView::getNumberOfChromosomes(){
    return numberOfChromosomes;
}

chromosome* TabView::getSortedChromosomes(){

    chromosome *newChromosomes = new chromosome[maxNumberOfChromosomes];
    for (int i=0; i<numberOfChromosomes; i++){
        newChromosomes[i] = chromosomes[i];
    }
    for(int i=0; i<numberOfChromosomes; i++){
        for(int j=0; j<i; j++){
            if(newChromosomes[i].getChromosomeLength()>newChromosomes[j].getChromosomeLength()){
                newChromosomes[maxNumberOfChromosomes-1] = newChromosomes[i];
                newChromosomes[i] = newChromosomes[j];
                newChromosomes[j] = newChromosomes[maxNumberOfChromosomes-1];
            }
        }
    }

    double temp =0;
    for(int i=0; i<numberOfChromosomes; i++){
        if(newChromosomes[i].getChromosomeWing1Length() > newChromosomes[i].getChromosomeWing2Length()){
            temp = newChromosomes[i].getChromosomeWing2Length();
            newChromosomes[i].setChromosomeWing2Length(newChromosomes[i].getChromosomeWing1Length());
            newChromosomes[i].setChromosomeWing1Length(temp);
        }
    }

    return newChromosomes;
}

QColor TabView::getLinePenColor() const
{
    return linePenColor;
}

void TabView::setLinePenColor(const QColor &value)
{
    linePenColor = value;
}

int TabView::getLinePenWidth() const
{
    return linePenWidth;
}

void TabView::setLinePenWidth(int value)
{
    linePenWidth = value;
}


