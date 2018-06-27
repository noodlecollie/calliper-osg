#ifndef BASESETTINGSCATEGORY_H
#define BASESETTINGSCATEGORY_H

#include <QObject>
#include <QMap>

class BaseSetting;
class SettingsSubCategory;

class BaseSettingsCategory : public QObject
{
public:
    BaseSettingsCategory(const QString& uniqueName, const QString& descriptiveName);
    virtual ~BaseSettingsCategory();

    QString name() const;

    QString descriptiveName() const;
    void setDescriptiveName(const QString& newName);

    BaseSetting* addSetting(BaseSetting* setting);
    SettingsSubCategory* addSubCategory(SettingsSubCategory* subCategory);

    SettingsSubCategory* subCategory(quint32 key) const;
    BaseSetting* setting(quint32 settingKey) const;
    BaseSetting* setting(quint32 subCategoryKey, quint32 settingKey) const;

    template<typename T>
    T* settingAs(quint32 settingKey) const
    {
        T* target = qobject_cast<T*>(setting(settingKey));
        Q_ASSERT_X(target, Q_FUNC_INFO, "Type mismatch when retrieving setting");
        return target;
    }

    template<typename T>
    T* settingAs(quint32 subCategoryKey, quint32 settingKey) const
    {
        T* target = qobject_cast<T*>(setting(subCategoryKey, settingKey));
        Q_ASSERT_X(target, Q_FUNC_INFO, "Type mismatch when retrieving setting");
        return target;
    }

private:
    const QString m_Name;
    QString m_DescriptiveName;

    QMap<quint32, BaseSetting*> m_Settings;
    QMap<quint32, SettingsSubCategory*> m_SubCategories;
};

#endif // BASESETTINGSCATEGORY_H
