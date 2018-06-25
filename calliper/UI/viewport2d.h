#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "UI/osgviewwidget.h"
#include "UI/orthographiccameracontroller.h"

class QGestureEvent;
class QPinchGesture;
class QPanGesture;
class MouseClickDragHandler;

class Viewport2D : public OSGViewWidget
{
    Q_OBJECT
public:
    explicit Viewport2D(QWidget *parent = nullptr);
    virtual ~Viewport2D();

    bool navigateWithGestures() const;
    void setNavigateWithGestures(bool enabled);

protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeGL(int newWidth, int newHeight) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void handleDragBegin(const QPoint& begin);
    void handleDragMove(const QPoint& begin, const QPoint& last, const QPoint& current);
    void handleDragEnd(const QPoint& begin, const QPoint& last);

private:
    void zoomWithMouseWheel(QWheelEvent* event);
    void scrollWithMouseWheel(QWheelEvent* event);
    bool gestureEvent(QGestureEvent* event);
    void zoomWithPinchGesture(QPinchGesture* gesture);
    void updateMultiTouchScrollState(QWheelEvent* event);
    void setDragToScrollMode(bool enabled);

    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<OrthographicCameraController> m_OrthoController;
    bool m_NavigateWithGestures;
    bool m_MultiTouchScroll;
    bool m_DragToScroll;
    MouseClickDragHandler* m_DragHandler;
};

#endif // VIEWPORT2D_H
