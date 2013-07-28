#include "tabview.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>


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

}

void TabView::mousePressEvent(QMouseEvent *event)
{
    QPen pen(Qt::black);
    QBrush brush(Qt::red);
    pen.setWidth(6);
    scene->addEllipse(-150,-150,1000,500,pen,brush);
}
