#ifndef TABVIEW_H
#define TABVIEW_H

#include <QGraphicsView>

class TabView:public QGraphicsView
{
public:
    TabView(QString fileName);


private:
    QGraphicsScene *scene;

protected:
     void mousePressEvent(QMouseEvent *event);
};

#endif // MYVIEW_H
