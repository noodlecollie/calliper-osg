#include "viewport3d.h"

Viewport3D::Viewport3D(QWidget *parent)
    : OSGViewWidget(parent)
{
    m_Camera = new osg::Camera;
    m_Camera->setViewport(0, 0, width(), height());
    m_Camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.0f, 1.0f));
    float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    m_Camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
    m_Camera->setGraphicsContext(m_GraphicsWindow);
    m_Viewer->setCamera(m_Camera);

    m_TrackballManipulator = new osgGA::TrackballManipulator();
    m_TrackballManipulator->setAllowThrow(false);
    setMouseTracking(true);
    m_Viewer->setCameraManipulator(m_TrackballManipulator);
}

Viewport3D::~Viewport3D()
{
}

void Viewport3D::resizeGL(int newWidth, int newHeight)
{
    OSGViewWidget::resizeGL(newWidth, newHeight);

    m_Camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
}

void Viewport3D::mouseMoveEvent(QMouseEvent* event)
{
    OSGViewWidget::mouseMoveEvent(event);
    update();
}

void Viewport3D::mousePressEvent(QMouseEvent* event)
{
    OSGViewWidget::mousePressEvent(event);
    update();
}

void Viewport3D::mouseReleaseEvent(QMouseEvent* event)
{
    OSGViewWidget::mouseReleaseEvent(event);
    update();
}
