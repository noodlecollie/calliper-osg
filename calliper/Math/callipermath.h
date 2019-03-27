#ifndef CALLIPERMATH_H
#define CALLIPERMATH_H

#include <osg/Vec3d>
#include <osg/Quat>
#include <osg/Matrixd>

namespace CalliperMath
{
    enum class CardinalAxis
    {
        X = 0,
        Y,
        Z
    };

    // Convenience helpers.
    // OSG uses a right-handed, Y-up co-ordinate system.
    // In a right-handed system, rotations around an axis
    // go clockwise if you are looking down that axis - ie.
    // the direction the ringers on your right hand go if
    // you point your thumb along the positive direction
    // of the axis. Note that to rotate a vector a
    // matrix must post-multiply it, ie. v * m.

    osg::Quat quatRotation(CardinalAxis axis, double deg, bool invertAxis = false);
    osg::Matrixd matRotation(CardinalAxis axis, double deg, bool invertAxis = false);
}

#endif // CALLIPERMATH_H
