#include "viewport3d.h"

#include "UI/uisettings.h"

Viewport3D::Viewport3D(QWidget *parent)
    : OSGViewWidget(parent),
      m_Camera(),
      m_FPController(),
      m_FOV(90.0f),
      m_3DViewSubCat(UISettings::instance()->subCategory(UISettings::Cat_View3D)),
      m_DefaultCameraFOVSetting(m_3DViewSubCat->settingAs<GenericSetting>(UISettings::View3D_DefaultCameraFOV)),
      m_FarPlaneSetting(m_3DViewSubCat->settingAs<GenericSetting>(UISettings::View3D_FarPlaneDistance))
{
    m_Camera = new osg::Camera;
    m_Camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.0f, 1.0f));
    m_Camera->setGraphicsContext(m_GraphicsWindow);
    m_Viewer->setCamera(m_Camera);

    m_FPController = new FirstPersonCameraController();
    m_FPController->setAllowThrow(false);
    m_Viewer->setCameraManipulator(m_FPController);

    connect(m_FPController->signalAdapter(), SIGNAL(updated()), this, SLOT(update()));

    m_FOV = m_DefaultCameraFOVSetting->value().toFloat();
    connect(m_FarPlaneSetting, SIGNAL(valueChanged(QVariant)), this, SLOT(updateCameraProjection()));

    updateCameraProjection();
}

Viewport3D::~Viewport3D()
{
}

void Viewport3D::resizeGL(int newWidth, int newHeight)
{
    OSGViewWidget::resizeGL(newWidth, newHeight);

    updateCameraProjection();
}

void Viewport3D::updateCameraProjection()
{
    const float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    const float halfFOV = m_FOV / 2.0f;
    const float farPlaneDist = m_FarPlaneSetting->value().toFloat();

    m_Camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
    m_Camera->setProjectionMatrixAsPerspective(halfFOV, aspectRatio, 1.f, farPlaneDist);
}
