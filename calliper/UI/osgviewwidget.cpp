#include "osgviewwidget.h"

#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include "OSG/osgdefs.h"

OSGViewWidget::OSGViewWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    m_GraphicsWindow(new osgViewer::GraphicsWindowEmbedded(x(), y(), width(), height())),
    m_Viewer(new osgViewer::Viewer()),
    m_Scale(QApplication::desktop()->devicePixelRatio()) // take care of HDPI screen, e.g. Retina display on Mac
{
}

OSGViewWidget::~OSGViewWidget()
{
}

osg::Node* OSGViewWidget::rootNode() const
{
    return m_Viewer->getSceneData();
}

void OSGViewWidget::setRootNode(osg::Node *node)
{
    m_Viewer->setSceneData(node);
}

void OSGViewWidget::initializeGL()
{
    m_Viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_Viewer->realize();
}

void OSGViewWidget::paintGL()
{
    m_Viewer->frame();
}

void OSGViewWidget::resizeGL(int newWidth, int newHeight)
{
    getEventQueue()->windowResize(x() * m_Scale, y() * m_Scale, newWidth * m_Scale, newHeight * m_Scale);
    m_GraphicsWindow->resized(x() * m_Scale, y() * m_Scale, newWidth * m_Scale, newHeight * m_Scale);
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

osgGA::EventQueue* OSGViewWidget::getEventQueue() const
{
    return m_GraphicsWindow->getEventQueue();
}
