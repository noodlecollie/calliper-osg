#include "basesettingscategory.h"

#include "Settings/settingsmanager.h"
#include "Settings/basesetting.h"
#include "Settings/settingssubcategory.h"

BaseSettingsCategory::BaseSettingsCategory(const QString &uniqueName, const QString &descriptiveName)
    : QObject(Q_NULLPTR),
      m_Name(uniqueName),
      m_DescriptiveName(descriptiveName),
      m_Settings()
{
}

BaseSettingsCategory::~BaseSettingsCategory()
{
}

QString BaseSettingsCategory::name() const
{
    return m_Name;
}

QString BaseSettingsCategory::descriptiveName() const
{
    return m_DescriptiveName;
}

void BaseSettingsCategory::setDescriptiveName(const QString& newName)
{
    m_DescriptiveName = newName;
}

BaseSetting* BaseSettingsCategory::addSetting(BaseSetting *setting)
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

SettingsSubCategory* BaseSettingsCategory::addSubCategory(SettingsSubCategory *subCategory)
{
    if ( m_SubCategories.contains(subCategory->id()) )
    {
        SettingsSubCategory* old = m_SubCategories.take(subCategory->id());

        if ( old != subCategory )
        {
            delete old;
        }
    }

    m_SubCategories.insert(subCategory->id(), subCategory);
    subCategory->setParent(this);

    return subCategory;
}

SettingsSubCategory* BaseSettingsCategory::subCategory(quint32 key) const
{
    SettingsSubCategory* subcat = m_SubCategories.value(key, Q_NULLPTR);
    Q_ASSERT_X(subcat, Q_FUNC_INFO, "Sub-category with given key was not found.");
    return subcat;
}

BaseSetting* BaseSettingsCategory::setting(quint32 settingKey) const
{
    BaseSetting* target = m_Settings.value(settingKey, Q_NULLPTR);
    Q_ASSERT_X(target, Q_FUNC_INFO, "Setting with given key was not found.");
    return target;
}

BaseSetting* BaseSettingsCategory::setting(quint32 subCategoryKey, quint32 settingKey) const
{
    SettingsSubCategory* subcat = subCategory(subCategoryKey);
    if ( !subcat )
    {
        return Q_NULLPTR;
    }

    return subcat->setting(settingKey);
}
