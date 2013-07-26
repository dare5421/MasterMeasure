#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    scene = new QGraphicsScene;
//    scene->addPixmap(QPixmap("d:/1.png"));

    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);


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
        QImage image(fileName);
        if (image.isNull()) {
           QMessageBox::information(this, tr("Master Measure"),
                                    tr("Cannot load %1.").arg(fileName));
           return;
        }

        scene = new QGraphicsScene;
        view = new QGraphicsView;
        view->setScene(scene);
        ui->tabWidget->addTab(view,"someTab");

        scene->addPixmap(QPixmap::fromImage(image));

        QFileInfo fileInfo = fileName;
        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());


     }
}

//void MainWindow::on_actionOpen_in_new_tab_triggered()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
//    if (!fileName.isEmpty()) {
//        QImage image(fileName);
//        if (image.isNull()) {
//           QMessageBox::information(this, tr("Master Measure"),
//                                    tr("Cannot load %1.").arg(fileName));
//           return;
//        }

//        scene = new QGraphicsScene;
//        view = new QGraphicsView;
//        view->setScene(scene);
//        ui->tabWidget->addTab(view,"someTab");

//        scene->addPixmap(QPixmap::fromImage(image));
//        QFileInfo fileInfo = fileName;
//        ui->tabWidget->setTabText(ui->tabWidget->count()-1,fileInfo.baseName());


//        }
//}

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

//void MainWindow::on_actionExit_triggered()
//{
//}



void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
//    if(ui->tabWidget->count()==1){
//        ui->tabWidget->addTab(this,"empty");
//        ui->tabWidget->removeTab(index);
//    }
//    else
        ui->tabWidget->removeTab(index);
}
