#ifndef TABVIEW_H
#define TABVIEW_H

#include <QGraphicsView>

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

protected:
     void mousePressEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent * event);
};

#endif // MYVIEW_H
