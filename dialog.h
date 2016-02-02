#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
signals:
    void rotateAccpetSignal(int angle);
private slots:
    void on_horizontalSlider_sliderMoved(int position);
    void on_spinBox_valueChanged(int arg1);
    void on_buttonBox_accepted();
};

#endif // DIALOG_H
