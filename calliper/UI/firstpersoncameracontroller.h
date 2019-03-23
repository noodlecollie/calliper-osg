#ifndef FIRSTPERSONCAMERACONTROLLER_H
#define FIRSTPERSONCAMERACONTROLLER_H

#include "OSG/pushwarnings.h"
#include <osgGA/StandardManipulator>
#include "OSG/popwarnings.h"

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
    void rotationMatrices(osg::Matrixd* yaw, osg::Matrixd* pitch) const;
    void rotationQuats(osg::Quat* yaw, osg::Quat* pitch) const;
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
