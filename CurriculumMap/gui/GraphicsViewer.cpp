/*

This part includes the implementation of GraphicsViewer internal functions

*/

#include "GraphicsViewer.h"
#include "global.h"
#include <iostream>
#include <cmath>

#include <QMessageBox>
#include <QInputDevice>
using namespace std;

//scale center at the cente of mouse
GraphicsViewer::GraphicsViewer(QWidget* parent) : QGraphicsView(parent)
{
    grabGesture(Qt::PinchGesture);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

//use +/- to zoom in and zoom out 
//Arrow keys to control movement
void GraphicsViewer::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key::Key_Plus:
            scaleFactor*=1.1;update_zoom();
            break;
        case Qt::Key::Key_Minus:
            scaleFactor/=1.1;update_zoom();
            break;
    }
    QGraphicsView::keyPressEvent(event);
}

//realize zoom in and zoom out using wheelEvent and TrackPad
void GraphicsViewer::wheelEvent(QWheelEvent* event)
{
    if(event->deviceType()==QInputDevice::DeviceType::Mouse)
    {
        scaleFactor*=exp(event->angleDelta().y() / 720.0);
        update_zoom();
        // QGraphicsView::wheelEvent(event); so it will not scroll
    }
    else//TrackPad
    {
        QPoint pixelDelta=event->pixelDelta(),angleDelta=event->angleDelta();
        if( event->isInverted() ) pixelDelta.setX(-pixelDelta.x()),angleDelta.setX(-angleDelta.x());//natural
        else pixelDelta.setY(-pixelDelta.y()),angleDelta.setY(-angleDelta.y());
        QWheelEvent* p=new QWheelEvent(
                        event->position(),event->globalPosition(),
                        pixelDelta,angleDelta,
                        event->buttons(),event->modifiers(),event->phase(),0,event->source()
                    );
        QGraphicsView::wheelEvent(p);delete p;
    }
}

//mousePressEvent
void GraphicsViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::MouseButton::LeftButton) pressing=1,lst=event->pos();
    QGraphicsView::mousePressEvent(event);
}

//mouseReleaseEvent
void GraphicsViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::MouseButton::LeftButton) pressing=0;
    QGraphicsView::mouseReleaseEvent(event);
}

//mouseMoveEvent
void GraphicsViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(pressing)
    {
        QPointF delta=mapToScene(event->pos())-mapToScene(lst);delta*=pic_zoom;
        QPoint t=viewport()->rect().center() - delta.toPoint() ;
        QPolygonF tt=mapToScene(QRect(t, QSize(2, 2)));//rect->super high correctness
        QGraphicsItem *p=scene()->addPolygon(tt);centerOn(p);scene()->removeItem(p);

        lst=event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

//pinch Triggered 
void GraphicsViewer::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged)
        currentStepScaleFactor = gesture->totalScaleFactor(),update_zoom();
    if (gesture->state() == Qt::GestureFinished)
    {
        scaleFactor *= currentStepScaleFactor;
        currentStepScaleFactor = 1;
        update_zoom();
    }
}

//grab event PinchGesture
bool GraphicsViewer::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        if( QGesture *pinch = static_cast<QGestureEvent*>(event)->gesture(Qt::PinchGesture) )
            {pinchTriggered(static_cast<QPinchGesture *>(pinch));return true;}
    return QGraphicsView::event(event);
}

//update_zoom
void GraphicsViewer::update_zoom()
{
    qreal enroll_scale=scaleFactor*currentStepScaleFactor;
    if(enroll_scale != pic_zoom)
    {
        scale(enroll_scale/pic_zoom,enroll_scale/pic_zoom);
        pic_zoom = enroll_scale;
    }
}
