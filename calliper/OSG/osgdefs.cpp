#include "OSG/osgdefs.h"

namespace OSGDefs
{
    const osg::Matrixd LEFT_ZUP_TO_RIGHT_YUP(
            1,  0, 0, 0,
            0,  0, 1, 0,
            0, -1, 0, 0,
            0,  0, 0, 1 );

    const osg::Matrixd RIGHT_YUP_TO_LEFT_ZUP(
            1, 0,  0, 0,
            0, 0, -1, 0,
            0, 1,  0, 0,
            0, 0,  0, 1 );

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
