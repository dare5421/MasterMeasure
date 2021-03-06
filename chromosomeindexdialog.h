#ifndef CHROMOSOMEINDEXDIALOG_H
#define CHROMOSOMEINDEXDIALOG_H

#include <QDialog>

namespace Ui {
class ChromosomeIndexDialog;
}

class ChromosomeIndexDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChromosomeIndexDialog(QWidget *parent = 0);
    ~ChromosomeIndexDialog();
    
    int getPairIndex() const;
    void setPairIndex(int value);

    QChar getGenome() const;
    void setGenome(QChar value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ChromosomeIndexDialog *ui;

    int pairIndex;
    QChar genome;
};

#endif // CHROMOSOMEINDEXDIALOG_H
