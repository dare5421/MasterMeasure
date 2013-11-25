#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>

namespace Ui {
class ScaleDialog;
}

class ScaleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ScaleDialog(QWidget *parent = 0);
    ~ScaleDialog();
    int getScale();
    void setScale(int scl);

    bool isScaleSet();

    bool flag;

    friend class MainWindow;
    
private:
    Ui::ScaleDialog *ui;
    int scale;

    bool scaleSet;


private slots:
    void on_pushButton_clicked();
    void on_browseButton_clicked();
};

#endif // SCALEDIALOG_H
