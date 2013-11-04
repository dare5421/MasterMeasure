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
    
    double getMicro() const;
    void setMicro(double value);

    TabView *currentTabView() const;

private slots:
    void on_actionOpen_triggered();
    void on_tabWidget_tabCloseRequested(int index);

    void on_showButton_clicked();


    void on_calibrateButton_clicked();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();


    void on_actionLine_Color_triggered();

    void on_actionLine_Width_triggered();

    void on_actionScale_Bar_Color_triggered();

    void on_tableWidget_cellClicked(int row, int column);

    void on_actionAuto_triggered();

    void on_actionManual_triggered();

    void on_actionSave_Tab_triggered();

    void on_actionOpen_Saved_Tab_triggered();

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

    void drawChromosome(int x, int y, double wing1, double wing2,double errorWing1,double errorWing2,double satellite, bool isSatUp);

    double* errorBarWing1;
    double* errorBarWing2;

    void createActions();
    QAction *saveAction;

    bool manualFlag;


protected:
    void closeEvent(QCloseEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

};

#endif // MAINWINDOW_H
