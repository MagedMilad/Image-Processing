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

    void on_zoomButton_clicked();

    void on_cropButton_clicked();

    void on_rotateButton_clicked();



private:
    Ui::MainWindow *ui;
public slots:
    void setEnable();
      void enableRotateSlot();
};

#endif // MAINWINDOW_H
