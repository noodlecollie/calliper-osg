#include "osgviewwidget.h"

#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include "OSG/osgdefs.h"

OSGViewWidget::OSGViewWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    m_GraphicsWindow(new osgViewer::GraphicsWindowEmbedded(x(), y(), width(), height())),
    m_Viewer(new osgViewer::Viewer),
    m_Scale(QApplication::desktop()->devicePixelRatio()) // take care of HDPI screen, e.g. Retina display on Mac
{
    osg::Camera* camera = new osg::Camera;
    camera->setViewport(0, 0, width(), height());
    camera->setClearColor(osg::Vec4( 0.9f, 0.9f, 1.f, 1.f ));
    float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
    camera->setGraphicsContext(m_GraphicsWindow);

    m_Viewer->setCamera(camera);
    m_TrackballManipulator = new osgGA::TrackballManipulator;
    m_TrackballManipulator->setAllowThrow(false);
    setMouseTracking(true);
    m_Viewer->setCameraManipulator(m_TrackballManipulator);
    m_Viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_Viewer->realize();
}

OSGViewWidget::~OSGViewWidget()
{
}

osgViewer::Viewer* OSGViewWidget::viewer()
{
    return m_Viewer;
}

const osgViewer::Viewer* OSGViewWidget::viewer() const
{
    return m_Viewer;
}

osg::Camera* OSGViewWidget::camera()
{
    return m_Viewer->getCamera();
}

const osg::Camera* OSGViewWidget::camera() const
{
    return m_Viewer->getCamera();
}

bool OSGViewWidget::usingTrackballManipulator() const
{
    return m_Viewer->getCameraManipulator();
}

void OSGViewWidget::setUsingTrackballManipulator(bool enabled)
{
    if ( usingTrackballManipulator() == enabled )
    {
        return;
    }

    if ( enabled )
    {
        m_Viewer->setCameraManipulator(m_TrackballManipulator);
    }
    else
    {
        m_Viewer->setCameraManipulator(Q_NULLPTR);
    }
}

osg::Node* OSGViewWidget::rootNode() const
{
    return m_Viewer->getSceneData();
}

void OSGViewWidget::setRootNode(osg::Node *node)
{
    m_Viewer->setSceneData(node);
}

void OSGViewWidget::paintGL()
{
    m_Viewer->frame();
}

void OSGViewWidget::resizeGL(int newWidth, int newHeight)
{
    getEventQueue()->windowResize(x() * m_Scale, y() * m_Scale, newWidth * m_Scale, newHeight * m_Scale);
    m_GraphicsWindow->resized(x() * m_Scale, y() * m_Scale, newWidth * m_Scale, newHeight * m_Scale);

    osg::Camera* camera = m_Viewer->getCamera();
    camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
}

void OSGViewWidget::initializeGL()
{
}

void OSGViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    getEventQueue()->mouseMotion(event->x() * m_Scale, event->y() * m_Scale);
}

void OSGViewWidget::mousePressEvent(QMouseEvent* event)
{
    getEventQueue()->mouseButtonPress(event->x() * m_Scale, event->y() * m_Scale, OSGDefs::QtToOSGMouseButton(event->button()));
}

void OSGViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    getEventQueue()->mouseButtonRelease(event->x() * m_Scale, event->y() * m_Scale, OSGDefs::QtToOSGMouseButton(event->button()));
}

void OSGViewWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0
        ? osgGA::GUIEventAdapter::SCROLL_UP
        : osgGA::GUIEventAdapter::SCROLL_DOWN;

    getEventQueue()->mouseScroll(motion);
}

bool OSGViewWidget::event(QEvent* event)
{
    bool handled = QOpenGLWidget::event(event);
    update();
    return handled;
}

osgGA::EventQueue* OSGViewWidget::getEventQueue() const
{
    return m_GraphicsWindow->getEventQueue();
}
