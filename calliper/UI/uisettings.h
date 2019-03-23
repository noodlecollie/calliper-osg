#ifndef UISETTINGS_H
#define UISETTINGS_H

#include "Settings/basesettingscategory.h"
#include "Settings/settingscategoryinstance.h"

class UISettings : public BaseSettingsCategory,
                   public SettingsCategoryInstance<UISettings>
{
public:
    enum SubCategories
    {
        Cat_View2D = 0,
        Cat_View3D = 1,
    };

    enum View2DSettings
    {
        View2D_DragPanKey = 0,
        View2D_MultiTouchZoom,
    };

    enum View3DSettings
    {
        View3D_DefaultCameraFOV = 0,
        View3D_FarPlaneDistance,
        View3D_ToggleNavigation,
        View3D_MoveForward,
        View3D_MoveBackward,
        View3D_MoveLeft,
        View3D_MoveRight,
        View3D_MoveUp,
        View3D_MoveDown,
        View3D_MovementSpeed,
    };

    UISettings();
};

#endif // UISETTINGS_H
