#ifndef QUADVIEWPORTWIDGET_H
#define QUADVIEWPORTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QVector2D>
#include <QCursor>

class QuadViewportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QuadViewportWidget(QWidget *parent = nullptr);

    enum ViewportPosition
    {
        UpperLeft = 0,
        UpperRight,
        LowerLeft,
        LowerRight
    };

signals:

public slots:

private:
    friend class Divider;
    class Divider : public QPushButton
    {
    public:
        enum DividerType
        {
            Top = 0,
            Bottom,
            Left,
            Right,
            Middle
        };

        static constexpr quint32 DIVIDER_COUNT = 5;
        static constexpr quint32 DIVIDER_WIDTH = 5;

        explicit Divider(QuadViewportWidget* parentWidget, DividerType type, QWidget* parent = Q_NULLPTR)
            : QPushButton(parent),
              m_ParentWidget(parentWidget),
              m_Type(type)
        {
            setFlat(true);

            switch ( m_Type )
            {
                case Left:
                case Right:
                {
                    cursorShape = Qt::SizeVerCursor;
                    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    break;
                }

                case Top:
                case Bottom:
                {
                    cursorShape = Qt::SizeHorCursor;
                    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
                    break;
                }

                case Middle:
                {
                    cursorShape = Qt::SizeAllCursor;
                    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                    break;
                }
            }
        }

        inline bool canDragHorizontally() const
        {
            return m_Type == Top || m_Type == Bottom || m_Type == Middle;
        }

        inline bool canDragVertically() const
        {
            return m_Type == Left || m_Type == Right || m_Type == Middle;
        }

        virtual QSize sizeHint() const override
        {
            return QSize(DIVIDER_WIDTH, DIVIDER_WIDTH);
        }

        QuadViewportWidget* m_ParentWidget;
        DividerType m_Type;
        QPoint originalDragPos;
        Qt::CursorShape cursorShape;

    protected:
        virtual void mousePressEvent(QMouseEvent* ) override
        {
            // Do nothing, so the event doesn't get consumed by the subclass.
        }

        virtual void mouseMoveEvent(QMouseEvent* event) override
        {
            m_ParentWidget->dividerMoveEvent(*this, event);
        }

        virtual void mouseReleaseEvent(QMouseEvent* ) override
        {
            // Do nothing, so the event doesn't get consumed by the subclass.
        }

        virtual void enterEvent(QEvent*) override
        {
            setCursor(cursorShape);
        }

        virtual void leaveEvent(QEvent*) override
        {
            unsetCursor();
        }
    };

    void createViewportContainer(quint32 row, quint32 column);
    void createDivider(quint32 row, quint32 column);
    void dividerMoveEvent(Divider& divider, QMouseEvent* event);
    void updateLayoutFromProportions();

    static QWidget* createPlaceholderViewportWidget(quint32 index);

    static constexpr quint32 VIEWPORT_COUNT = 4;

    QVector<QWidget*> m_ViewportContainers;
    QVector<Divider*> m_Dividers;
    QVector2D m_ViewProportions;
};

#endif // QUADVIEWPORTWIDGET_H
