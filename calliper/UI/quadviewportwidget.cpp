#include "quadviewportwidget.h"

// For placeholders
#include <QLabel>
#include "UI/osgviewwidget.h"
#include "Model/applicationmodel.h"

#include <QGridLayout>
#include <QPushButton>
#include <QtDebug>
#include <QMouseEvent>
#include <QVBoxLayout>

namespace
{
    template<typename T>
    T clamp(const T& value, const T& min, const T& max)
    {
        if ( value < min )
        {
            return min;
        }
        else if ( value > max )
        {
            return max;
        }
        else
        {
            return value;
        }
    }
}

QuadViewportWidget::QuadViewportWidget(QWidget *parent)
    : QWidget(parent),
      m_ViewportContainers(),
      m_Dividers(),
      m_ViewProportions(0.5f, 0.5f)
{
    m_ViewportContainers.fill(Q_NULLPTR, VIEWPORT_COUNT);
    m_Dividers.fill(Q_NULLPTR, Divider::DIVIDER_COUNT);

    QGridLayout* gridLayout = new QGridLayout();
    setLayout(gridLayout);

    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    for ( quint32 row = 0; row < 3; ++row )
    {
        for ( quint32 column = 0; column < 3; ++column )
        {
            if ( column % 2 == 0 && row % 2 == 0 )
            {
                createViewportContainer(row, column);
            }
            else
            {
                createDivider(row, column);
            }

            gridLayout->setRowStretch(row, row % 2 == 0 ? 1 : 0);
            gridLayout->setRowMinimumHeight(row, 1);

            gridLayout->setColumnStretch(column, column % 2 == 0 ? 1 : 0);
            gridLayout->setColumnMinimumWidth(column, 1);
        }
    }

    updateLayoutFromProportions();
}

void QuadViewportWidget::createViewportContainer(quint32 row, quint32 column)
{
    quint32 index = 0;

    if ( row == 0 )
    {
        index = column == 0 ? UpperLeft : UpperRight;
    }
    else
    {
        index = column == 0 ? LowerLeft : LowerRight;
    }

    QFrame* container = new QFrame();
    Q_ASSERT_X(!m_ViewportContainers.at(index), Q_FUNC_INFO, "Unexpected viewport overwrite!");
    m_ViewportContainers[index] = container;

    container->setFrameShadow(QFrame::Sunken);
    container->setFrameShape(QFrame::Panel);

    QVBoxLayout* containerLayout = new QVBoxLayout();
    container->setLayout(containerLayout);
    containerLayout->setMargin(0);
    containerLayout->setSpacing(0);

    // Stick in a placeholder for now.
    containerLayout->addWidget(createPlaceholderViewportWidget(index));

    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());
    gridLayout->addWidget(container, row, column);
}

void QuadViewportWidget::createDivider(quint32 row, quint32 column)
{
    quint32 type = 0;

    if ( row == 0 )
    {
        type = Divider::Top;
    }
    else if ( row == 2 )
    {
        type = Divider::Bottom;
    }
    else
    {
        if ( column == 0 )
        {
            type = Divider::Left;
        }
        else if ( column == 2 )
        {
            type = Divider::Right;
        }
        else
        {
            type = Divider::Middle;
        }
    }

    Divider* divider = new Divider(this, static_cast<Divider::DividerType>(type));
    Q_ASSERT_X(!m_Dividers.at(type), Q_FUNC_INFO, "Unexpected divider overwrite!");
    m_Dividers[type] = divider;

    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());
    gridLayout->addWidget(divider, row, column);
}

void QuadViewportWidget::dividerMoveEvent(Divider& divider, QMouseEvent* event)
{
    QPoint localPos = mapFromGlobal(event->globalPos());

    if ( divider.canDragHorizontally() )
    {
        float proportion = static_cast<float>(localPos.x()) / static_cast<float>(width() < 1 ? 1 : width());
        m_ViewProportions.setX(proportion);
    }

    if ( divider.canDragVertically() )
    {
        float proportion = static_cast<float>(localPos.y()) / static_cast<float>(height() < 1 ? 1 : height());
        m_ViewProportions.setY(proportion);
    }

    updateLayoutFromProportions();
}

void QuadViewportWidget::updateLayoutFromProportions()
{
    m_ViewProportions.setX(clamp(m_ViewProportions.x(), 0.01f, 0.99f));
    m_ViewProportions.setY(clamp(m_ViewProportions.y(), 0.01f, 0.99f));

    quint32 lowerHalfDividerWidth = Divider::DIVIDER_WIDTH / 2;
    quint32 upperHalfDividerWidth = Divider::DIVIDER_WIDTH - lowerHalfDividerWidth;

    qint32 lWidth = qMax<qint32>(static_cast<qint32>(m_ViewProportions.x() * width()) - lowerHalfDividerWidth, 1);
    qint32 uHeight = qMax<qint32>(static_cast<qint32>(m_ViewProportions.y() * height()) - lowerHalfDividerWidth, 1);
    qint32 rWidth = qMax<qint32>(width() - lWidth - upperHalfDividerWidth, 1);
    qint32 lHeight = qMax<qint32>(height() - uHeight - upperHalfDividerWidth, 1);

    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());

    gridLayout->setColumnStretch(0, lWidth);
    gridLayout->setColumnStretch(1, Divider::DIVIDER_WIDTH);
    gridLayout->setColumnStretch(2, rWidth);

    gridLayout->setRowStretch(0, uHeight);
    gridLayout->setRowStretch(1, Divider::DIVIDER_WIDTH);
    gridLayout->setRowStretch(2, lHeight);
}

QWidget* QuadViewportWidget::createPlaceholderViewportWidget(quint32 index)
{
    OSGViewWidget* viewport =  new OSGViewWidget();
    viewport->setRootNode(ApplicationModel::singleton()->sampleSceneGraph());
    osg::Camera* camera = viewport->camera();

    if ( index != UpperLeft )
    {
        camera->setProjectionMatrixAsOrtho2D(-1, 1, -1, 1);
        viewport->setUsingTrackballManipulator(false);
    }

    switch ( index )
    {
        case UpperRight:
        {
            // Top down
            camera->setViewMatrixAsLookAt(osg::Vec3d(0, 0, 1024), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));
            break;
        }

        case LowerLeft:
        {
            // On +X looking back
            camera->setViewMatrixAsLookAt(osg::Vec3d(1024, 0, 0), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 0, 1));
            break;
        }

        case LowerRight:
        {
            // On -Y looking forward
            camera->setViewMatrixAsLookAt(osg::Vec3d(0, -1024, 0), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 0, 1));
            break;
        }

        default:
        {
            break;
        }
    }

    return viewport;
}
