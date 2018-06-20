#ifndef OSGDEFS_H
#define OSGDEFS_H

#include <QMouseEvent>

namespace OSGDefs
{
    // Why on earth OSG doesn't actually enum these I don't know.
    enum MouseButton
    {
        NoButton = 0,
        LeftButton = 1,
        MiddleButton = 2,
        RightButton = 3
    };

    MouseButton QtToOSGMouseButton(Qt::MouseButton button)
    {
        switch (button)
        {
            case Qt::LeftButton:
            {
                return LeftButton;
            }

            case Qt::MiddleButton:
            {
                return MiddleButton;
            }

            case Qt::RightButton:
            {
                return RightButton;
            }

            default:
            {
                return NoButton;
            }
        }
    }

    Qt::MouseButton OSGToQtMouseButton(MouseButton button)
    {
        switch (button)
        {
            case LeftButton:
            {
                return Qt::LeftButton;
            }

            case MiddleButton:
            {
                return Qt::MiddleButton;
            }

            case RightButton:
            {
                return Qt::RightButton;
            }

            default:
            {
                return Qt::NoButton;
            }
        }
    }
}

#endif // OSGDEFS_H
