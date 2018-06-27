#include "settingssubcategory.h"

#include "Settings/basesetting.h"

SettingsSubCategory::SettingsSubCategory(quint32 id, const QString &descripriveName)
    : QObject(Q_NULLPTR),
      m_ID(id),
      m_DescriptiveName(descripriveName)
{
}

SettingsSubCategory::~SettingsSubCategory()
{
}

quint32 SettingsSubCategory::id() const
{
    return m_ID;
}

QString SettingsSubCategory::descriptiveName() const
{
    return m_DescriptiveName;
}

void SettingsSubCategory::setDescriptiveName(const QString &name)
{
    m_DescriptiveName = name;
}

BaseSetting* SettingsSubCategory::addSetting(BaseSetting *setting)
{
    if ( m_Settings.contains(setting->id()) )
    {
        BaseSetting* old = m_Settings.take(setting->id());

        if ( old != setting )
        {
            delete old;
        }
    }

    m_Settings.insert(setting->id(), setting);
    setting->setParent(this);

    return setting;
}

BaseSetting* SettingsSubCategory::setting(quint32 key) const
{
    BaseSetting* setting = m_Settings.value(key, Q_NULLPTR);
    Q_ASSERT_X(setting, Q_FUNC_INFO, "Setting with given key was not found.");
    return setting;
}
