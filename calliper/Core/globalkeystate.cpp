#include "globalkeystate.h"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>

GlobalKeyState::GlobalKeyState()
    : QObject(Q_NULLPTR)
{
    Q_ASSERT_X(qApp, Q_FUNC_INFO, "Application must be initialised first!");
    qApp->installEventFilter(this);
}

GlobalKeyState::~GlobalKeyState()
{
    if ( qApp )
    {
        qApp->removeEventFilter(this);
    }
}

bool GlobalKeyState::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);

    switch ( event->type() )
    {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        {
            const int key = static_cast<QKeyEvent*>(event)->key();
            if ( key != 0 && key !=- Qt::Key_unknown  )
            {
                m_KeyState[key] = event->type() == QEvent::KeyPress;
            }

            break;
        }

        default:
        {
            break;
        }
    }

    return false;
}
