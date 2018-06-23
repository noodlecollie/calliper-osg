#include "viewport2d.h"

Viewport2D::Viewport2D(QWidget *parent)
    : OSGViewWidget(parent)
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
    m_Viewer->setCameraManipulator(m_OrthoController);
}

Viewport2D::~Viewport2D()
{
}

void Viewport2D::resizeGL(int newWidth, int newHeight)
{
    OSGViewWidget::resizeGL(newWidth, newHeight);

    m_Camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
    float widthMult = static_cast<float>(width()) / static_cast<float>(height());
    m_Camera->setProjectionMatrixAsOrtho2D(-1 * widthMult, 1 * widthMult, -1, 1);

    m_OrthoController->setViewportSize(QSize(newWidth, newHeight));
}
