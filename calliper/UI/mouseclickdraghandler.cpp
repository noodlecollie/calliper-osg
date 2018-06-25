#include "mouseclickdraghandler.h"

#include <QEvent>
#include <QtDebug>
#include <QMouseEvent>

MouseClickDragHandler::MouseClickDragHandler(QObject *parent)
    : QObject(parent),
      m_Enabled(true),
      m_InDrag(false),
      m_Begin(),
      m_Last(),
      m_Current()
{
    if ( !parent )
    {
        return;
    }

    parent->installEventFilter(this);
}

bool MouseClickDragHandler::enabled() const
{
    return m_Enabled;
}

void MouseClickDragHandler::setEnabled(bool isEnabled)
{
    if ( isEnabled == m_Enabled )
    {
        return;
    }

    m_Enabled = isEnabled;

    if ( !m_Enabled )
    {
        endDrag();
    }
}

bool MouseClickDragHandler::eventFilter(QObject *watched, QEvent *event)
{
    if ( !m_Enabled )
    {
        return QObject::eventFilter(watched, event);
    }

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            beginDrag(static_cast<QMouseEvent*>(event));
            return true;
        }

        case QEvent::MouseMove:
        {
            moveDrag(static_cast<QMouseEvent*>(event));
            return true;
        }

        case QEvent::MouseButtonRelease:
        {
            endDrag();
            return true;
        }

        default:
        {
            return QObject::eventFilter(watched, event);
        }
    }
}

void MouseClickDragHandler::beginDrag(QMouseEvent *event)
{
    if ( m_InDrag )
    {
        return;
    }

    m_InDrag = true;
    m_Begin = event->pos();
    m_Last = m_Begin;
    m_Current = m_Begin;

    emit dragBegin(m_Begin);
}

void MouseClickDragHandler::moveDrag(QMouseEvent *event)
{
    if ( !m_InDrag )
    {
        return;
    }

    m_Last = m_Current;
    m_Current = event->pos();

    emit dragMove(m_Begin, m_Last, m_Current);
}

void MouseClickDragHandler::endDrag()
{
    if ( !m_InDrag )
    {
        return;
    }

    emit dragEnd(m_Begin, m_Last);
    m_InDrag = false;
}
