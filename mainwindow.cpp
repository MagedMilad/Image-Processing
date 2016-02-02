#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :



    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->zoomButton->setEnabled(false);
    ui->cropButton->setEnabled(false);
    ui->rotateButton->setEnabled(false);
    connect(ui->graphicsView,SIGNAL(areaSelected()) , this , SLOT(setEnable()));
    connect(ui->graphicsView,SIGNAL(enableRotateSignal()) , this , SLOT(enableRotateSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("Images (*.png *.bmp *.jpg)" )
                );
    ui->graphicsView->loadImage(imagePath);

}

void MainWindow::setEnable()
{
    if(ui->graphicsView->activeArea){
        ui->zoomButton->setEnabled(true);
        ui->cropButton->setEnabled(true);
    }
    else{
        ui->zoomButton->setEnabled(false);
        ui->cropButton->setEnabled(false);
    }
}

void MainWindow::enableRotateSlot()
{
    ui->rotateButton->setEnabled(true);
}


void MainWindow::on_zoomButton_clicked()
{
    ui->graphicsView->zoom();
}

void MainWindow::on_cropButton_clicked()
{
    ui->graphicsView->crop();
}

void MainWindow::on_rotateButton_clicked()
{
    Dialog *d = new Dialog(this);
    connect(d,SIGNAL(rotateAccpetSignal(int)),ui->graphicsView,SLOT(rotateAccpetSlot(int)));
    d->exec();
}
