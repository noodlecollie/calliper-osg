#include "callipermath.h"

#include <QtMath>

namespace CalliperMath
{
    osg::Quat quatRotation(CardinalAxis axis, double deg, bool invertAxis)
    {
        osg::Vec3d axisVec;
        axisVec[static_cast<quint32>(axis)] = invertAxis ? -1 : 1;

        return osg::Quat(qDegreesToRadians(deg), axisVec);
    }

    osg::Matrixd matRotation(CardinalAxis axis, double deg, bool invertAxis)
    {
        return osg::Matrixd::rotate(quatRotation(axis, deg, invertAxis));
    }
}
