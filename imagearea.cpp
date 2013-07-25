#include "imagearea.h"

ImageArea::ImageArea(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}
