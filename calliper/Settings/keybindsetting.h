#ifndef KEYBINDSETTING_H
#define KEYBINDSETTING_H

#include "Settings/basesetting.h"

class KeyBindSetting : public BaseSetting
{
    Q_OBJECT
public:
    KeyBindSetting(quint32 id, const QString& descriptiveName, Qt::Key defaultValue);
    virtual ~KeyBindSetting();

    virtual BaseSetting::Type type() const override;
    virtual QVariant value() const override;
    void setValue(const QVariant &newVal) override;

    Qt::Key key() const;
    void setKey(Qt::Key value);

    Qt::Key defaultKey() const;
    void setDefaultKey(Qt::Key value);

signals:
    void valueChanged(Qt::Key newValue);
    void defaultChanged(Qt::Key newDefault);

private:
    Qt::Key m_Value;
    Qt::Key m_DefaultValue;
};

#endif // KEYBINDSETTING_H
