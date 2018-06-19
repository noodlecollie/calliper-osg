#include "osgviewwidget.h"

#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>

OSGViewWidget::OSGViewWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    m_GraphicsWindow(new osgViewer::GraphicsWindowEmbedded(x(), y(), width(), height())),
    m_Viewer(new osgViewer::Viewer),
    m_Scale(QApplication::desktop()->devicePixelRatio()) // take care of HDPI screen, e.g. Retina display on Mac
{
    osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3( 0.f, 0.f, 0.f ), 0.25f, 0.5f);
    osg::ShapeDrawable* sd = new osg::ShapeDrawable( cylinder );
    sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(sd);

    osg::Camera* camera = new osg::Camera;
    camera->setViewport(0, 0, width(), height());
    camera->setClearColor(osg::Vec4( 0.9f, 0.9f, 1.f, 1.f ));
    float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
    camera->setGraphicsContext(m_GraphicsWindow);

    m_Viewer->setCamera(camera);
    m_Viewer->setSceneData(geode);
    osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow(false);
    setMouseTracking(true);
    m_Viewer->setCameraManipulator(manipulator);
    m_Viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_Viewer->realize();
}

OSGViewWidget::~OSGViewWidget()
{
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
    osg::Geode* geode = static_cast<osg::Geode*>(m_Viewer->getSceneData());
    osg::StateSet* stateSet = geode->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
}

void OSGViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    getEventQueue()->mouseMotion(event->x() * m_Scale, event->y() * m_Scale);
}

void OSGViewWidget::mousePressEvent(QMouseEvent* event)
{
    unsigned int button = 0;

    switch (event->button())
    {
        case Qt::LeftButton:
        {
            button = 1;
            break;
        }

        case Qt::MiddleButton:
        {
            button = 2;
            break;
        }

        case Qt::RightButton:
        {
            button = 3;
            break;
        }

        default:
        {
            break;
        }
    }

    getEventQueue()->mouseButtonPress(event->x() * m_Scale, event->y() * m_Scale, button);
}

void OSGViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button = 0;

    switch (event->button())
    {
        case Qt::LeftButton:
        {
            button = 1;
            break;
        }

        case Qt::MiddleButton:
        {
            button = 2;
            break;
        }

        case Qt::RightButton:
        {
            button = 3;
            break;
        }

        default:
        {
            break;
        }
    }

    getEventQueue()->mouseButtonRelease(event->x() * m_Scale, event->y() * m_Scale, button);
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
