#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "tabview.h"
#include "chromosome.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionOpen_triggered();
    void on_tabWidget_tabCloseRequested(int index);

    void on_showButton_clicked();

private:
    Ui::MainWindow *ui;
//    QGraphicsView *view;
//    QGraphicsScene *scene;
    TabView *tabView;

    bool isMousePressed;
    QPoint startPoint,endPoint;

    void drawLineTo(const QPoint &endPoint);


protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
