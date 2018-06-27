#ifndef BASESETTING_H
#define BASESETTING_H

#include <QObject>

class BaseSetting : public QObject
{
    Q_OBJECT
public:
    enum class Type
    {
        GenericSetting = 0,
        KeyBindSetting
    };

    BaseSetting(quint32 id, const QString& descriptiveName);
    virtual ~BaseSetting();

    quint32 id() const;

    QString descriptiveName() const;
    void setDescriptiveName(const QString& name);

    virtual Type type() const = 0;
    virtual QVariant value() const = 0;
    virtual void setValue(const QVariant& newVal) = 0;

private:
    const quint32 m_ID;
    QString m_DescriptiveName;
};

#endif // BASESETTING_H
