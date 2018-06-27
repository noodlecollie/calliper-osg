#ifndef OSGDEFS_H
#define OSGDEFS_H

#include <QMouseEvent>
#include <osg/Matrixd>

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

    MouseButton QtToOSGMouseButton(Qt::MouseButton button);
    Qt::MouseButton OSGToQtMouseButton(MouseButton button);
}

#endif // OSGDEFS_H
