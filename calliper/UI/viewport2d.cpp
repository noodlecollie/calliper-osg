#include "viewport2d.h"

#include <QWheelEvent>
#include <QGestureEvent>
#include <QGesture>
#include <QPinchGesture>
#include <QPanGesture>
#include <QVector>
#include <QtDebug>
#include <QTime>

namespace
{
    constexpr float WHEEL_DELTA_ZOOM_FACTOR = 60.0f;
    constexpr float WHEEL_DELTA_SCROLL_FACTOR = 1.0f;
    constexpr float WHEEL_PIXEL_ZOOM_FACTOR = 60.0f;
    constexpr float WHEEL_PIXEL_SCROLL_FACTOR = 1.0f;
    constexpr float SCROLL_ZOOM_FACTOR = 1.0f / 50.0f;

    const QVector<Qt::GestureType> SUPPORTED_GESTURES = QVector<Qt::GestureType>()
            << Qt::PinchGesture;
}

Viewport2D::Viewport2D(QWidget *parent)
    : OSGViewWidget(parent),
      m_Camera(),
      m_OrthoController(),
      m_NavigateWithGestures(false)
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
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_Viewer->setCameraManipulator(m_OrthoController);
    connect(m_OrthoController->signalAdapter(), SIGNAL(updated()), this, SLOT(update()));

    // TODO: REMOVE ME
    setNavigateWithGestures(true);
}

Viewport2D::~Viewport2D()
{
}

bool Viewport2D::navigateWithGestures() const
{
    return m_NavigateWithGestures;
}

void Viewport2D::setNavigateWithGestures(bool enabled)
{
    if ( m_NavigateWithGestures == enabled )
    {
        return;
    }

    m_NavigateWithGestures = enabled;

    if ( m_NavigateWithGestures )
    {
        for ( Qt::GestureType gestureType : SUPPORTED_GESTURES )
        {
            grabGesture(gestureType);
        }
    }
    else
    {
        for ( Qt::GestureType gestureType : SUPPORTED_GESTURES )
        {
            ungrabGesture(gestureType);
        }
    }
}

void Viewport2D::resizeGL(int newWidth, int newHeight)
{
    OSGViewWidget::resizeGL(newWidth, newHeight);

    m_Camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
    float widthMult = static_cast<float>(width()) / static_cast<float>(height());
    m_Camera->setProjectionMatrixAsOrtho2D(-1 * widthMult, 1 * widthMult, -1, 1);

    m_OrthoController->setViewportSize(QSize(newWidth, newHeight));
}

bool Viewport2D::event(QEvent *event)
{
    bool recognised = OSGViewWidget::event(event);

    switch ( event->type() )
    {
        case QEvent::Gesture:
        {
            gestureEvent(static_cast<QGestureEvent*>(event));
            return true;
        }

        default:
        {
            return recognised;
        }
    }
}

void Viewport2D::wheelEvent(QWheelEvent *event)
{
    if ( !m_NavigateWithGestures )
    {
        zoomWithMouseWheel(event);
    }
    else
    {
        scrollWithMouseWheel(event);
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

void Viewport2D::zoomWithMouseWheel(QWheelEvent *event)
{
    const float delta = (static_cast<float>(event->delta()) / WHEEL_DELTA_ZOOM_FACTOR);
    float multiplier = 1.0f;

    if ( delta >= 0.0f )
    {
        multiplier = 1.0f + delta;
    }
    else
    {
        multiplier = 1.0f / (1.0f - delta);
    }

    m_OrthoController->setZoom(m_OrthoController->zoom() * multiplier);
}

void Viewport2D::scrollWithMouseWheel(QWheelEvent *event)
{
    osg::Vec2d transDelta;

    // TODO: Scrolling sideways on the trackpad still affects this?
    // TODO: Shift to scroll sideways?
    transDelta[1] += static_cast<float>(event->delta()) / WHEEL_DELTA_SCROLL_FACTOR;

    // Modify by current zoom, so that if we are zoomed in further, the scroll itself is smaller.
    transDelta *= SCROLL_ZOOM_FACTOR * m_OrthoController->zoom();

    m_OrthoController->setTranslation(m_OrthoController->translation() + transDelta);
}

void Viewport2D::zoomWithPinchGesture(QPinchGesture *gesture)
{
    // Each time this event comes through, there is a scale factor "delta"
    // representing how much nearer or further apart the pinch is from
    // the previous event. We can use this as a multiplier for the zoom.
    m_OrthoController->setZoom(m_OrthoController->zoom() * gesture->scaleFactor());
}
