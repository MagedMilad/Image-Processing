#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H


#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QRubberBand>
#include <stack>

class CustomView : public QGraphicsView
{
    Q_OBJECT

public:
    CustomView(QWidget * parent = 0);
    bool loadImage(QString path);
    bool activeArea;
    bool openImage;

    int rotationAngle;


private:
    QGraphicsScene *scene;
    QImage *image;
    QImage *originalState;
    QImage *rotated;
    QPoint origin , endPoint;
    QRubberBand *rubberBand;
    std::deque<QImage> undoStack;
    std::deque<QImage> redoStack;
    void zoom(qreal factor);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
//    void wheelEvent(QWheelEvent * event);
signals:
    void areaSelected();
    void enableRotateSignal();
public slots:
    void zoomIn();
    void zoomOut();
    void crop();
    void rotate(int angle);
    void mScale(double scalex, double scaley);
    void undo();
    void redo();
    void reset();
    bool undoEmpty();
    bool redoEmpty();
    void clearUndo();
    void clearRedo();
    QPixmap getPix();

};

#endif // CUSTOMVIEW_H







