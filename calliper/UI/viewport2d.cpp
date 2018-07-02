#include "viewport2d.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGestureEvent>
#include <QGesture>
#include <QPinchGesture>
#include <QPanGesture>
#include <QVector>
#include <QtDebug>
#include <QTime>
#include <QtMath>

#include "UI/mouseclickdraghandler.h"
#include "Core/globalkeystate.h"
#include "UI/uisettings.h"
#include "OSG/osgdefs.h"

namespace
{
    constexpr float WHEEL_DELTA_ZOOM_FACTOR = 60.0f;
    constexpr float WHEEL_DELTA_SCROLL_FACTOR = 1.0f;
    constexpr float WHEEL_PIXEL_ZOOM_FACTOR = 25.0f;
    constexpr float SCROLL_ZOOM_FACTOR = 2.0f;
}

Viewport2D::Viewport2D(QWidget *parent)
    : OSGViewWidget(parent),
      m_Camera(),
      m_OrthoController(),
      m_MultiTouchZoom(false),
      m_InMultiTouchScroll(false),
      m_DragToScroll(false),
      m_DragHandler(new MouseClickDragHandler(this)),
      m_2DViewSubCat(UISettings::instance()->subCategory(UISettings::Cat_View2D)),
      m_PanKeyBind(m_2DViewSubCat->settingAs<KeyBindSetting>(UISettings::View2D_DragPanKey)),
      m_MultiTouchZoomSetting(m_2DViewSubCat->settingAs<GenericSetting>(UISettings::View2D_MultiTouchZoom))
{
    m_Camera = new osg::Camera;
    m_Camera->setViewport(0, 0, width(), height());
    m_Camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.0f, 1.0f));
    float widthMult = static_cast<float>(width()) / static_cast<float>(height());
    m_Camera->setProjectionMatrixAsOrtho2D(-1 * widthMult, 1 * widthMult, -1, 1);
    m_Camera->setGraphicsContext(m_GraphicsWindow);
    m_Viewer->setCamera(m_Camera);

    m_OrthoController = new OrthographicCameraController();
    m_OrthoController->setAllowThrow(false);
    m_Viewer->setCameraManipulator(m_OrthoController);
    connect(m_OrthoController->signalAdapter(), SIGNAL(updated()), this, SLOT(update()));
    connect(m_OrthoController->signalAdapter(), SIGNAL(projectionUpdated()), this, SLOT(updateCameraProjection()));

    connect(m_DragHandler, &MouseClickDragHandler::dragMove, this, &Viewport2D::handleDragMove);
    m_DragHandler->setEnabled(false);
}

Viewport2D::~Viewport2D()
{
}

bool Viewport2D::navigateWithGestures() const
{
    return m_MultiTouchZoom;
}

void Viewport2D::setNavigateWithGestures(bool enabled)
{
    if ( m_MultiTouchZoom == enabled )
    {
        return;
    }

    m_MultiTouchZoom = enabled;

    if ( m_MultiTouchZoom )
    {
        grabGesture(Qt::PinchGesture);
    }
    else
    {
        ungrabGesture(Qt::PinchGesture);
    }
}

Viewport2D::ViewMode Viewport2D::viewMode() const
{
    return m_OrthoController->viewMode();
}

void Viewport2D::setViewMode(ViewMode mode)
{
    m_OrthoController->setViewMode(mode);
}

void Viewport2D::resizeGL(int newWidth, int newHeight)
{
    OSGViewWidget::resizeGL(newWidth, newHeight);

    m_Camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
    updateCameraProjection();

    m_OrthoController->setViewportSize(QSize(newWidth, newHeight));
}

bool Viewport2D::event(QEvent *event)
{
    switch ( event->type() )
    {
        case QEvent::Gesture:
        {
            if ( m_MultiTouchZoom )
            {
                // For now we just eat all gestures, as it's unclear whether we
                // can pass on the ones we didn't handle.
                gestureEvent(static_cast<QGestureEvent*>(event));
                return true;
            }

            break;
        }

        default:
        {
            break;
        }
    }

    return OSGViewWidget::event(event);
}

void Viewport2D::wheelEvent(QWheelEvent *event)
{
    // We always want to do things with scroll events, so don't call the parent implementation.

    updateMultiTouchScrollState(event);

    if ( m_MultiTouchZoom )
    {
        scrollWithMouseWheel(event);
    }
    else
    {
        zoomWithMouseWheel(event);
    }
}

void Viewport2D::gestureEvent(QGestureEvent *event)
{
    QList<QGesture*> active = event->activeGestures();

    for ( QGesture* gesture : active )
    {
        switch ( gesture->gestureType() )
        {
            case Qt::PinchGesture:
            {
                zoomWithPinchGesture(static_cast<QPinchGesture*>(gesture));
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

void Viewport2D::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == m_PanKeyBind->key() )
    {
        if ( !event->isAutoRepeat() )
        {
            setDragToScrollMode(true);
        }
    }
    else
    {
        OSGViewWidget::keyPressEvent(event);
    }
}

void Viewport2D::keyReleaseEvent(QKeyEvent *event)
{
    if ( event->key() == m_PanKeyBind->key() )
    {
        if ( !event->isAutoRepeat() )
        {
            setDragToScrollMode(false);
        }
    }
    else
    {
        OSGViewWidget::keyReleaseEvent(event);
    }
}

void Viewport2D::focusInEvent(QFocusEvent *event)
{
    OSGViewWidget::focusInEvent(event);

    setDragToScrollMode(GlobalKeyState::singleton()->keyIsPressed(Qt::Key_Space));
}

void Viewport2D::focusOutEvent(QFocusEvent *event)
{
    OSGViewWidget::focusOutEvent(event);

    m_InMultiTouchScroll = false;
    setDragToScrollMode(false);
}

void Viewport2D::updateMultiTouchScrollState(QWheelEvent *event)
{
    m_InMultiTouchScroll = event->phase() == Qt::ScrollBegin ||
                         event->phase() == Qt::ScrollUpdate ||
                         !event->pixelDelta().isNull();
}

void Viewport2D::zoomWithMouseWheel(QWheelEvent *event)
{
    const float delta = m_InMultiTouchScroll
            ? (static_cast<float>(event->pixelDelta().y()) / WHEEL_PIXEL_ZOOM_FACTOR)
            : (static_cast<float>(event->delta()) / WHEEL_DELTA_ZOOM_FACTOR);
    float multiplier = 1.0f;

    if ( delta >= 0.0f )
    {
        multiplier = 1.0f + delta;
    }
    else
    {
        multiplier = 1.0f / (1.0f - delta);
    }

    QPoint centre = centreOfView();
    QPoint mouseDelta = event->pos() - centre;
    osg::Vec2d fMouseDelta(-mouseDelta.x(), mouseDelta.y());
    osg::Vec2d translation = m_OrthoController->translation()
                             - (fMouseDelta * m_OrthoController->zoom())
                             + (fMouseDelta * (m_OrthoController->zoom() * multiplier));

    m_OrthoController->setZoom(m_OrthoController->zoom() * multiplier);
    m_OrthoController->setTranslation(translation);
}

void Viewport2D::scrollWithMouseWheel(QWheelEvent *event)
{
    osg::Vec2d transDelta;

    if ( m_InMultiTouchScroll )
    {
        QPoint delta = event->pixelDelta();
        transDelta[0] = static_cast<float>(-delta.x());
        transDelta[1] = static_cast<float>(delta.y());
    }
    else
    {
        // TODO: Shift to scroll sideways?
        transDelta[1] += static_cast<float>(event->delta()) / WHEEL_DELTA_SCROLL_FACTOR;
    }

    // Modify by current zoom, so that if we are zoomed in further, the scroll itself is smaller.
    transDelta *= SCROLL_ZOOM_FACTOR * m_OrthoController->zoom();

    m_OrthoController->setTranslation(m_OrthoController->translation() + transDelta);
}

void Viewport2D::zoomWithPinchGesture(QPinchGesture *gesture)
{
    // Each time this event comes through, there is a scale factor "delta"
    // representing how much nearer or further apart the pinch is from
    // the previous event.
    m_OrthoController->setZoom(m_OrthoController->zoom() / gesture->scaleFactor());
}

void Viewport2D::setDragToScrollMode(bool enabled)
{
    if ( m_DragToScroll == enabled )
    {
        return;
    }

    m_DragToScroll = enabled;

    if ( m_DragToScroll )
    {
        setCursor(Qt::SizeAllCursor);
        m_DragHandler->setEnabled(true);
    }
    else
    {
        unsetCursor();
        m_DragHandler->setEnabled(false);
    }
}

void Viewport2D::handleDragMove(const QPoint& begin, const QPoint& last, const QPoint& current)
{
    Q_UNUSED(begin);

    QPoint delta = current - last;

    // Delta is inverted because we want to drag + scroll the background.
    osg::Vec2d fDelta(-delta.x(), delta.y());
    fDelta *= m_OrthoController->zoom();

    m_OrthoController->setTranslation(m_OrthoController->translation() + fDelta);
}

QPoint Viewport2D::centreOfView() const
{
    return QPoint(width() / 2, height() / 2);
}

void Viewport2D::updateCameraProjection()
{
    const float halfWidth = static_cast<float>(width()) / 2.0f;
    const float halfHeight = static_cast<float>(height()) / 2.0f;
    const float zoom = m_OrthoController->zoom();

    m_Camera->setProjectionMatrixAsOrtho(-halfWidth * zoom, halfWidth * zoom, -halfHeight * zoom, halfHeight * zoom, 0, -10000);
    update();
}
