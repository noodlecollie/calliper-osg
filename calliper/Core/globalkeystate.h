#ifndef GLOBALKEYSTATE_H
#define GLOBALKEYSTATE_H

#include <QObject>
#include <QHash>
#include "Core/globalsingleton.h"

// For some reason Qt doesn't provide the ability to query a key state.
// Hence we have to keep track of states here.
// This singleton should be instanciated after the application has been created.
class GlobalKeyState : public QObject,
                       public GlobalSingleton<GlobalKeyState>
{
    Q_OBJECT
public:
    GlobalKeyState();
    virtual ~GlobalKeyState();

    inline bool keyIsPressed(Qt::Key key) const
    {
        // Default-constructed value in the hash should be false.
        return m_KeyState[key];
    }

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QHash<int, bool> m_KeyState;
};

#endif // GLOBALKEYSTATE_H
