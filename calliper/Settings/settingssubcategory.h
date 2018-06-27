#ifndef SETTINGSSUBCATEGORY_H
#define SETTINGSSUBCATEGORY_H

#include <QObject>
#include <QMap>

class BaseSetting;

class SettingsSubCategory : public QObject
{
    Q_OBJECT
public:
    SettingsSubCategory(quint32 id, const QString& descripriveName);
    virtual ~SettingsSubCategory();

    quint32 id() const;

    QString descriptiveName() const;
    void setDescriptiveName(const QString& name);

    BaseSetting* addSetting(BaseSetting* setting);
    BaseSetting* setting(quint32 id) const;

    template<typename T>
    T* settingAs(quint32 key) const
    {
        T* target = qobject_cast<T*>(setting(key));
        Q_ASSERT_X(target, Q_FUNC_INFO, "Type mismatch when retrieving setting");
        return target;
    }

private:
    const quint32 m_ID;
    QString m_DescriptiveName;

    QMap<quint32, BaseSetting*> m_Settings;
};

#endif // SETTINGSSUBCATEGORY_H
