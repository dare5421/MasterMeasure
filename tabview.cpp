#include "tabview.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>

#include <QMouseEvent>

TabView::TabView(QString fileName)
{
    scene = new QGraphicsScene;

    QImage image(fileName);

    if (image.isNull()) {
       QMessageBox::information(this, tr("Master Measure"),
                                tr("Cannot load %1.").arg(fileName));
       return;
    }

    scene->addPixmap(QPixmap::fromImage(image));
    this->setScene(scene);

    isEndPoint = false;
    isDraw = true;

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
            startPoint = endPoint;
            lastPoint = endPoint;

        }else{
            startPoint = event->pos();
            startPoint = mapToScene(startPoint.x(),startPoint.y());
            scene->addEllipse(startPoint.x(),startPoint.y(),2,2,penDot,brush);
            isEndPoint = true;
            lastPoint = startPoint;
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
        break;
    case Qt::Key_H:
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penHead,brushHead);
        break;
    case Qt::Key_T:
        scene->addEllipse(lastPoint.x()-4,lastPoint.y()-4,8,8,penTail,brushTail);
        break;
    case Qt::Key_E:
        isDraw = false;
        break;
    case Qt::Key_S:
        isDraw = true;
        break;
    }

}
