#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
//#include <QGraphicsView>
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

//    void on_actionExit_triggered();

//    void on_actionOpen_in_new_tab_triggered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;

protected:
    void closeEvent(QCloseEvent *event);



};

#endif // MAINWINDOW_H
