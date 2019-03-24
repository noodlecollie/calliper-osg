#ifndef KEYBINDSETTING_H
#define KEYBINDSETTING_H

#include "Settings/basesetting.h"

class QKeyEvent;

class KeyBindSetting : public BaseSetting
{
    Q_OBJECT
public:
    KeyBindSetting(quint32 id, const QString& descriptiveName, Qt::Key defaultValue);
    virtual ~KeyBindSetting() override;

    virtual BaseSetting::Type type() const override;
    virtual QVariant value() const override;
    void setValue(const QVariant &newVal) override;

    Qt::Key key() const;
    void setKey(Qt::Key value);

    Qt::Key defaultKey() const;
    void setDefaultKey(Qt::Key value);

    // For convenience: evaluates a QKeyEvent and, if the event matches this setting's
    // key and is not an autorepeat event, fires the signal keyEvent().
    // Returns whether the event's key matched this key - if true is returned, the event
    // should be considered "consumed". Note that true is still returned if the event
    // was an autorepeat.
    bool evaluateEvent(QKeyEvent* event);

signals:
    void valueChanged(Qt::Key newValue);
    void defaultChanged(Qt::Key newDefault);
    void keyEvent(bool pressed);

private:
    Qt::Key m_Value;
    Qt::Key m_DefaultValue;
};

#endif // KEYBINDSETTING_H
