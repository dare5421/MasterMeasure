#ifndef TABVIEW_H
#define TABVIEW_H

#include <QGraphicsView>

#include "chromosome.h"

class TabView:public QGraphicsView
{
public:
    TabView(QString fileName);


private:
    QGraphicsScene *scene;
    QPointF startPoint;
    QPointF endPoint;
    QPointF lastPoint;

    bool isEndPoint;
    bool isDraw;

    int numberOfChromosomes;
    int maxNumberOfChromosomes;
    chromosome *chromosomes;
    chromosome myChro;
//    double chromosomeLength;
//    double chromosomeHeadLength;
//    double chromosomeTailLength;
//    double chromosomeWing1;
//    double chromosomeWing2;

    double lineLength(QPointF startPoint, QPointF endPoint);

protected:
     void mousePressEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent * event);
};

#endif // MYVIEW_H
