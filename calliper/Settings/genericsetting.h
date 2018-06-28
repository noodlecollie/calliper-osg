#ifndef GENERICSETTING_H
#define GENERICSETTING_H

#include "Settings/basesetting.h"

#include <QVariant>

class GenericSetting : public BaseSetting
{
    Q_OBJECT
public:
    enum ValueType
    {
        Boolean = QVariant::Bool,
        Integer = QVariant::Int,
        Double = QVariant::Double,
        String = QVariant::String
    };

    explicit GenericSetting(quint32 id, ValueType type, const QString& descriptiveName, const QVariant& defaultValue = QVariant());
    virtual ~GenericSetting();

    ValueType valueType() const;

    virtual BaseSetting::Type type() const override;
    virtual QVariant value() const override;
    virtual void setValue(const QVariant &newVal) override;

    QVariant defaultValue() const;
    void setDefaultValue(const QVariant& newDefault);

private:
    QVariant stockDefaultValue() const;

    const ValueType m_Type;
    QVariant m_Value;
    QVariant m_DefaultValue;
};

#endif // GENERICSETTING_H