#include "uisettings.h"

#include "Settings/settingsmanager.h"
#include "Settings/settingssubcategory.h"
#include "Settings/keybindsetting.h"
#include "Settings/genericsetting.h"

UISettings::UISettings()
    : BaseSettingsCategory("UISettings", tr("User Interface"))
{
    SettingsSubCategory* catView2D = addSubCategory(new SettingsSubCategory(Cat_View2D, tr("2D View")));
    catView2D->addSetting(new KeyBindSetting(UISettings::View2D_DragPanKey, tr("Pan"), Qt::Key_Space));
    catView2D->addSetting(new GenericSetting(View2D_MultiTouchZoom, GenericSetting::ValueType::Boolean, tr("Multitouch Zoom"), QVariant(false)));

    SettingsSubCategory* catView3D = addSubCategory(new SettingsSubCategory(Cat_View3D, tr("3D View")));
    catView3D->addSetting(new GenericSetting(View3D_DefaultCameraFOV,
                                             GenericSetting::ValueType::Double,
                                             tr("Default Field of View"),
                                             QVariant(90.0),
                                             75.0,
                                             100.0));
    catView3D->addSetting(new GenericSetting(View3D_FarPlaneDistance,
                                             GenericSetting::ValueType::Double,
                                             tr("Far Plane Distance"),
                                             QVariant(1000.0),
                                             10.0,
                                             50000.0));

    catView3D->addSetting(new KeyBindSetting(View3D_ToggleNavigation, tr("Toggle Navigation"), Qt::Key_Space));
    catView3D->addSetting(new KeyBindSetting(View3D_MoveForward, tr("Forward"), Qt::Key_W));
    catView3D->addSetting(new KeyBindSetting(View3D_MoveBackward, tr("Backward"), Qt::Key_S));
    catView3D->addSetting(new KeyBindSetting(View3D_MoveLeft, tr("Left"), Qt::Key_A));
    catView3D->addSetting(new KeyBindSetting(View3D_MoveRight, tr("Right"), Qt::Key_D));
    catView3D->addSetting(new KeyBindSetting(View3D_MoveUp, tr("Up"), Qt::Key_Q));
    catView3D->addSetting(new KeyBindSetting(View3D_MoveDown, tr("Down"), Qt::Key_Z));

    catView3D->addSetting(new GenericSetting(View3D_MovementSpeed,
                                             GenericSetting::ValueType::Double,
                                             tr("Movement Speed"),
                                             QVariant(150.0),
                                             10.0,
                                             500.0));
}
