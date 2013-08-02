#include "scaledialog.h"
#include "ui_scaledialog.h"
#include <QCloseEvent>
//#include <QMessageBox>

ScaleDialog::ScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
//    scale = 100;
//    this->setWindowFlags(Qt::WindowTitleHint);
//    this->setWindowFlags(Qt::WindowCloseButtonHint);
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}

void ScaleDialog::on_pushButton_clicked()
{
    this->close();
    setScale((ui->comboBox->currentText()).toInt());
//    QMessageBox::information(this, tr("Master Measure"),QString::number(scale));

}

void ScaleDialog::on_browseButton_clicked()
{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

}

int ScaleDialog::getScale(){
    return scale;
}

void ScaleDialog::setScale(int scl){
    scale = scl;
}
