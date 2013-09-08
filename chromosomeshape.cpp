#include "chromosomeshape.h"

ChromosomeShape::ChromosomeShape()
{
    itemList.clear();
    pointList.clear();
    typeList.clear();
}

void ChromosomeShape::addItem(QGraphicsItem *item)
{
    itemList << item;
}

void ChromosomeShape::addType(type itemType)
{
    typeList << itemType;
}

void ChromosomeShape::addPoint(QPointF point)
{
    pointList << point;
}

void ChromosomeShape::addImage(QImage img)
{
    image = img;
}

void ChromosomeShape::removeLastItem()
{
    itemList.removeLast();
}

void ChromosomeShape::removeLastType()
{
    typeList.removeLast();
}

void ChromosomeShape::removeLastPoint()
{
    pointList.removeLast();
}

QList<QGraphicsItem *> ChromosomeShape::getItemList() const
{
    return itemList;
}

QList<QPointF> ChromosomeShape::getPointList() const
{
    return pointList;
}

QList<ChromosomeShape::type> ChromosomeShape::getTypeList() const
{
    return typeList;
}

void ChromosomeShape::clearItemList()
{
    itemList.clear();
}

void ChromosomeShape::clearTypeList()
{
    typeList.clear();
}

void ChromosomeShape::clearPointList()
{
    pointList.clear();
}

void ChromosomeShape::clearLists()
{
    clearItemList();
    clearTypeList();
    clearPointList();
}

QImage ChromosomeShape::getImage() const
{
    return image;
}



