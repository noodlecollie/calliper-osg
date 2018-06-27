#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "UI/osgviewwidget.h"
#include "UI/orthographiccameracontroller.h"
#include "Settings/keybindsetting.h"
#include "Settings/settingssubcategory.h"

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
    void handleDragMove(const QPoint& begin, const QPoint& last, const QPoint& current);

private:
    void zoomWithMouseWheel(QWheelEvent* event);
    void scrollWithMouseWheel(QWheelEvent* event);
    void gestureEvent(QGestureEvent* event);
    void zoomWithPinchGesture(QPinchGesture* gesture);
    void updateMultiTouchScrollState(QWheelEvent* event);
    void setDragToScrollMode(bool enabled);
    QPoint centreOfView() const;

    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<OrthographicCameraController> m_OrthoController;
    bool m_NavigateWithGestures;
    bool m_MultiTouchScroll;
    bool m_DragToScroll;
    MouseClickDragHandler* m_DragHandler;

    SettingsSubCategory* m_2DViewSubCat;
    KeyBindSetting* m_PanKeyBind;
};

#endif // VIEWPORT2D_H
