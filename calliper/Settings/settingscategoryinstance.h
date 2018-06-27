#ifndef SETTINGSCATEGORYINSTANCE_H
#define SETTINGSCATEGORYINSTANCE_H

#include "Settings/settingsmanager.h"

template<typename T>
class SettingsCategoryInstance
{
public:
    static T* const instance()
    {
        return m_Instance;
    }

private:
    static T* const m_Instance;
};

template<typename T>
T* const SettingsCategoryInstance<T>::m_Instance = SettingsManager::registerSettingsCategory<T>();

#endif // SETTINGSCATEGORYINSTANCE_H
