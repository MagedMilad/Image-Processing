#include "customview.h"

#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QGraphicsScene>
#include <QRubberBand>
#include <QPoint>
#include <QPixmap>


qreal rotation;

CustomView::CustomView(QWidget * parent) : QGraphicsView( parent)
{
    rotation = 0.0;
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
        //cout << origin.x() << " "<<origin.y() <<endl ;
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
    activeArea = !(abs(origin.x()-endPoint.x()) <= 2 && abs(origin.y()-endPoint.y()) <=2);
    emit areaSelected();
}

//void CustomView::QWheelEvent(QWheelEvent *event)
//{
//    //rubberBand->hide();
//    //activeArea = false;
//    emit areaSelected();

//    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    setTransformationAnchor(origin);

//    double scaleFactor = 1.15;
//    scale(scaleFactor, scaleFactor);
////    if(event->delta() > 0) {
////        scale(scaleFactor, scaleFactor);
////    } else {
////        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
////    }
//}

void CustomView::zoomIn()
{
    rubberBand->hide();
    QRect rect = rubberBand->geometry().normalized();
    if (rect.width() > 5 && rect.height() > 5)
        fitInView(QRectF(mapToScene(rect.topLeft()), mapToScene(rect.bottomRight())), Qt::KeepAspectRatio);
    activeArea = false;
    emit areaSelected();
}

void CustomView::zoomOut()
{

    if(!activeArea){
        centerOn(QPointF(size().width()/2, size().height()/2));
    }
    else{
        rubberBand->hide();
        QRect rect = rubberBand->geometry().normalized();
        QRectF rec =QRectF(mapToScene(rect.topLeft()), mapToScene(rect.bottomRight()));
        centerOn(rec.center());
    }
    scale(0.75, 0.75);
    activeArea = false;
    emit areaSelected();

}

void CustomView::crop()
{
    rubberBand->hide();
    QImage copy ;
    //rotate(-1*rotation);
    copy = image->copy(QRect(origin - this->mapFromScene(0,0), endPoint - this->mapFromScene(0,0)));
    //    scene->clear();
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(QPixmap::fromImage(copy));
    //    scene->setSceneRect(0,0,copy.width(),copy.height());
    *image = copy;
    //rotate(rotation);
    activeArea = false;
    emit areaSelected();
}

void CustomView::rotateAccpetSlot(int angle)
{
    rotation = angle;
    //   rotate(angle);
    QPoint center = image->rect().center();
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(angle);
    QImage dstImg = image->transformed(matrix);
    QPixmap dstPix = QPixmap::fromImage(dstImg);

    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(dstPix);

    *image = dstImg;

}



