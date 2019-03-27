#ifndef EULERANGLES_H
#define EULERANGLES_H

#include <osg/Matrixd>
#include <osg/Quat>
#include <osg/Vec3d>

namespace EulerAngles
{
    // Rules:
    // An orientation of (0,0,0) means that an object/camera/etc. is facing forward down +X,
    //   with +Y pointing left and +Z pointing up. This is called the "default" orientation.
    // Pitch, yaw and roll are in degrees.
    // A pitch of +90 looks up along +Z; a pitch of -90 looks down along -Z.
    //   Therefore pitch is applied as a right-handed rotation around -Y.
    //   Positive values rotate anticlockwise if you are looking side-on (ie. down +Y).
    // A yaw of +90 looks along +Y; a yaw of -90 looks along -Y.
    //   Therefore yaw is applied as a right-handed rotation around +Z.
    //   Positive values rotate anticlockwise if you are looking top-down (ie. down -Z).
    // A roll of +90 rotates the object so that the up direction points down -Y.
    //   A roll of -90 rotates the object so that the up direction points down +Y.
    //   Therefore roll is applied as a right-handed rotation around +X.
    //   Positive values rotate anticlockwise if you are looking front-on (ie. down -X).
    // Roll is applied first, then pitch and then yaw.

    // Roll is left at 0.
    osg::Vec3d directionToEulerAnglesSimple(const osg::Vec3d& dir);

    // Returns a rotation that should be applied to an object in the
    // default orientation in order to orient it according to the
    // provided pitch, yaw and roll values.
    osg::Matrixd matForAngles(float pitch, float yaw, float roll);
    osg::Quat quatForAngles(float pitch, float yaw, float roll);
}

#endif // EULERANGLES_H
