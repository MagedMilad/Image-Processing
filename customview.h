#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H


#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QRubberBand>

class CustomView : public QGraphicsView
{
    Q_OBJECT

public:
    CustomView(QWidget * parent = 0);
    void loadImage(QString path);
    bool activeArea;
private:
    QGraphicsScene *scene;
    QImage *image;
    QPoint origin , endPoint;
    QRubberBand *rubberBand;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void wheelEvent(QWheelEvent * event);
signals:
    void areaSelected();
    void enableRotateSignal();
public slots:
    void zoom();
    void crop();
    void rotateAccpetSlot(int angle);
};

#endif // CUSTOMVIEW_H







