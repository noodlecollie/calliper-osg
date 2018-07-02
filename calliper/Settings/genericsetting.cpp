#include "genericsetting.h"

GenericSetting::GenericSetting(quint32 id, ValueType type, const QString &descriptiveName, const QVariant &defaultValue)
    : BaseSetting(id, descriptiveName),
      m_Type(type),
      m_Value(),
      m_DefaultValue()
{
    m_DefaultValue = static_cast<quint32>(defaultValue.type()) == m_Type ? defaultValue : stockDefaultValue();
    m_Value = m_DefaultValue;
}

GenericSetting::~GenericSetting()
{
}

GenericSetting::ValueType GenericSetting::valueType() const
{
    return m_Type;
}

BaseSetting::Type GenericSetting::type() const
{
    return BaseSetting::Type::GenericSetting;
}

QVariant GenericSetting::value() const
{
    return m_Value;
}

void GenericSetting::setValue(const QVariant &newVal)
{
    if ( static_cast<quint32>(newVal.type()) != m_Type )
    {
        return;
    }

    m_Value = newVal;
    emit valueChanged(m_Value);
}

QVariant GenericSetting::defaultValue() const
{
    return m_DefaultValue;
}

void GenericSetting::setDefaultValue(const QVariant &newDefault)
{
    if ( static_cast<quint32>(newDefault.type()) != m_Type )
    {
        return;
    }

    m_DefaultValue = newDefault;
    emit defaultChanged(m_DefaultValue);
}

QVariant GenericSetting::stockDefaultValue() const
{
    switch ( m_Type )
    {
        case ValueType::Boolean:
        {
            return QVariant(false);
        }

        case ValueType::Integer:
        {
            return QVariant(0);
        }

        case ValueType::Double:
        {
            return QVariant(0.0);
        }

        case ValueType::String:
        {
            return QVariant("");
        }

        default:
        {
            Q_UNREACHABLE();
            return QVariant();
        }
    }
}
