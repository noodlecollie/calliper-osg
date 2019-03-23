#ifndef GENERICSETTING_H
#define GENERICSETTING_H

#include "Settings/basesetting.h"

#include <QVariant>
#include <limits>

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

    explicit GenericSetting(quint32 id,
                            ValueType type,
                            const QString& descriptiveName,
                            const QVariant& defaultValue = QVariant(),
                            double min = std::numeric_limits<double>::min(),
                            double max = std::numeric_limits<double>::max());

    virtual ~GenericSetting() override;

    ValueType valueType() const;

    virtual BaseSetting::Type type() const override;
    virtual QVariant value() const override;
    virtual void setValue(const QVariant &newVal) override;

    QVariant defaultValue() const;
    void setDefaultValue(const QVariant& newDefault);

signals:
    void valueChanged(const QVariant& newValue);
    void defaultChanged(const QVariant& newValue);

private:
    QVariant stockDefaultValue() const;

    const ValueType m_Type;
    QVariant m_Value;
    QVariant m_DefaultValue;
    double m_DoubleMin;
    double m_DoubleMax;
};

#endif // GENERICSETTING_H
