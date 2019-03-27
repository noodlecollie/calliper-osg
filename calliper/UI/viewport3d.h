#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include <QTime>

#include "UI/osgviewwidget.h"
#include "UI/firstpersoncameracontroller.h"

#include "Settings/genericsetting.h"
#include "Settings/keybindsetting.h"
#include "Settings/settingssubcategory.h"

class QTimer;

class Viewport3D : public OSGViewWidget
{
    Q_OBJECT
public:
    explicit Viewport3D(QWidget *parent = nullptr);
    virtual ~Viewport3D() override;

protected:
    virtual void resizeGL(int newWidth, int newHeight) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

private slots:
    void updateCameraProjection();
    void setMovementSpeed(const QVariant& speed);
    void updateMovement();
    void toggleNavigationState(bool pressed);

    void setMoveForward(bool pressed);
    void setMoveBackward(bool pressed);
    void setMoveLeft(bool pressed);
    void setMoveRight(bool pressed);
    void setMoveUp(bool pressed);
    void setMoveDown(bool pressed);

private:
    void setNavigationEnabled(bool enabled);
    void clearAllMovementKeys();

    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<FirstPersonCameraController> m_FPController;
    float m_FOV;
    bool m_NavigationEnabled;
    QTimer* m_MovementUpdateTimer;
    QTime m_LastMovementUpdateTime;

    SettingsSubCategory* m_3DViewSubCat;
    GenericSetting* m_DefaultCameraFOVSetting;
    GenericSetting* m_FarPlaneSetting;
    KeyBindSetting* m_ToggleNavigationSetting;
    KeyBindSetting* m_MoveForwardSetting;
    KeyBindSetting* m_MoveBackwardSetting;
    KeyBindSetting* m_MoveLeftSetting;
    KeyBindSetting* m_MoveRightSetting;
    KeyBindSetting* m_MoveUpSetting;
    KeyBindSetting* m_MoveDownSetting;
    GenericSetting* m_MovementSpeedSetting;
};

#endif // VIEWPORT3D_H
