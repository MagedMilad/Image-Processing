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
    ui->zoomOut->setEnabled(false);
    ui->zoomIn->setEnabled(false);
    ui->cropButton->setEnabled(false);
    ui->rotateButton->setEnabled(false);
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
    ui->reset->setEnabled(false);
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
    ui->reset->setEnabled(true);

}

void MainWindow::setEnable()
{
    if(ui->graphicsView->activeArea){
        ui->zoomIn->setEnabled(true);
        ui->cropButton->setEnabled(true);
        ui->zoomOut->setEnabled(true);
    }
    else{
        ui->zoomIn->setEnabled(false);
        ui->zoomOut->setEnabled(false);
        ui->cropButton->setEnabled(false);
    }
}

void MainWindow::enableRotateSlot()
{
    ui->rotateButton->setEnabled(true);
}



void MainWindow::on_cropButton_clicked()
{
    ui->graphicsView->crop();

    ui->undo->setEnabled(true);
    ui->redo->setEnabled(false);
    ui->graphicsView->clearRedo();
}

void MainWindow::on_rotateButton_clicked()
{
    Dialog *d = new Dialog(this);
    connect(d,SIGNAL(rotateAccpetSignal(int)),ui->graphicsView,SLOT(rotateAccpetSlot(int)));
    d->exec();
    ui->undo->setEnabled(true);
    ui->redo->setEnabled(false);
    ui->graphicsView->clearRedo();
}

void MainWindow::on_reset_clicked()
{
    ui->graphicsView->reset();
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
}

void MainWindow::on_zoomIn_clicked()
{
    ui->graphicsView->zoomIn();
}

void MainWindow::on_zoomOut_clicked()
{
    ui->graphicsView->zoomOut();
}

void MainWindow::on_undo_clicked()
{
    ui->graphicsView->undo();
    if(ui->graphicsView->undoEmpty()){
        ui->undo->setEnabled(false);
    }
    ui->redo->setEnabled(true);
}

void MainWindow::on_redo_clicked()
{
    ui->graphicsView->redo();
    if(ui->graphicsView->redoEmpty()){
        ui->redo->setEnabled(false);
    }
     ui->undo->setEnabled(true);
}

void MainWindow::on_actionSave_triggered()
{
    QString imagePath = QFileDialog::getSaveFileName(
                this,
                tr("save File"),
                "",
                tr("Images (*.png *.bmp *.jpg)" )
                );
    //QGraphicsView* view = new QGraphicsView(scene,this);
    //QString fileName = ".png";
    QPixmap pixMap = ui->graphicsView->getPix();
    //QPixmap pixMap = QPixmap::grabWidget(ui->graphicsView->image);
    pixMap.save(imagePath);
}
