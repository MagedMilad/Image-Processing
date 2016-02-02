#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_horizontalSlider_sliderMoved(int position)
{
    ui->spinBox->setValue(position);

}

void Dialog::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);

}

void Dialog::on_buttonBox_accepted()
{
    emit rotateAccpetSignal(ui->spinBox->value());
}
