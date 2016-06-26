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

    //make the background transparent
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    activeArea = false;
    openImage = false;
}

//load image stored in path into the image variable
bool CustomView::loadImage(QString path)
{
    image = new QImage();
    image->load(path);
    if(image->isNull()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Not Supported Image");
        messageBox.setFixedSize(500,200);
        return false;
    }
    originalState = new QImage(*image);

    scene = new QGraphicsScene(this);
    setScene(scene);
    resetTransform();

    scene->addPixmap(QPixmap::fromImage(*image));
    emit enableRotateSignal();
    return true;
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    //validating that there is an image open
    if(!openImage)
        return;

    if(this->underMouse()){
        origin = event->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
    }
}

void CustomView::mouseMoveEvent(QMouseEvent *event)
{
    //validating that there is an image open
    if(!openImage)
        return;
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    activeArea = true;
    emit areaSelected();
}

void CustomView::mouseReleaseEvent(QMouseEvent *)
{
    //validating that there is an image open
    if(!openImage)
        return;
    origin = rubberBand->mapToParent(QPoint(0,0));
    endPoint = rubberBand->mapToParent(rubberBand->rect().bottomRight());
    activeArea = !(abs(origin.x()-endPoint.x()) <= 2 && abs(origin.y()-endPoint.y()) <=2);
    emit areaSelected();
}


void CustomView::zoomIn()
{
    rubberBand->hide();
    QRect rect = rubberBand->geometry().normalized();
    if (rect.width() > 5 && rect.height() > 5)
        fitInView(QRectF(mapToScene(rect.topLeft()), mapToScene(rect.bottomRight())), Qt::KeepAspectRatio);
    activeArea = false;
    emit areaSelected();
   // zoom(1.25);
}

void CustomView::zoomOut()
{
    zoom(0.75);
}

//a unified function for zooming in/out
void CustomView::zoom(qreal factor){
    rubberBand->hide();

    QRect rect = rubberBand->geometry().normalized();
    QRectF rec =QRectF(mapToScene(rect.topLeft()), mapToScene(rect.bottomRight()));

    centerOn(rec.center());
    scale(factor, factor);

     activeArea = false;
      emit areaSelected();
}


void CustomView::crop()
{
    rubberBand->hide();
    QImage copy ;
    QRect rect = rubberBand->geometry().normalized();
    copy = image->copy(QRectF(mapToScene(rect.topLeft()), mapToScene(rect.bottomRight())).toRect());
    //save current state in undo stack
    undoStack.push_front(*new QImage(*image));
    if(undoStack.size() > 5){
        undoStack.pop_back();
    }
    //show the croped image in the Scene
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(QPixmap::fromImage(copy));
    //save the croped image in the image variable
    *image = copy;

    activeArea = false;
    emit areaSelected();
}

//a function to rotate the image
void CustomView::rotate(int angle)
{
    undoStack.push_front(*new QImage(*image));
    if(undoStack.size() > 5){
        undoStack.pop_back();
    }
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

//TODO add scale code into action
void CustomView::scale(double scalex, double scaley)
{
    /*
    QImage img(name);
    QPixmap pixmap;
    pixmap = pixmap.fromImage(img.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    QFile file(folder+"/"+name);
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "jpeg",100);
    file.close();
    */
    QImage dstImg;
    try {
        undoStack.push_front(*new QImage(*image));
        if(undoStack.size() > 5){
            undoStack.pop_back();
        }
        QPoint center = image->rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.scale(scalex/100, scaley/100);

        dstImg = image->transformed(matrix  ,Qt::SmoothTransformation);
    } catch (...) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","very large image result");
        messageBox.setFixedSize(500,200);
        return ;
    }
    QPixmap dstPix = QPixmap::fromImage(dstImg);

    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(dstPix);

    *image = dstImg;
}

void CustomView::undo(){
    QPixmap dstPix = QPixmap::fromImage(*undoStack.begin());

    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(dstPix);

    redoStack.push_front(*new QImage(*image));
    if(redoStack.size() > 5){
        redoStack.pop_back();
    }
    *image = *undoStack.begin();
    undoStack.pop_front();

    rubberBand->hide();
    activeArea = false;
    emit areaSelected();
}


void CustomView::redo(){
    QPixmap dstPix = QPixmap::fromImage(*redoStack.begin());

    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(dstPix);

    undoStack.push_front(*new QImage(*image));
    if(undoStack.size() > 5){
        undoStack.pop_back();
    }
    *image = *redoStack.begin();
    redoStack.pop_front();

    rubberBand->hide();
    activeArea = false;
    emit areaSelected();
}


void CustomView::reset(){
    QPixmap dstPix = QPixmap::fromImage(*originalState);
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->addPixmap(dstPix);
    *image = QImage(*originalState);


    rubberBand->hide();
    activeArea = false;
    emit areaSelected();
}


bool CustomView::undoEmpty(){
    return undoStack.empty();
}

bool CustomView::redoEmpty(){
    return redoStack.empty();
}

void CustomView::clearUndo(){
    while(!undoStack.empty())
        undoStack.pop_back();
}

void CustomView::clearRedo(){
    while(!redoStack.empty())
        redoStack.pop_back();
}

QPixmap CustomView::getPix(){
    return QPixmap::fromImage(*image);
}
