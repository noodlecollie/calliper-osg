#include "keybindsetting.h"

#include <QVariant>

KeyBindSetting::KeyBindSetting(quint32 id, const QString &descriptiveName, Qt::Key defaultValue)
    : BaseSetting(id, descriptiveName),
      m_Value(defaultValue),
      m_DefaultValue(defaultValue)
{
}

KeyBindSetting::~KeyBindSetting()
{
}

BaseSetting::Type KeyBindSetting::type() const
{
    return BaseSetting::Type::KeyBindSetting;
}

QVariant KeyBindSetting::value() const
{
    return QVariant(key());
}

void KeyBindSetting::setValue(const QVariant &newVal)
{
    if ( newVal.type() != QVariant::Int )
    {
        return;
    }

    // Unfortunately there's no way of currently checking whether the value is a valid key.
    // Really what we'd want to do would be to create our own enum of keys as part of this
    // object and declare it as a meta enum, so we can load/save strings instead of integers
    // and map them back to integer values. Until we definitely have a need for that, this
    // will have to do.
    setKey(static_cast<Qt::Key>(newVal.toInt()));
}

Qt::Key KeyBindSetting::key() const
{
    return m_Value;
}

void KeyBindSetting::setKey(Qt::Key value)
{
    if ( m_Value == value )
    {
        return;
    }

    m_Value = value;
    emit valueChanged(m_Value);
}

Qt::Key KeyBindSetting::defaultKey() const
{
    return m_DefaultValue;
}

void KeyBindSetting::setDefaultKey(Qt::Key value)
{
    if ( m_DefaultValue == value )
    {
        return;
    }

    m_DefaultValue = value;
    emit valueChanged(m_DefaultValue);
}
