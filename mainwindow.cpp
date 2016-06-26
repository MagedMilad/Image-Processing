#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>

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
    ui->actionSave->setEnabled(false);
    connect(ui->graphicsView,SIGNAL(areaSelected()) , this , SLOT(setEnable()));
    connect(ui->graphicsView,SIGNAL(enableRotateSignal()) , this , SLOT(enableRotateSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
// open the image
void MainWindow::on_actionOpen_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("Images (*.png *.bmp *.jpg)" )
                );
    if(imagePath.size() == 0){
        return ;
    }
   if(ui->graphicsView->loadImage(imagePath)){
       ui->graphicsView->openImage = true;
       ui->reset->setEnabled(true);
   }


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
    ui->actionSave->setEnabled(true);
}


// crop action
void MainWindow::on_cropButton_clicked()
{
    ui->graphicsView->crop();

    ui->undo->setEnabled(true);
    ui->redo->setEnabled(false);
    ui->graphicsView->clearRedo();
}

// rotate action
void MainWindow::on_rotateButton_clicked()
{

    ui->graphicsView->rotate(ui->spinBox->value());
    ui->undo->setEnabled(true);
    ui->redo->setEnabled(false);
    ui->graphicsView->clearRedo();

}

// reset action
void MainWindow::on_reset_clicked()
{
    ui->graphicsView->reset();
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
}

// zoom in action
void MainWindow::on_zoomIn_clicked()
{
    ui->graphicsView->zoomIn();
}

//zoom out action
void MainWindow::on_zoomOut_clicked()
{
    ui->graphicsView->zoomOut();
}

//undo action
void MainWindow::on_undo_clicked()
{
    ui->graphicsView->undo();
    if(ui->graphicsView->undoEmpty()){
        ui->undo->setEnabled(false);
    }
    ui->redo->setEnabled(true);
}

//redo action
void MainWindow::on_redo_clicked()
{
    ui->graphicsView->redo();
    if(ui->graphicsView->redoEmpty()){
        ui->redo->setEnabled(false);
    }
     ui->undo->setEnabled(true);
}

//save action
//void MainWindow::on_actionSave_triggered()
//{
//    QString imagePath = QFileDialog::getSaveFileName(
//                this,
//                tr("save File"),
//                "",
//                tr("Images (*.png *.bmp *.jpg)" )
//                );

//    if(imagePath.size() == 0){
//        return ;
//    }
//    QPixmap pixMap = ui->graphicsView->getPix();

//    pixMap.save(imagePath);
//}
void MainWindow::on_actionSave_triggered()
{
    if(!ui->graphicsView->openImage)
        return;

    QFileDialog dialog(this);
    dialog.setViewMode(QFileDialog::Detail);

    QString fileName = QFileDialog::getSaveFileName(this ,"Save file", QDir::currentPath(),"All files (*)" ,
                                                    new QString("Text files (*.txt)"));
    char const *format;
    if(fileName.size() >= 4)
    {
        QString last = fileName.right(4).toUpper();
        if(last == ".JPG")
            format = "JPG";

        else if(last == ".BMP")
            format = "BMP";

        else
        {
            fileName += ".png";
            format = "PNG";
        }
    }
    else{
        fileName += ".png";
        format = "PNG";
    }
        QPixmap pixMap = ui->graphicsView->getPix();

        pixMap.save(fileName,format);
}



void MainWindow::on_horizontalSlider_sliderMoved(int value)
{
    ui->spinBox->setValue(value);
}

void MainWindow::on_spinBox_valueChanged(int value)
{
 ui->horizontalSlider->setValue(value);
}

//close action
void MainWindow::on_actionExit_triggered()
{
    close();
    qApp->quit();
}

void MainWindow::on_rotateButton_3_clicked()
{
    ui->graphicsView->scale(ui->spinBox_3->value(), ui->spinBox_4->value());
    ui->undo->setEnabled(true);
    ui->redo->setEnabled(false);
    ui->graphicsView->clearRedo();
}
