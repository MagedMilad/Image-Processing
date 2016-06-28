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
    ui->rotateButton_3->setEnabled(false);
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
    ui->reset->setEnabled(false);
    ui->actionSave->setEnabled(false);
    connect(ui->graphicsView,SIGNAL(areaSelected()) , this , SLOT(setEnable()));
    connect(ui->graphicsView,SIGNAL(enableRotateSignal()) , this , SLOT(enableRotateSlot()));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this, SLOT(on_cropButton_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), this, SLOT(on_zoomIn_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this, SLOT(on_zoomOut_clicked()));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(on_undo_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this, SLOT(on_redo_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, SLOT(on_reset_clicked()));


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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
   if(ui->graphicsView->loadImage(imagePath)){
       ui->graphicsView->openImage = true;
       ui->reset->setEnabled(true);
   }
   QApplication::restoreOverrideCursor();
   QApplication::processEvents();

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
    ui->rotateButton_3->setEnabled(true);
}


// crop action
void MainWindow::on_cropButton_clicked()
{
    if(ui->graphicsView->activeArea){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->graphicsView->crop();
        ui->undo->setEnabled(true);
        ui->redo->setEnabled(false);
        ui->graphicsView->clearRedo();
        QApplication::restoreOverrideCursor();
        QApplication::processEvents();
    }
}

// rotate action
void MainWindow::on_rotateButton_clicked()
{
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->graphicsView->rotate(ui->spinBox->value());
        ui->undo->setEnabled(true);
        ui->redo->setEnabled(false);
        ui->graphicsView->clearRedo();
        QApplication::restoreOverrideCursor();
        QApplication::processEvents();
}

// reset action
void MainWindow::on_reset_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    ui->graphicsView->reset();
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
}

// zoom in action
void MainWindow::on_zoomIn_clicked()
{
    if(ui->graphicsView->activeArea){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->graphicsView->zoomIn();
        QApplication::restoreOverrideCursor();
        QApplication::processEvents();
    }
}

//zoom out action
void MainWindow::on_zoomOut_clicked()
{
    if(ui->graphicsView->activeArea){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->graphicsView->zoomOut();
        QApplication::restoreOverrideCursor();
        QApplication::processEvents();
    }
}


//undo action
void MainWindow::on_undo_clicked()
{
    if(!ui->graphicsView->undoEmpty()){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->graphicsView->undo();
        if(ui->graphicsView->undoEmpty()){
            ui->undo->setEnabled(false);
        }
        ui->redo->setEnabled(true);
        QApplication::restoreOverrideCursor();
        QApplication::processEvents();
    }
}

//redo action
void MainWindow::on_redo_clicked()
{
    if(!ui->graphicsView->redoEmpty()){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->graphicsView->redo();
        if(ui->graphicsView->redoEmpty()){
            ui->redo->setEnabled(false);
        }
         ui->undo->setEnabled(true);
         QApplication::restoreOverrideCursor();
         QApplication::processEvents();
    }
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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
        QPixmap pixMap = ui->graphicsView->getPix();

        pixMap.save(fileName,format);
        QApplication::restoreOverrideCursor();
        QApplication::processEvents();
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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    ui->graphicsView->mScale(ui->spinBox_3->value(), ui->spinBox_4->value());
    ui->undo->setEnabled(true);
    ui->redo->setEnabled(false);
    ui->graphicsView->clearRedo();
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
}

void MainWindow::on_checkBox_stateChanged(int checked)
{
    if(checked){
        ui->spinBox_3->setValue(ui->spinBox_4->value());
    }
}

void MainWindow::on_spinBox_4_editingFinished()
{

}

void MainWindow::on_spinBox_3_editingFinished()
{

}

void MainWindow::on_spinBox_4_valueChanged(int)
{
    if(ui->checkBox->isChecked()){
        ui->spinBox_3->setValue(ui->spinBox_4->value());
    }
}

void MainWindow::on_spinBox_3_valueChanged(int)
{
    if(ui->checkBox->isChecked()){
        ui->spinBox_4->setValue(ui->spinBox_3->value());
    }
}
