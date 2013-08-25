#include "tabview.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>

#include <QMouseEvent>
#include <qmath.h>

#include <QGraphicsLineItem>
#include <QMenu>

//#include <QLineEdit>

void TabView::drawScaleBar(double micro)
{
    QPen pen;
    pen.setWidth(4);
    pen.setColor(scaleBarPenColor);

    QGraphicsLineItem *hLine;
    QGraphicsLineItem *vLine1;
    QGraphicsLineItem *vLine2;
    QGraphicsTextItem *text;
    text = scene->addText("10 micrometer");
    text->setPos(10,5);

    text->setDefaultTextColor(scaleBarPenColor);

    hLine = scene->addLine(10,25,micro*10,25,pen);
    vLine1 = scene->addLine(10,23,10,27,pen);
    vLine2 = scene->addLine(micro*10,23,micro*10,27,pen);

    QList<QGraphicsItem*> groupItems;
    groupItems.append(hLine);
    groupItems.append(vLine1);
    groupItems.append(vLine2);
    groupItems.append(text);
    // Finally  construct the group


    cliGroup = scene->createItemGroup(groupItems);

    cliGroup->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void TabView::removeScaleBar(){
    //    scene->destroyItemGroup(cliGroup);
    //    scene->removeItem();
}

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

    drawScaleBar(micro);

    this->setScene(scene);

    isEndPoint = false;
    isDraw = false;

    createActions();

    maxNumberOfChromosomes = 100; //danger
    numberOfChromosomes = 0;

    chromosomes = new chromosome[maxNumberOfChromosomes];

    flag_start = false;
    flag_head = false;
    flag_center = 0;
    flag_satellite = 0;
    flag_end = false;

    linePenColor = Qt::red;
    linePenWidth = 2;

    manualFlag = false;

    itemIndex = 0;


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

                QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(endPoint.x(),endPoint.y(),2,2);
                QGraphicsLineItem* line = new QGraphicsLineItem(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());

                QList<QGraphicsItem*> groupItems;

                ellipse->setPen(penDot);
                ellipse->setBrush(brush);
                line->setPen(penLine);

                groupItems.append(ellipse);
                groupItems.append(line);

                lineList.append(scene->createItemGroup(groupItems));
                itemIndex++;


                chromosomes[numberOfChromosomes-1].setChromosomeLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()+lineLength(startPoint, endPoint));

                if(flag_center == 1 ){
                    chromosomes[numberOfChromosomes-1].setChromosomeWing1Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
                    flag_center = 2;
                }
                if(flag_head){
                    chromosomes[numberOfChromosomes-1].setChromosomeHeadLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
                    flag_head = false;
                }
                if(flag_satellite == 1){
                    chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
                    flag_satellite = 2;
                }

//                pointStack.push(startPoint);
//                pointStack.push(endPoint);

                startPoint = endPoint;
                lastPoint = endPoint;

                startPointList<<startPoint;

                //                if(chromosomes[numberOfChromosomes-1].getTopLeft().x() > startPoint.x() ){
                //                    chromosomes[numberOfChromosomes-1].setTopLeft(QPoint(startPoint.x(),))
                //                }
                if(chromosomes[numberOfChromosomes-1].getTopLeftX() > startPoint.x()){
                    chromosomes[numberOfChromosomes-1].setTopLeftX(startPoint.x());
                }

                if(chromosomes[numberOfChromosomes-1].getTopLeftY() > startPoint.y()){
                    chromosomes[numberOfChromosomes-1].setTopLeftY(startPoint.y());
                }

                if(chromosomes[numberOfChromosomes-1].getBottomRightX() < startPoint.x()){
                    chromosomes[numberOfChromosomes-1].setBottomRightX(startPoint.x());
                }

                if(chromosomes[numberOfChromosomes-1].getBottomRightY() < startPoint.y()){
                    chromosomes[numberOfChromosomes-1].setBottomRightY(startPoint.y());
                }

            }else {
                startPoint = event->pos();
                startPoint = mapToScene(startPoint.x(),startPoint.y());

                lineList.clear();
                startPointList.clear();

                lineList.insert(0, scene->addEllipse(startPoint.x(),startPoint.y(),2,2,penDot,brush));
                startPointList.insert(0,startPoint);

                itemIndex++;

//                lineList[itemIndex++]=scene->addEllipse(startPoint.x(),startPoint.y(),2,2,penDot,brush);

                isEndPoint = true;
                lastPoint = startPoint;
                numberOfChromosomes++;
                flag_start = true;
                flag_head = false;
                flag_center = 0;
                flag_satellite = 0;
                flag_end = false;

                chromosomes[numberOfChromosomes-1].setTopLeftX(startPoint.x());
                chromosomes[numberOfChromosomes-1].setTopLeftY(startPoint.y());
                chromosomes[numberOfChromosomes-1].setBottomRightX(startPoint.x());
                chromosomes[numberOfChromosomes-1].setBottomRightY(startPoint.y());

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

    //    QLineEdit* textBox = new QLineEdit;

    int key = event->key();
    switch(key){

    case Qt::Key_Delete:
    {
        if(!lineList.isEmpty()){

            scene->removeItem(lineList.last());

            if(startPointList.count()>1)
                startPointList.removeLast();
            else // this is start point of chromosome
            {
                isDraw = false;
                isEndPoint = false;
            }

            startPoint = startPointList.last();

            lineList.removeLast();

        }

        break;
    }

    case Qt::Key_S:
    {
        isDraw = true;
        if(manualFlag){
            indexDialog = new ChromosomeIndexDialog(this);
            indexDialog->show();
        }

        break;
    }

    case Qt::Key_C:
    {
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penCenter,brushCenter);
        flag_center = 1;
        break;
    }

            //    case Qt::Key_H:
            //        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penHead,brushHead);
            //        flag_head = true;
            //        break;

    case Qt::Key_T:
    {
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penTail,brushTail);
        if (flag_center != 2){
            flag_head = true;
        }else
            flag_satellite = 1;
        break;
    }

    case Qt::Key_E:

    {

        if(manualFlag){
            QGraphicsTextItem *text;
            text = scene->addText(QString::number(indexDialog->getPairIndex()));
            text->setPos(lastPoint.x()-15,lastPoint.y()+15);
            text->setDefaultTextColor(Qt::white);
            text->setFlag(QGraphicsItem::ItemIsMovable, true);

            chromosomes[numberOfChromosomes-1].setIndex(indexDialog->getPairIndex());
        }

        isDraw = false;
        isEndPoint = false;
        flag_end = true;
        chromosomes[numberOfChromosomes-1].setChromosomeWing2Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeWing1Length());
        if(flag_satellite == 2){
            chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeTailLength());
            flag_satellite = false;
        }


        //        scene->addRect(QRectF(QPointF(chromosomes[numberOfChromosomes-1].
        //                              getTopLeftX(),chromosomes[numberOfChromosomes-1].getTopLeftY())
        //                              ,QPointF(chromosomes[numberOfChromosomes-1].
        //                              getBottomRightX(),chromosomes[numberOfChromosomes-1].getBottomRightY())));
        break;
    }


    case Qt::Key_P:
    {
        QMessageBox::information(this, tr("Master Measure"),
                                 QString::number(numberOfChromosomes)+"\nlenght:"+
                                 QString::number(chromosomes[numberOfChromosomes-1].getChromosomeLength())+"\nhead:"+
                QString::number(chromosomes[numberOfChromosomes-1].getChromosomeHeadLength())+"\nwing1:"+
                QString::number(chromosomes[numberOfChromosomes-1].getChromosomeWing1Length())+"\ntail:"+
                QString::number(chromosomes[numberOfChromosomes-1].getChromosomeTailLength())+"\nwing2:"+
                QString::number(chromosomes[numberOfChromosomes-1].getChromosomeWing2Length())
                );
        break;
    }


    case Qt::Key_Q:
    {
        QString str="";
        for (int i=0; i<numberOfChromosomes;i++){
            str += QString::number(i)+" "+
                    QString::number(chromosomes[i].getChromosomeLength())+"\n";
        }
        QMessageBox::information(this, tr("Master Measure"),str);
        break;
    }


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
    menu.addAction(satelliteAction);
    menu.addAction(endAction);

    menu.exec(event->globalPos());
}

void TabView::createActions()
{
    startAction = new QAction(tr("&Start drawing"), this);
    startAction->setIcon(QIcon(":/image/pencil.png"));
    //    newAct->setShortcuts(QKeySequence::New);
    //    newAct->setStatusTip(tr("Create a new drawing"));
    connect(startAction, SIGNAL(triggered()),this, SLOT(start()));

    endAction = new QAction(tr("&End drawing"), this);
    endAction->setIcon(QIcon(":/image/stop.png"));
    connect(endAction, SIGNAL(triggered()),this, SLOT(end()));

    centerAction = new QAction(tr("&Set Centromere"), this);
    centerAction->setIcon(QIcon(":/image/center.png"));
    connect(centerAction, SIGNAL(triggered()),this, SLOT(center()));

    satelliteAction = new QAction(tr("&Set Satellite"), this);
    satelliteAction->setIcon(QIcon(":/image/satellite.png"));
    connect(satelliteAction, SIGNAL(triggered()),this, SLOT(satellite()));
}

void TabView::satellite()
{
    scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,QPen(Qt::yellow),QBrush(Qt::cyan));
    if (flag_center != 2){
        flag_head = true;
    }else
        flag_satellite = 1;
}


void TabView::start()
{
    isDraw = true;
    if(manualFlag){
        indexDialog = new ChromosomeIndexDialog(this);
        indexDialog->show();
    }

    //    QMessageBox::information(this,"hi", "aha ");
}

void TabView::end()
{
    if(manualFlag){
        QGraphicsTextItem *text;
        text = scene->addText(QString::number(indexDialog->getPairIndex()));
        text->setPos(lastPoint.x()-15,lastPoint.y()+15);
        text->setDefaultTextColor(Qt::white);
        text->setFlag(QGraphicsItem::ItemIsMovable, true);

        chromosomes[numberOfChromosomes-1].setIndex(indexDialog->getPairIndex());
    }

    isDraw = false;
    isEndPoint = false;
    flag_end = true;
    chromosomes[numberOfChromosomes-1].setChromosomeWing2Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeWing1Length());
    if(flag_satellite == 2){
        chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeTailLength());
        flag_satellite = false;
    }
}

void TabView::center()
{
    QPen penCenter(Qt::blue);

    QBrush brushCenter(Qt::blue);

    scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penCenter,brushCenter);
    flag_center = 1;
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

chromosome* TabView::getSortedChromosomes(bool manual){

    chromosome *newChromosomes = new chromosome[maxNumberOfChromosomes];
    for (int i=0; i<numberOfChromosomes; i++){
        newChromosomes[i] = chromosomes[i];
    }

    if(manual){

        for(int i=0; i<numberOfChromosomes; i++){
            for(int j=0; j<i; j++){
                if(newChromosomes[i].getIndex() < newChromosomes[j].getIndex()){
                    newChromosomes[maxNumberOfChromosomes-1] = newChromosomes[i];
                    newChromosomes[i] = newChromosomes[j];
                    newChromosomes[j] = newChromosomes[maxNumberOfChromosomes-1];
                }

            }
        }

    }else{

        for(int i=0; i<numberOfChromosomes; i++){
            for(int j=0; j<i; j++){
                if(newChromosomes[i].getChromosomeLength() > newChromosomes[j].getChromosomeLength()){
                    newChromosomes[maxNumberOfChromosomes-1] = newChromosomes[i];
                    newChromosomes[i] = newChromosomes[j];
                    newChromosomes[j] = newChromosomes[maxNumberOfChromosomes-1];
                }

            }
        }

    }

    double temp = 0;
    for(int i=0; i<numberOfChromosomes; i++){
        if(newChromosomes[i].getChromosomeWing1Length() > newChromosomes[i].getChromosomeWing2Length()){
            temp = newChromosomes[i].getChromosomeWing2Length();
            newChromosomes[i].setChromosomeWing2Length(newChromosomes[i].getChromosomeWing1Length());
            newChromosomes[i].setChromosomeWing1Length(temp);

            temp = newChromosomes[i].getChromosomeHeadLength();
            newChromosomes[i].setChromosomeHeadLength(newChromosomes[i].getChromosomeTailLength());
            newChromosomes[i].setChromosomeTailLength(temp);

//            temp = newChromosomes[i].getChromosomeTailLength();
//            newChromosomes[i].setChromosomeTailLength(newChromosomes[i].getChromosomeHeadLength());
//            newChromosomes[i].setChromosomeHeadLength(temp);
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


QColor TabView::getScaleBarPenColor() const
{
    return scaleBarPenColor;
}

void TabView::setScaleBarPenColor(const QColor &value)
{
    scaleBarPenColor = value;
}


bool TabView::getManualFlag() const
{
    return manualFlag;
}

void TabView::setManualFlag(bool value)
{
    manualFlag = value;
}

