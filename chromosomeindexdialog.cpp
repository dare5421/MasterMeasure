

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

QChar ChromosomeIndexDialog::getGenome() const
{
    return genome;
}

void ChromosomeIndexDialog::setGenome(QChar value)
{
    genome = value;
}

void ChromosomeIndexDialog::on_pushButton_clicked()
{
    setPairIndex(ui->lineEdit->text().toInt());

    setGenome(ui->comboBox->currentText().at(0));

    this->close();
}

