#include "tabview.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>

#include <QMouseEvent>
#include <qmath.h>

#include <QGraphicsLineItem>
#include <QMenu>

#include <QScrollBar>


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
    chromosomeShape.addImage(image);

    drawScaleBar(micro);

    this->setScene(scene);

    this->setCursor(QCursor(Qt::CrossCursor));
    this->verticalScrollBar()->setCursor(QCursor(Qt::ArrowCursor));
    this->horizontalScrollBar()->setCursor(QCursor(Qt::ArrowCursor));

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

TabView::TabView(QString fileName, double micro, QDataStream &stream)
{

    // we need to save number of chromosomes draw before
    // we should save color and other thing and load it


    scene = new QGraphicsScene;


    if (!this->load(stream)) {
        QMessageBox::warning(this,
                            tr("Parse error"),
                            tr("Failed to parse\n%1").arg(fileName));
        return;
    }

    QImage image = shapeList[0].getImage();


    scene->setBackgroundBrush(QBrush(Qt::lightGray));
    scene->addPixmap(QPixmap::fromImage(image));
    this->setScene(scene);

    drawScaleBar(micro);

    this->setCursor(QCursor(Qt::CrossCursor));
    this->verticalScrollBar()->setCursor(QCursor(Qt::ArrowCursor));
    this->horizontalScrollBar()->setCursor(QCursor(Qt::ArrowCursor));


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
    manualFlag = false;


    linePenColor = Qt::red;
    linePenWidth = 2;

    QPen penDot(linePenColor.lighter(150));
    QPen penLine(linePenColor);
    QBrush brush(linePenColor.darker(200));
    penDot.setWidth(6);
    penLine.setWidth(linePenWidth);

    QPen penCenter(Qt::blue);
    QPen penHead(Qt::green);
    QPen penTail(Qt::cyan);

    QBrush brushCenter(Qt::blue);
    QBrush brushHead(Qt::green);
    QBrush brushTail(Qt::yellow);



    for (int i=0;i<shapeList.size();i++){
//        for(int j=0;j<shapeList[i].getPointList().size();j++){

//        }
        int tempCounter = 0;
        QList<ChromosomeShape::type> typeList = shapeList[i].getTypeList();
        QPointF sP;
        foreach (QPointF p, shapeList[i].getPointList()) {
            //switch case on point type and draw what you want
            switch(type2int(typeList[tempCounter++])){

            case 0:// sPointType:
            {
                scene->addEllipse(p.x(),p.y(),2,2,penDot,brush);
                sP = p;
                break;
            }
            case 1://lineType:
            {
                QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(p.x(),p.y(),2,2);
                QGraphicsLineItem* line = new QGraphicsLineItem(sP.x(),sP.y(),p.x(),p.y());

                QList<QGraphicsItem*> groupItems;

                ellipse->setPen(penDot);
                ellipse->setBrush(brush);
                line->setPen(penLine);

                groupItems.append(ellipse);
                groupItems.append(line);

                scene->createItemGroup(groupItems);

                sP = p;
                break;
            }
            case 2://centromereType:
            {
                scene->addEllipse(p.x()-4,p.y()-4,8,8,penCenter,brushCenter);
                break;
            }
            case 3://satelliteType:
            {
                scene->addEllipse(p.x()-4,p.y()-4,8,8,penTail,brushTail);
                break;
            }
            }

        }
//        foreach (ChromosomeShape::type t,shapeList[i].getTypeList()){


//        }
    }

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

//                itemList.append(scene->createItemGroup(groupItems));
//                typeList << lineType;
//                startPointList<<endPoint;
                chromosomeShape.addItem(scene->createItemGroup(groupItems));
                chromosomeShape.addType(ChromosomeShape::type(lineType));
                chromosomeShape.addPoint(endPoint);

                itemIndex++;


//                chromosomes[numberOfChromosomes-1].setChromosomeLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()+lineLength(startPoint, endPoint));

//                if(flag_center == 1 ){
//                    chromosomes[numberOfChromosomes-1].setChromosomeWing1Length(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
//                    flag_center = 2;
//                }
//                if(flag_head){
//                    chromosomes[numberOfChromosomes-1].setChromosomeHeadLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
//                    flag_head = false;
//                }
//                if(flag_satellite == 1){
//                    chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-lineLength(startPoint, endPoint));
//                    flag_satellite = 2;
//                }


                startPoint = endPoint;
                lastPoint = endPoint;


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

//                itemList.clear();
//                startPointList.clear();
//                typeList.clear();
                chromosomeShape.clearLists();
//                itemList << scene->addEllipse(startPoint.x(),startPoint.y(),2,2,penDot,brush);
//                startPointList << startPoint;
//                typeList << sPointType;

                chromosomeShape.addItem(scene->addEllipse(startPoint.x(),startPoint.y(),2,2,penDot,brush));
                chromosomeShape.addType(ChromosomeShape::type(sPointType));
                chromosomeShape.addPoint(startPoint);

                itemIndex++;

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

    int key = event->key();
    switch(key){

    case Qt::Key_Delete:
    {
        if(!chromosomeShape.getItemList().isEmpty()){

            scene->removeItem(chromosomeShape.getItemList().last());

            switch(chromosomeShape.getTypeList().last()){

                case sPointType:
                {
                    isDraw = false;
                    isEndPoint = false;

                    numberOfChromosomes--;
                    flag_head = false;
                    flag_center = 0;
                    flag_satellite = 0;


                    break;
                }
                case lineType:
                {
//                    startPointList.removeLast();
                    chromosomeShape.removeLastPoint();

                    break;
                }
                case centromereType:
                {
//                    startPointList.removeLast();
                    chromosomeShape.removeLastPoint();

                    break;
                }
                case satelliteType:
                {
//                    startPointList.removeLast();
                    chromosomeShape.removeLastPoint();

                    break;
                }
            }

            startPoint = chromosomeShape.getPointList().last();

//            itemList.removeLast();
//            typeList.removeLast();
            chromosomeShape.removeLastItem();
            chromosomeShape.removeLastType();

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
//        itemList << scene->addEllipse(startPoint.x()-4,startPoint.y()-4,8,8,penCenter,brushCenter);
//        startPointList << startPoint;
//        typeList << centromereType;
        chromosomeShape.addItem(scene->addEllipse(startPoint.x()-4,startPoint.y()-4,8,8,penCenter,brushCenter));
        chromosomeShape.addType(ChromosomeShape::type(centromereType) );
        chromosomeShape.addPoint(startPoint);

        flag_center = 1;

        break;
    }


    case Qt::Key_T:
    {
//        itemList << scene->addEllipse(startPoint.x()-4,startPoint.y()-4,8,8,penTail,brushTail);
//        startPointList << startPoint;
//        typeList << satelliteType;

        chromosomeShape.addItem(scene->addEllipse(startPoint.x()-4,startPoint.y()-4,8,8,penTail,brushTail));
        chromosomeShape.addType(ChromosomeShape::type(satelliteType) );
        chromosomeShape.addPoint(startPoint);

        if (flag_center != 2){
            flag_head = true;
        }else
            flag_satellite = 1;
        break;
    }

    case Qt::Key_E:

    {
        bool cFlag = false;
        bool satFlag = false;

        if (!chromosomeShape.getItemList().isEmpty())
            shapeList << chromosomeShape;

        while(!chromosomeShape.getTypeList().isEmpty()){
            switch(chromosomeShape.getTypeList().last()){

                case sPointType:
                {

                    break;
                }
                case lineType:
                {
                    chromosomes[numberOfChromosomes-1].setChromosomeLength
                        (chromosomes[numberOfChromosomes-1].getChromosomeLength()+
                        lineLength(chromosomeShape.getPointList()[chromosomeShape.getPointList().size()-2], chromosomeShape.getPointList().last()));
                    break;
                }
                case centromereType:
                {
                    chromosomes[numberOfChromosomes-1].setChromosomeWing2Length
                            (chromosomes[numberOfChromosomes-1].getChromosomeLength());
                    cFlag = satFlag ? false : true ;
                    break;
                }
                case satelliteType:
                {
                    chromosomes[numberOfChromosomes-1].setChromosomeTailLength
                                (chromosomes[numberOfChromosomes-1].getChromosomeLength());
                    satFlag = true;

                    break;
                }

            }

//            itemList.removeLast();
//            typeList.removeLast();
//            startPointList.removeLast();

            chromosomeShape.removeLastItem();
            chromosomeShape.removeLastPoint();
            chromosomeShape.removeLastType();
        }

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
        chromosomes[numberOfChromosomes-1].setChromosomeWing1Length
                (chromosomes[numberOfChromosomes-1].getChromosomeLength()-
                chromosomes[numberOfChromosomes-1].getChromosomeWing2Length());

//        if(flag_satellite == 2){
//            chromosomes[numberOfChromosomes-1].setChromosomeTailLength(chromosomes[numberOfChromosomes-1].getChromosomeLength()-chromosomes[numberOfChromosomes-1].getChromosomeTailLength());
//            flag_satellite = false;
//        }
        if(cFlag){
            if(chromosomes[numberOfChromosomes-1].getChromosomeTailLength()!=0)
                chromosomes[numberOfChromosomes-1].setChromosomeTailLength
                        (chromosomes[numberOfChromosomes-1].getChromosomeLength()-
                        chromosomes[numberOfChromosomes-1].getChromosomeTailLength());
        }


/*                scene->addRect(QRectF(QPointF(chromosomes[numberOfChromosomes-1].
                                      getTopLeftX(),chromosomes[numberOfChromosomes-1].getTopLeftY())
                                      ,QPointF(chromosomes[numberOfChromosomes-1].
                                      getBottomRightX(),chromosomes[numberOfChromosomes-1].getBottomRightY())));
*/
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

void TabView::save(QDataStream &stream)
{
    stream << shapeList.count();
    for(int i = 0 ; i< shapeList.count(); i++){
        const ChromosomeShape shape = shapeList[i];
        stream << shape.getItemList().count();
        for (int j=0; j< shape.getItemList().count();j++){
            if(i==0&&j==0){
                stream << shape.getImage();
            }
            const QPointF point = shape.getPointList()[j];
            const ChromosomeShape::type pointType=shape.getTypeList()[j];
            type2int(pointType);
//            const QGraphicsItem *item = shape.getItemList()[j];
            stream << point << pointType;
        }
    }
}

bool TabView::load(QDataStream &stream){

    shapeList.clear();
    QImage image ;
    int numberOfShape;
    int numberOfItem;


//    if(!stream.atEnd()){
//        stream >> image;
//    }

    stream >> numberOfShape;
    for(int i = 0 ; i< numberOfShape; i++){
        ChromosomeShape shape;
        stream >> numberOfItem;
        for (int j=0; j< numberOfItem;j++){
            if(i==0&&j==0){
                stream >> image;
                shape.addImage(image);
            }
            QPointF point;
            int pointTypeInt;
//            QGraphicsItem *item;
            stream >> point >> pointTypeInt;
//            shape.addItem(item);
            ChromosomeShape::type pointType = int2type(pointTypeInt);
            shape.addPoint(point);
            shape.addType(pointType);
        }
        shapeList << shape;
    }




    return true;
}


int TabView::type2int(ChromosomeShape::type t)
{
    return (int)t;
}

ChromosomeShape::type TabView::int2type(int i)
{
    return (ChromosomeShape::type)i;
}
