#include "viewport3d.h"

#include <QTimer>
#include <QKeyEvent>
#include "UI/uisettings.h"

Viewport3D::Viewport3D(QWidget *parent)
    : OSGViewWidget(parent),
      m_Camera(),
      m_FPController(),
      m_FOV(90.0f),
      m_NavigationEnabled(false),
      m_MovementUpdateTimer(new QTimer(this)),
      m_LastMovementUpdateTime(),
      m_3DViewSubCat(UISettings::instance()->subCategory(UISettings::Cat_View3D)),
      m_DefaultCameraFOVSetting(m_3DViewSubCat->settingAs<GenericSetting>(UISettings::View3D_DefaultCameraFOV)),
      m_FarPlaneSetting(m_3DViewSubCat->settingAs<GenericSetting>(UISettings::View3D_FarPlaneDistance)),
      m_ToggleNavigationSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_ToggleNavigation)),
      m_MoveForwardSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_MoveForward)),
      m_MoveBackwardSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_MoveBackward)),
      m_MoveLeftSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_MoveLeft)),
      m_MoveRightSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_MoveRight)),
      m_MoveUpSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_MoveUp)),
      m_MoveDownSetting(m_3DViewSubCat->settingAs<KeyBindSetting>(UISettings::View3D_MoveDown)),
      m_MovementSpeedSetting(m_3DViewSubCat->settingAs<GenericSetting>(UISettings::View3D_MovementSpeed))
{
    m_Camera = new osg::Camera;
    m_Camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.0f, 1.0f));
    m_Camera->setGraphicsContext(m_GraphicsWindow);
    m_Viewer->setCamera(m_Camera);

    m_FPController = new FirstPersonCameraController();
    m_FPController->setAllowThrow(false);
    m_Viewer->setCameraManipulator(m_FPController);

    connect(m_FPController->signalAdapter(), SIGNAL(updated()), this, SLOT(update()));
    connect(m_MovementUpdateTimer, &QTimer::timeout, this, &Viewport3D::updateMovement);

    m_FOV = m_DefaultCameraFOVSetting->value().toFloat();
    connect(m_FarPlaneSetting, SIGNAL(valueChanged(QVariant)), this, SLOT(updateCameraProjection()));
    connect(m_MovementSpeedSetting, &GenericSetting::valueChanged, this, &Viewport3D::setMovementSpeed);

    connect(m_ToggleNavigationSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::toggleNavigationState);
    connect(m_MoveForwardSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::setMoveForward);
    connect(m_MoveBackwardSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::setMoveBackward);
    connect(m_MoveLeftSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::setMoveLeft);
    connect(m_MoveRightSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::setMoveRight);
    connect(m_MoveUpSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::setMoveUp);
    connect(m_MoveDownSetting, &KeyBindSetting::keyEvent, this, &Viewport3D::setMoveDown);

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

void Viewport3D::keyPressEvent(QKeyEvent* event)
{
    // Only allow movement keys when in navigation mode.
    if ( m_NavigationEnabled )
    {
        if ( m_MoveForwardSetting->evaluateEvent(event) ||
             m_MoveBackwardSetting->evaluateEvent(event) ||
             m_MoveLeftSetting->evaluateEvent(event) ||
             m_MoveRightSetting->evaluateEvent(event) ||
             m_MoveUpSetting->evaluateEvent(event) ||
             m_MoveDownSetting->evaluateEvent(event) )
        {
            // One was matched, so event was consumed.
            return;
        }
    }

    // Default handling:
    OSGViewWidget::keyPressEvent(event);
}

void Viewport3D::keyReleaseEvent(QKeyEvent* event)
{    
    if ( m_ToggleNavigationSetting->evaluateEvent(event) )
    {
        return;
    }

    // Only allow movement keys when in navigation mode.
    if ( m_NavigationEnabled )
    {
        if ( m_MoveForwardSetting->evaluateEvent(event) ||
             m_MoveBackwardSetting->evaluateEvent(event) ||
             m_MoveLeftSetting->evaluateEvent(event) ||
             m_MoveRightSetting->evaluateEvent(event) ||
             m_MoveUpSetting->evaluateEvent(event) ||
             m_MoveDownSetting->evaluateEvent(event) )
        {
            // One was matched, so event was consumed.
            return;
        }
    }

    // Default handling:
    OSGViewWidget::keyPressEvent(event);
}

void Viewport3D::updateCameraProjection()
{
    const float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    const float halfFOV = m_FOV / 2.0f;
    const float farPlaneDist = m_FarPlaneSetting->value().toFloat();

    m_Camera->setViewport(0, 0, width() * m_Scale, height() * m_Scale);
    m_Camera->setProjectionMatrixAsPerspective(halfFOV, aspectRatio, 1.f, farPlaneDist);
}

void Viewport3D::setMovementSpeed(const QVariant &speed)
{
    m_FPController->setMovementSpeed(speed.toFloat());
}

void Viewport3D::toggleNavigationState(bool pressed)
{
    // Only when key is released.
    if ( !pressed )
    {
        setNavigationEnabled(!m_NavigationEnabled);
    }
}

void Viewport3D::setNavigationEnabled(bool enabled)
{
    if ( enabled == m_NavigationEnabled )
    {
        return;
    }

    m_NavigationEnabled = enabled;

    if ( m_NavigationEnabled )
    {
        m_LastMovementUpdateTime = QTime(); // Navigation mode is being switched on, so there's no "last" update.
        updateMovement();                   // Update movement on the camera controller.
        m_MovementUpdateTimer->start();     // Enable the timer to refire again soon.
    }
    else
    {
        m_MovementUpdateTimer->stop();      // Cancel any pending refire.
        clearAllMovementKeys();             // Make sure all movement keys are forcibly released.
        updateMovement();                   // Do one final movement update on the controller.
        m_LastMovementUpdateTime = QTime(); // Reset the last update timestamp to an invalid state, for safety.
    }
}

void Viewport3D::updateMovement()
{
    float delta = 0.0f;

    if ( !m_LastMovementUpdateTime.isValid() )
    {
        // Cache timestamp in preparation for next time. Delta remains at 0.
        // This is so that we can "touch" the controller when navigation mode is first enabled.
        m_LastMovementUpdateTime.start();
    }
    else
    {
        int msecElapsed = m_LastMovementUpdateTime.elapsed();

        if ( msecElapsed < 1 )
        {
            // If we've refired so quickly that we haven't even advanced by a millisecond yet,
            // just leave everything as it is and wait until next time.
            return;
        }

        delta = static_cast<float>(msecElapsed) / 1000.0f;
        m_LastMovementUpdateTime.restart();
    }

    m_FPController->updateMovement(delta);
}

void Viewport3D::setMoveForward(bool pressed)
{
    m_FPController->setForward(pressed);
}

void Viewport3D::setMoveBackward(bool pressed)
{
    m_FPController->setBackward(pressed);
}

void Viewport3D::setMoveLeft(bool pressed)
{
    m_FPController->setLeft(pressed);
}

void Viewport3D::setMoveRight(bool pressed)
{
    m_FPController->setRight(pressed);
}

void Viewport3D::setMoveUp(bool pressed)
{
    m_FPController->setUp(pressed);
}

void Viewport3D::setMoveDown(bool pressed)
{
    m_FPController->setDown(pressed);
}

void Viewport3D::clearAllMovementKeys()
{
    m_FPController->clearAllMovement();
}
