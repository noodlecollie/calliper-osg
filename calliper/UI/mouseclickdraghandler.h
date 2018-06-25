#ifndef MOUSECLICKDRAGHANDLER_H
#define MOUSECLICKDRAGHANDLER_H

#include <QObject>
#include <QPoint>

class QMouseEvent;

class MouseClickDragHandler : public QObject
{
    Q_OBJECT
public:
    explicit MouseClickDragHandler(QObject *parent = nullptr);

    bool enabled() const;
    void setEnabled(bool isEnabled);

signals:
    void dragBegin(const QPoint& begin);
    void dragMove(const QPoint& begin, const QPoint& last, const QPoint& current);
    void dragEnd(const QPoint& begin, const QPoint& last);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void beginDrag(QMouseEvent* event);
    void moveDrag(QMouseEvent* event);
    void endDrag();

    bool m_Enabled;
    bool m_InDrag;
    QPoint m_Begin;
    QPoint m_Last;
    QPoint m_Current;
};

#endif // MOUSECLICKDRAGHANDLER_H
