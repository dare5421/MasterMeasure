#include "chromosomeindexdialog.h"
#include "ui_chromosomeindexdialog.h"

ChromosomeIndexDialog::ChromosomeIndexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChromosomeIndexDialog)
{
    ui->setupUi(this);
    pairIndex = -1;

}

ChromosomeIndexDialog::~ChromosomeIndexDialog()
{
    delete ui;
}

int ChromosomeIndexDialog::getPairIndex() const
{
    return pairIndex;
}

void ChromosomeIndexDialog::setPairIndex(int value)
{
    pairIndex = value;
}

void ChromosomeIndexDialog::on_pushButton_clicked()
{
    setPairIndex(ui->lineEdit->text().toInt());
    this->close();
}
