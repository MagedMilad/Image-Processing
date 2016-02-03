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
    void loadImage(QString path);
    bool activeArea;
private:
    QGraphicsScene *scene;
    QImage *image;
    QImage *originalState;
    QPoint origin , endPoint;
    QRubberBand *rubberBand;
    std::stack<QImage> undoStack;
    std::stack<QImage> redoStack;
    void zoom(qreal factor);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void wheelEvent(QWheelEvent * event);
signals:
    void areaSelected();
    void enableRotateSignal();
public slots:
    void zoomIn();
    void zoomOut();
    void crop();
    void rotateAccpetSlot(int angle);
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







