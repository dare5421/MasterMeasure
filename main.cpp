#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
//    w.show();
    w.setWindowIcon(QIcon(":/image/measure.png"));
    return a.exec();
}
