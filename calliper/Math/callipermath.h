#ifndef CALLIPERMATH_H
#define CALLIPERMATH_H

#include <osg/Vec3d>

namespace CalliperMath
{
    // Roll is left at 0.
    osg::Vec3d directionToEulerAnglesSimple(const osg::Vec3d& dir);
}

#endif // CALLIPERMATH_H
