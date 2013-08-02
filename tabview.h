#ifndef TABVIEW_H
#define TABVIEW_H

#include <QGraphicsView>

#include "chromosome.h"

class TabView:public QGraphicsView
{
public:
    TabView(QString fileName);

    chromosome* getChromosomes();
    int getNumberOfChromosomes();
    chromosome* getSortedChromosomes();

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

    bool flag_start;
    bool flag_head;
    bool flag_center;
    short flag_tail;
    bool flag_end;

    double lineLength(QPointF startPoint, QPointF endPoint);

protected:
     void mousePressEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent * event);

};

#endif // MYVIEW_H
