#ifndef BASESETTINGSCATEGORY_H
#define BASESETTINGSCATEGORY_H

#include <QObject>

class BaseSettingsCategory : public QObject
{
public:
    BaseSettingsCategory(const QString& uniqueName, const QString& descriptiveName);
    virtual ~BaseSettingsCategory();

    QString name() const;

    QString descriptiveName() const;
    void setDescriptiveName(const QString& newName);

private:
    const QString m_Name;
    QString m_DescriptiveName;
};

#endif // BASESETTINGSCATEGORY_H
