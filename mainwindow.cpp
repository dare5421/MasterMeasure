#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
//    scene->addPixmap(QPixmap("d:/1.png"));



    ui->graphicsView->setScene(scene);
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
       QImage image(fileName);
       if (image.isNull()) {
           QMessageBox::information(this, tr("Image Viewer"),
                                    tr("Cannot load %1.").arg(fileName));
           return;
       }

       scene->addPixmap(QPixmap::fromImage(image));
   }
}
