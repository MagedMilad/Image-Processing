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

}

void CustomView::loadImage(QString path)
{
    image = new QImage();
    image->load(path);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*image));
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    //      qDebug("1111111111111111");
    if(this->underMouse()){
        myPoint = event->pos();
        rubberBand->setGeometry(QRect(myPoint, QSize()));
        rubberBand->show();
    }
}

void CustomView::mouseMoveEvent(QMouseEvent *event)
{
    //      qDebug("222222222222222");
    rubberBand->setGeometry(QRect(myPoint, event->pos()).normalized());
}

void CustomView::mouseReleaseEvent(QMouseEvent *event)
{
    //    if (event->button() == Qt::LeftButton) {
    //        qDebug("33333333333333");
    //    QRect myRect(myPoint, event->pos());

    //    rubberBand->hide();

    //    QPixmap OriginalPix(*ui->imageLabel->pixmap());

    //    QImage newImage;
    //    newImage = OriginalPix.toImage();

    //    QImage copyImage;
    //    copyImage = copyImage.copy(myRect);

    //    ui->imageLabel->setPixmap(QPixmap::fromImage(copyImage));
    //    ui->imageLabel->repaint();
    ///////////////////////////////////////////////////
    QPoint a = mapToGlobal(myPoint);
    QPoint b = event->globalPos();

    a = this->mapFromGlobal(a);
    b = this->mapFromGlobal(b);

    QPixmap OriginalPix = QPixmap::fromImage(*image);


    //        double sx = this->rect().width();
    //        double sy = this->rect().height();

    //        sx = OriginalPix.width() / sx;
    //        sy = OriginalPix.height() / sy;

    //    a.x() = (a.x() * sx);
    //    b.x() = (b.x() * sx);
    //    a.y() = (a.y() * sy);
    //    b.y() = (b.y() * sy);

    a.setX((a.x() + this->rect().x() + OriginalPix.rect().x()));
    b.setX((b.x()+ this->rect().x() + OriginalPix.rect().x()));
    a.setY((a.y()+ this->rect().y() + OriginalPix.rect().y()));
    b.setY((b.y() +this->rect().y() + OriginalPix.rect().y()));

    rubberBand->hide();

    QRect myRect(a, b);

    QImage copy ;
    copy = image->copy(myRect);

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(copy));

    *image = copy;
    /////////////////////////////////////////////////
    //    rubberBand->hide();
    //    const QRect & zoomRect = rubberBand->geometry();
    ////    int xp1, yp1, xp2, yp2;
    ////    zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
    ////    int x1 = xAxis->pixelToCoord(xp1);
    ////    int x2 = xAxis->pixelToCoord(xp2);
    ////    int y1 = yAxis->pixelToCoord(yp1);
    ////    int y2 = yAxis->pixelToCoord(yp2);

    ////        QRect myRect(new QPoint(x1,y1), new QPoint(x2,y2));

    //        QImage copy ;
    //        copy = image->copy(zoomRect);

    //        scene->clear();
    //        scene->addPixmap(QPixmap::fromImage(copy));

    //        *image = copy;

}

