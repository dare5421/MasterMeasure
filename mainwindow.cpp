#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QGraphicsLineItem>

#include "tabview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    scene = new QGraphicsScene;
//    scene->addPixmap(QPixmap("d:/1.png"));

    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

//    isMousePressed = false;

//    ui->graphicsView->setScene(scene);
//    ui->graphicsView->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
//        QImage image(fileName);
//        if (image.isNull()) {
//           QMessageBox::information(this, tr("Master Measure"),
//                                    tr("Cannot load %1.").arg(fileName));
//           return;
//        }

//        scene = new QGraphicsScene;
//        view = new QGraphicsView;
        TabView *view = new TabView(fileName);


//        view->setScene(scene);
        ui->tabWidget->addTab(view,"someTab");

//        scene->addPixmap(QPixmap::fromImage(image));

        QFileInfo fileInfo = fileName;
        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);



     }
}


//closeEvent overrided to show a properiate message before close
void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton msg;
    msg = QMessageBox::warning(this,"Master Measure","Are you sure you want to close?",QMessageBox::Yes | QMessageBox::No);
    if (msg == QMessageBox::Yes){
        event->accept();
    }
    else
        event->ignore();

}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}


//void MainWindow::mouseMoveEvent(QMouseEvent *event)
//{
////    if ((event->buttons() & Qt::LeftButton) && isMousePressed)
////        drawLineTo(event->pos());
//}

//void MainWindow::mouseReleaseEvent(QGraphicsSceneEvent *event)
//{
////    if (event->button() == Qt::LeftButton &&  isMousePressed) {

////        scene->addLine(startPoint.x(),startPoint.y(),100,250);
////        scribbling = false;
////        isMousePressed = false;
////        scene->addLine(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());

////    }
//}

//void MainWindow::paintEvent(QGraphicsSceneEvent *event)
//{

////    QGraphicsScene scene(this);
////    QRect dirtyRect = event->rect();

////    scene.drawImage(dirtyRect, image, dirtyRect);
//}

//void MainWindow::drawLineTo(const QPoint &endPoint)
//{
////    QGraphicsScene scene(&view);
////    scene->addLine(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());
////    QPainter painter(&image);
////    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
////                        Qt::RoundJoin));
////    painter.drawLine(lastPoint, endPoint);
////    modified = true;

////    int rad = (myPenWidth / 2) + 2;
////    update(QRect(lastPoint, endPoint).normalized()
////                                     .adjusted(-rad, -rad, +rad, +rad));
////    lastPoint = endPoint;
//}
