#ifndef TABVIEW_H
#define TABVIEW_H

#include <QGraphicsView>

#include "chromosome.h"
#include "chromosomeindexdialog.h"

//#include <QStack>
#include <QList>
#include <QDataStream>

#include "chromosomeshape.h"

class TabView:public QGraphicsView
{

    Q_OBJECT

public:

    enum type{sPointType, lineType, centromereType, satelliteType};

    int type2int(ChromosomeShape::type t);
    ChromosomeShape::type int2type(int i);

    TabView(QString fileName, double micro);
    TabView(QString fileName, double micro, QDataStream &stream);

    chromosome* getChromosomes();
    int getNumberOfChromosomes();
    chromosome* getSortedChromosomes(bool manual);



    QColor getLinePenColor() const;
    void setLinePenColor(const QColor &value);

    int getLinePenWidth() const;
    void setLinePenWidth(int value);



    QColor getScaleBarPenColor() const;
    void setScaleBarPenColor(const QColor &value);

    void drawScaleBar(double micro);

    void removeScaleBar();
    bool getManualFlag() const;
    void setManualFlag(bool value);

    void save(QDataStream &stream);
    bool load(QDataStream &stream);
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
    short flag_center;
    short flag_satellite;
    bool flag_end;

    double lineLength(QPointF startPoint, QPointF endPoint);

    //right click menu stuff
    void createActions();
    QAction *startAction;
    QAction *endAction;
    QAction *centerAction;
    QAction *satelliteAction;

    QColor linePenColor;
    int linePenWidth;

    QColor scaleBarPenColor;

    QGraphicsItemGroup * cliGroup;


    ChromosomeIndexDialog *indexDialog;

    bool manualFlag;

//    QStack<QPointF> pointStack;

//    QList<QGraphicsItem*> itemList;
//    QList<QPointF> startPointList;
//    QList<type> typeList;

    ChromosomeShape chromosomeShape;

    QList<ChromosomeShape> shapeList;

    int itemIndex;

private slots:
    void start();
    void end();
    void center();
    void satellite();

protected:
     void mousePressEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent * event);

     //right click menu
     void contextMenuEvent(QContextMenuEvent *event);

};

#endif // MYVIEW_H
