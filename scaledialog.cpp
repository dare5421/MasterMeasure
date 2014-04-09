#include "scaledialog.h"
#include "ui_scaledialog.h"
#include <QCloseEvent>

#include "tabview.h"
#include "mainwindow.h"


ScaleDialog::ScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
    scale = 10;
//    this->setWindowFlags(Qt::WindowTitleHint);
//    this->setWindowFlags(Qt::WindowCloseButtonHint);
    flag = false;

    scaleSet = false;
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}

void ScaleDialog::on_pushButton_clicked()
{
    this->close();
//    setScale((ui->comboBox->currentText()).toInt());
}

void ScaleDialog::on_browseButton_clicked()
{

//    ((MainWindow*)this->parent())->setDialogFlag(true);
    this->close();
    setScale(ui->lineEdit->text().toInt());

    scaleSet = true;

}

int ScaleDialog::getScale(){
    return scale;
}

void ScaleDialog::setScale(int scl){
    scale = scl;
}

bool ScaleDialog::isScaleSet()
{
    return scaleSet;
}
