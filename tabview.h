#ifndef TABVIEW_H
#define TABVIEW_H

#include <QGraphicsView>

#include "chromosome.h"

class TabView:public QGraphicsView
{

    Q_OBJECT

public:
    TabView(QString fileName, double micro);

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

    //right click menu
    void createActions();
    QAction *startAction;
    QAction *endAction;
    QAction *centerAction;

private slots:
    void start();
    void end();
    void center();

protected:
     void mousePressEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent * event);

     void contextMenuEvent(QContextMenuEvent *event);

};

#endif // MYVIEW_H
