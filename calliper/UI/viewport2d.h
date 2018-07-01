#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "UI/osgviewwidget.h"
#include "UI/orthographiccameracontroller.h"
#include "Settings/keybindsetting.h"
#include "Settings/genericsetting.h"
#include "Settings/settingssubcategory.h"

class QGestureEvent;
class QPinchGesture;
class QPanGesture;
class MouseClickDragHandler;

class Viewport2D : public OSGViewWidget
{
    Q_OBJECT
public:
    typedef OrthographicCameraController::ViewMode ViewMode;

    explicit Viewport2D(QWidget *parent = nullptr);
    virtual ~Viewport2D();

    // TODO: Remove these and hook up to signals from the setting.
    // Right now the setting is sitting there not doing anything.
    bool navigateWithGestures() const;
    void setNavigateWithGestures(bool enabled);

    ViewMode viewMode() const;
    void setViewMode(ViewMode mode);

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
    void updateCameraProjection();

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
    bool m_MultiTouchZoom;
    bool m_InMultiTouchScroll;
    bool m_DragToScroll;
    MouseClickDragHandler* m_DragHandler;

    SettingsSubCategory* m_2DViewSubCat;
    KeyBindSetting* m_PanKeyBind;
    GenericSetting* m_MultiTouchZoomSetting;
};

#endif // VIEWPORT2D_H
