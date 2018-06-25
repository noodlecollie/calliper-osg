#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "UI/osgviewwidget.h"
#include "UI/orthographiccameracontroller.h"

class QGestureEvent;
class QPinchGesture;
class QPanGesture;

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

private:
    void zoomWithMouseWheel(QWheelEvent* event);
    void scrollWithMouseWheel(QWheelEvent* event);
    void gestureEvent(QGestureEvent* event);
    void zoomWithPinchGesture(QPinchGesture* gesture);
    void updateMultiTouchScrollState(QWheelEvent* event);

    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<OrthographicCameraController> m_OrthoController;
    bool m_NavigateWithGestures;
    bool m_MultiTouchScroll;
};

#endif // VIEWPORT2D_H
