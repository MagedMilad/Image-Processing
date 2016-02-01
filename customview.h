#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H


#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QRubberBand>

class CustomView : public QGraphicsView
{

public:
    CustomView(QWidget * parent = 0);
    void loadImage(QString path);
private:
    QGraphicsScene *scene;
    QImage *image;
    QPoint myPoint;
    QRubberBand *rubberBand;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // CUSTOMVIEW_H
