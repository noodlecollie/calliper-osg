#include "uisettings.h"

#include "Settings/settingsmanager.h"
#include "Settings/settingssubcategory.h"
#include "Settings/keybindsetting.h"

UISettings::UISettings()
    : BaseSettingsCategory("UISettings", tr("User Interface"))
{
    SettingsSubCategory* catView2D = addSubCategory(new SettingsSubCategory(UISettings::Cat_View2D, tr("2D View")));
    catView2D->addSetting(new KeyBindSetting(UISettings::View2D_DragPanKey, tr("Pan"), Qt::Key_Space));
}
