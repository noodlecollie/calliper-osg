#ifndef FIRSTPERSONCAMERACONTROLLER_H
#define FIRSTPERSONCAMERACONTROLLER_H

#include <osgGA/StandardManipulator>

#include "UI/firstpersoncameracontrollersignals.h"

class FirstPersonCameraController : public osgGA::StandardManipulator
{
    META_Object(osgGA, FirstPersonCameraController)
public:
    FirstPersonCameraController();
    FirstPersonCameraController(const FirstPersonCameraController& other,
                                const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

    void setForward(bool active);
    void setBackward(bool active);
    void setLeft(bool active);
    void setRight(bool active);
    void setUp(bool active);
    void setDown(bool active);
    void clearAllMovement();

    float movementSpeed() const;
    void setMovementSpeed(float speed);

    float pitch() const;
    void setPitch(float value);

    float yaw() const;
    void setYaw(float value);

    FirstPersonCameraControllerSignals* signalAdapter();
    const FirstPersonCameraControllerSignals* signalAdapter() const;

    // Use current velocity/pitch/yaw to update position.
    void updateMovement(float timeDeltaSec);

    virtual void init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;

    // CameraManipulator implementation.
    virtual void setByMatrix(const osg::Matrixd &matrix) override;
    virtual void setByInverseMatrix(const osg::Matrixd &matrix) override;
    virtual osg::Matrixd getMatrix() const override;
    virtual osg::Matrixd getInverseMatrix() const override;

    // StandardManipulator implementation.
    // We're gonna have to make a decision as to what the rotation is defined against - ie. what direction
    // the camera should face if there is no rotation. OSG's cannoical camera orientation is facing down -Z
    // with +Y as up, but the rest of the functions in this class use the Euler angle default orientation,
    // which faces down +X. For consistency, I'm going to decree that we use the Euler angle convention.
    // This means that any rotations used in the functions below will represent rotations away from the
    // Euler angle default orientation.
    virtual void setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation) override;
    virtual void setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up) override;
    virtual void getTransformation(osg::Vec3d &eye, osg::Quat &rotation) const override;
    virtual void getTransformation(osg::Vec3d &eye, osg::Vec3d& center, osg::Vec3d& up) const override;

private:
    enum MovementFlag
    {
        Flag_Forward = (1<<0),
        Flag_Backward = (1<<1),
        Flag_Left = (1<<2),
        Flag_Right = (1<<3),
        Flag_Up = (1<<4),
        Flag_Down = (1<<5)
    };

    inline void setMovementFlag(quint16 flag, bool active)
    {
        if ( active )
        {
            m_MovementFlags |= flag;
        }
        else
        {
            m_MovementFlags &= ~flag;
        }
    }

    void updateVelocity();
    void vectorsFromAngles(osg::Vec3d* forward, osg::Vec3d* left) const;
    void setPitchAndYawFromDir(const osg::Vec3d& dir);

    osg::Vec3d m_Position;
    float m_Pitch;
    float m_Yaw;
    osg::Vec3d m_Velocity;  // Fwd, left, up
    quint16 m_MovementFlags;
    float m_Speed;
    QScopedPointer<FirstPersonCameraControllerSignals> m_Signals;
};

#endif // FIRSTPERSONCAMERACONTROLLER_H
