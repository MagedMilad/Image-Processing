#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRubberBand>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_cropButton_clicked();

    void on_rotateButton_clicked();

    void on_reset_clicked();

    void on_zoomIn_clicked();

    void on_zoomOut_clicked();

    void on_undo_clicked();

    void on_redo_clicked();

    void on_actionSave_triggered();

    void on_horizontalSlider_sliderMoved(int position);

    void on_spinBox_valueChanged(int arg1);

    void on_actionExit_triggered();

    void on_rotateButton_3_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_spinBox_4_editingFinished();

    void on_spinBox_3_editingFinished();

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

public slots:
    void setEnable();
      void enableRotateSlot();
};

#endif // MAINWINDOW_H
