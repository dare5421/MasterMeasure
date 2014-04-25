#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
//    w.show();
    w.setWindowIcon(QIcon(":/image/ideokar.png"));
    return a.exec();
}
