#include "callipermath.h"

#include <QtMath>

namespace CalliperMath
{
    osg::Vec3d directionToEulerAnglesSimple(const osg::Vec3d& dir)
    {
        // TODO: Fuzzy comparisons?

        osg::Vec3d angles;

        if ( dir.length2() == 0.0 )
        {
            return angles;
        }

        double& pitch = angles[0];

        // If x and y are null, just set the pitch.
        if ( dir[0] == 0.0 && dir[1] == 0.0 )
        {
            pitch = dir[2] > 0 ? 90.0 : -90.0;
            return angles;
        }

        double& yaw = angles[1];

        // Yaw depends on the x and y co-ordinates.
        yaw = qRadiansToDegrees(qAtan2(dir[1], dir[0]));

        while ( yaw < 0.0f )
        {
            yaw += 360.0;
        }

        // Pitch is found by finding the angle between the xy projection of the vector
        // and the negative Z axis.
        const double temp = qSqrt((dir[0] * dir[0]) + (dir[1] * dir[1]));    // Length of projection onto xy plane
        pitch = qRadiansToDegrees(qAtan(-dir[2] / temp));                   // Angle between this and -z.

        while ( pitch <= -180.0 )
        {
            pitch += 360.0;
        }

        while ( pitch > 180.0 )
        {
            pitch -= 360.0;
        }

        return angles;
    }
}
