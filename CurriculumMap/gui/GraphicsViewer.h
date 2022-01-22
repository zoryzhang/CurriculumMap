/*

GraphicsViewer is an inherited subclass of QGraphicsView. 

The course visualization interface in the ui interface is an instance of GraphicsViewer. 

The new internal functions and the original function override are designed to realize image movement and zooming.

three ways to zoom in/out
-Keyboard +/- keys
-Mouse wheel
-TrackPad pinch

three ways to move
-TrackPad swipe
-Keyboard arrow keys
-drag to move

*/

#ifndef GRAPHICSVIEWER_H
#define GRAPHICSVIEWER_H

#include <QGraphicsView>
#include <QWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGestureEvent>

class GraphicsViewer : public QGraphicsView
{

    Q_OBJECT

public:
    GraphicsViewer(QWidget* parent = nullptr);

protected:
    //use +/- to zoom in and zoom out 
    //arrow keys to control movement
    virtual void keyPressEvent(QKeyEvent* event) override;
    //realize zoom in and zoom out using wheelEvent and TrackPad
    virtual void wheelEvent(QWheelEvent* event) override;
    //grab event PinchGesture
    virtual bool event(QEvent *event) override;

    ////mousePressEvent
    void mousePressEvent(QMouseEvent *event) override;
    //mouseReleaseEvent
    void mouseReleaseEvent(QMouseEvent *event) override;
    //mouseMoveEvent
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // Magnification
    qreal scaleFactor=1;
    qreal currentStepScaleFactor=1;
    qreal pic_zoom=1;

    //grab event PinchGesture
    bool gestureEvent(QGestureEvent *event);
    //pinch Triggered 
    void pinchTriggered(QPinchGesture *gesture);
    void update_zoom();

    bool pressing=0;
    QPoint lst;//mouse
};

#endif // GRAPHICSVIEWER_H

