#include "customview.h"

#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QGraphicsScene>
#include <QRubberBand>
#include <QPoint>
#include <QPixmap>

CustomView::CustomView(QWidget * parent) : QGraphicsView( parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    rubberBand = new QRubberBand(QRubberBand::Rectangle , this);
    activeArea = false;
}

void CustomView::loadImage(QString path)
{
    image = new QImage();
    image->load(path);
    scene = new QGraphicsScene(this);
    setScene(scene);
    resetTransform();
    scene->addPixmap(QPixmap::fromImage(*image));
    emit enableRotateSignal();
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    if(this->underMouse()){
        origin = event->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
    }
}

void CustomView::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    activeArea = true;
    emit areaSelected();
}

void CustomView::mouseReleaseEvent(QMouseEvent *event)
{
    origin = rubberBand->mapToParent(QPoint(0,0));
    endPoint = rubberBand->mapToParent(rubberBand->rect().bottomRight());

}

//void CustomView::wheelEvent(QWheelEvent *event)
//{
//    rubberBand->hide();
//    activeArea = false;
//    emit areaSelected();

//    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    double scaleFactor = 1.15;
//    if(event->delta() > 0) {
//        scale(scaleFactor, scaleFactor);
//    } else {

//        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
//    }
//}

void CustomView::zoom()
{
    rubberBand->hide();
    this->fitInView(QRect(origin - this->mapFromScene(0,0), endPoint - this->mapFromScene(0,0)), Qt::KeepAspectRatio);
    activeArea = false;
    emit areaSelected();
}

void CustomView::crop()
{
    rubberBand->hide();
    QImage copy ;
    copy = image->copy(QRect(origin - this->mapFromScene(0,0), endPoint - this->mapFromScene(0,0)));
    //    scene->clear();
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(QPixmap::fromImage(copy));
    //    scene->setSceneRect(0,0,copy.width(),copy.height());
    *image = copy;
    activeArea = false;
    emit areaSelected();
}

void CustomView::rotateAccpetSlot(int angle)
{
   rotate(angle);
}



