#include "genericsetting.h"

namespace
{
    inline double clampDouble(double value, double min, double max)
    {
        if ( value < min )
        {
            return min;
        }

        if ( value > max )
        {
            return max;
        }

        return value;
    }

    QVariant clampIfNumeric(const QVariant& orig, double min, double max)
    {
        switch ( static_cast<quint32>(orig.type()) )
        {
            case GenericSetting::Integer:
            {
                int roundedInt = qRound(clampDouble(static_cast<double>(orig.toInt()), min, max));

                // Adjust if the rounding put us over the bounds.
                if ( roundedInt < min )
                {
                    ++roundedInt;
                }
                else if ( roundedInt > max )
                {
                    --roundedInt;
                }

                return QVariant(roundedInt);
            }

            case GenericSetting::Double:
            {
                return QVariant(clampDouble(orig.toDouble(), min, max));
            }

            default:
            {
                return orig;
            }
        }
    }
}

GenericSetting::GenericSetting(quint32 id,
                               ValueType type,
                               const QString &descriptiveName,
                               const QVariant &defaultValue,
                               double min,
                               double max)
    : BaseSetting(id, descriptiveName),
      m_Type(type),
      m_Value(),
      m_DefaultValue(),
      m_DoubleMin(min),
      m_DoubleMax(max)
{
    m_DefaultValue = clampIfNumeric(static_cast<quint32>(defaultValue.type()) == m_Type ? defaultValue : stockDefaultValue(), m_DoubleMin, m_DoubleMax);
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

    QVariant candidate = clampIfNumeric(newVal, m_DoubleMin, m_DoubleMax);
    if ( candidate == m_Value )
    {
        return;
    }

    m_Value = candidate;
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

    QVariant candidate = clampIfNumeric(newDefault, m_DoubleMin, m_DoubleMax);
    if ( candidate == m_DefaultValue )
    {
        return;
    }

    m_DefaultValue = candidate;
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
