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
    };

    enum View2DSettings
    {
        View2D_DragPanKey = 0,
        View2D_MultiTouchZoom,
    };

    UISettings();
};

#endif // UISETTINGS_H
