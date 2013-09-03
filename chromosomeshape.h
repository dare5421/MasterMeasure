#ifndef SHAPE_H
#define SHAPE_H
#include <QList>
#include <QGraphicsItem>
class ChromosomeShape
{
public:

    enum type{sPointType, lineType, centromereType, satelliteType};

    ChromosomeShape();

    void addItem(QGraphicsItem* item);
    void addType(type itemType);
    void addPoint(QPointF point);
    void addImage(QImage *img);

    void removeLastItem();
    void removeLastType();
    void removeLastPoint();

    QList<QGraphicsItem *> getItemList() const;
    QList<QPointF> getPointList() const;
    QList<type> getTypeList() const;

    void clearItemList();
    void clearTypeList();
    void clearPointList();

    void clearLists();

    QImage *getImage() const;

private:
    QList<QGraphicsItem *> itemList;
    QList<QPointF> pointList;
    QList<type> typeList;
    QImage * image;
};

#endif // SHAPE_H
