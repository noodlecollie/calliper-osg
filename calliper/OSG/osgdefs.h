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

    extern const osg::Matrixd LEFT_ZUP_TO_RIGHT_YUP;
    extern const osg::Matrixd RIGHT_YUP_TO_LEFT_ZUP;

    MouseButton QtToOSGMouseButton(Qt::MouseButton button);
    Qt::MouseButton OSGToQtMouseButton(MouseButton button);
}

#endif // OSGDEFS_H
