#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "tabview.h"
#include "chromosome.h"
#include "scaledialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    double pixToMicro(double pix);

//    void setDialogFlag(bool flag);

//    bool getDialogFlag();
    
private slots:
    void on_actionOpen_triggered();
    void on_tabWidget_tabCloseRequested(int index);

    void on_showButton_clicked();


    void on_calibrateButton_clicked();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

//    QGraphicsView *view;
    QGraphicsScene *scene;

    TabView *tabView;

    bool isMousePressed;
    QPoint startPoint,endPoint;

    chromosome **tabsChromosomes;
    int maxTab;

    ScaleDialog *scaleDialog;
    int scale;

    double micro;

    void drawChromosome(int x, int y, double wing1, double wing2,double errorWing1,double errorWing2);

    double* errorBarWing1;
    double* errorBarWing2;


protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
