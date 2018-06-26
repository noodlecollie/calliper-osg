#include "basesettingscategory.h"

#include "Settings/settingsmanager.h"

BaseSettingsCategory::BaseSettingsCategory(const QString &uniqueName, const QString &descriptiveName)
    : QObject(Q_NULLPTR),
      m_Name(uniqueName),
      m_DescriptiveName(descriptiveName)
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
