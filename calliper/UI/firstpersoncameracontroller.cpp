#include "firstpersoncameracontroller.h"

#include <QtMath>
#include <QtDebug>
#include "OSG/osgdefs.h"
#include "Math/callipermath.h"

namespace
{
    constexpr float MAX_PITCH_DELTA = 89.0f;
}

FirstPersonCameraController::FirstPersonCameraController()
    : osgGA::StandardManipulator(0), // No flags - we want full control,
      m_Position(0,0,0),
      m_Pitch(0.0f),
      m_Yaw(0.0f),
      m_Velocity(0,0,0),
      m_MovementFlags(0),
      m_Speed(150.0f),
      m_Signals(new FirstPersonCameraControllerSignals())
{
}

FirstPersonCameraController::FirstPersonCameraController(const FirstPersonCameraController &other, const osg::CopyOp &copyOp)
    : osg::Object(other, copyOp),
      osg::Callback(other, copyOp),
      osgGA::StandardManipulator(other, copyOp),
      m_Position(other.m_Position),
      m_Pitch(other.m_Pitch),
      m_Yaw(other.m_Yaw),
      m_Velocity(other.m_Velocity),
      m_MovementFlags(other.m_MovementFlags)
{
}

void FirstPersonCameraController::setForward(bool active)
{
    setMovementFlag(Flag_Forward, active);
}

void FirstPersonCameraController::setBackward(bool active)
{
    setMovementFlag(Flag_Backward, active);
}

void FirstPersonCameraController::setLeft(bool active)
{
    setMovementFlag(Flag_Left, active);
}

void FirstPersonCameraController::setRight(bool active)
{
    setMovementFlag(Flag_Right, active);
}

void FirstPersonCameraController::setUp(bool active)
{
    setMovementFlag(Flag_Up, active);
}

void FirstPersonCameraController::setDown(bool active)
{
    setMovementFlag(Flag_Down, active);
}

float FirstPersonCameraController::movementSpeed() const
{
    return m_Speed;
}

void FirstPersonCameraController::setMovementSpeed(float speed)
{
    m_Speed = speed;
}

float FirstPersonCameraController::pitch() const
{
    return m_Pitch;
}

void FirstPersonCameraController::setPitch(float value)
{
    if ( value > MAX_PITCH_DELTA )
    {
        value = MAX_PITCH_DELTA;
    }
    else if ( value < -MAX_PITCH_DELTA )
    {
        value = -MAX_PITCH_DELTA;
    }

    if ( m_Pitch == value )
    {
        return;
    }

    m_Pitch = value;
    m_Signals->notifyUpdated();
}

float FirstPersonCameraController::yaw() const
{
    return m_Yaw;
}

void FirstPersonCameraController::setYaw(float value)
{
    while ( value < 0.0f )
    {
        value += 360.0f;
    }

    while ( value >= 360.0f )
    {
        value -= 360.0f;
    }

    if ( m_Yaw == value )
    {
        return;
    }

    m_Yaw = value;
    m_Signals->notifyUpdated();
}

FirstPersonCameraControllerSignals* FirstPersonCameraController::signalAdapter()
{
    return m_Signals.data();
}

const FirstPersonCameraControllerSignals* FirstPersonCameraController::signalAdapter() const
{
    return m_Signals.data();
}

void FirstPersonCameraController::updateMovement(float timeDeltaSec)
{
    osg::Vec3d forward;
    osg::Vec3d left;
    osg::Vec3d up(0,0,1);
    vectorsFromAngles(&forward, &left);

    updateVelocity();
    osg::Vec3d translation = (forward * m_Velocity[0]) +
                             (left * m_Velocity[1]) +
                             (up * m_Velocity[2]);

    m_Position += translation * timeDeltaSec;
    m_Signals->notifyUpdated();
}

void FirstPersonCameraController::init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    osgGA::StandardManipulator::init(ea, us);
}

void FirstPersonCameraController::setByMatrix(const osg::Matrixd &matrix)
{
    // Ignored unless we need this.
    Q_UNUSED(matrix);
}

void FirstPersonCameraController::setByInverseMatrix(const osg::Matrixd &matrix)
{
    // Ignored unless we need this.
    Q_UNUSED(matrix);
}

osg::Matrixd FirstPersonCameraController::getMatrix() const
{
    osg::Matrixd yawRot;
    osg::Matrixd pitchRot;
    rotationMatrices(&yawRot, &pitchRot);

    return osg::Matrixd::translate(m_Position) * yawRot * pitchRot;
}

osg::Matrixd FirstPersonCameraController::getInverseMatrix() const
{
    return osg::Matrixd::inverse(getMatrix());
}

void FirstPersonCameraController::setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation)
{
    m_Position = eye;
    setPitchAndYawFromDir(rotation * osg::Vec3d(1,0,0));
}

void FirstPersonCameraController::setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{
    Q_UNUSED(up);

    m_Position = eye;
    setPitchAndYawFromDir(center - eye);
}

void FirstPersonCameraController::getTransformation(osg::Vec3d &eye, osg::Quat &rotation) const
{
    osg::Quat yaw;
    osg::Quat pitch;
    rotationQuats(&yaw, &pitch);

    rotation = yaw * pitch;
    eye = m_Position;
}

void FirstPersonCameraController::getTransformation(osg::Vec3d &eye, osg::Vec3d& center, osg::Vec3d& up) const
{
    eye = m_Position;
    up = osg::Vec3d(0,0,1);

    osg::Vec3d forward;
    vectorsFromAngles(&forward, Q_NULLPTR);
    center = m_Position + forward;
}

void FirstPersonCameraController::updateVelocity()
{
    osg::Vec3i components;

    if ( m_MovementFlags & Flag_Forward )
    {
        components[0]++;
    }

    if ( m_MovementFlags & Flag_Backward )
    {
        components[0]--;
    }

    if ( m_MovementFlags & Flag_Left )
    {
        components[1]++;
    }

    if ( m_MovementFlags & Flag_Right )
    {
        components[1]--;
    }

    if ( m_MovementFlags & Flag_Up )
    {
        components[2]++;
    }

    if ( m_MovementFlags & Flag_Down )
    {
        components[2]--;
    }

    m_Velocity[0] = m_Speed * components[0];
    m_Velocity[1] = m_Speed * components[1];
    m_Velocity[2] = m_Speed * components[2];
}

void FirstPersonCameraController::vectorsFromAngles(osg::Vec3d* forward, osg::Vec3d* left) const
{
    osg::Matrixd yawRot;
    osg::Matrixd pitchRot;
    rotationMatrices(&yawRot, forward ? &pitchRot : Q_NULLPTR);

    if ( forward )
    {
        *forward = osg::Vec3d(1,0,0) * yawRot * pitchRot;
    }

    if ( left )
    {
        *left = osg::Vec3d(0,1,0) * yawRot;
    }
}

void FirstPersonCameraController::rotationMatrices(osg::Matrixd* yaw, osg::Matrixd* pitch) const
{
    osg::Quat qYaw;
    osg::Quat qPitch;
    rotationQuats(yaw ? &qYaw : Q_NULLPTR, pitch ? &qPitch : Q_NULLPTR);

    if ( yaw )
    {
        *yaw = osg::Matrixd::rotate(qYaw);
    }

    if ( pitch )
    {
        *pitch = osg::Matrixd::rotate(qPitch);
    }
}

void FirstPersonCameraController::rotationQuats(osg::Quat *yaw, osg::Quat *pitch) const
{
    // For reference:
    // - Yaw of 0 looks down +X.
    // - Yaw of 90 looks down -Y.
    // - Pitch of 0 looks down +X.
    // - Pitch of 90 looks down +Z.
    // In general: rotations (in Hammer space) while looking down an axis go anticlockwise.

    if ( yaw )
    {
        *yaw = osg::Quat(qDegreesToRadians(m_Yaw), osg::Vec3d(0,0,1));
    }

    if ( pitch )
    {
        *pitch = osg::Quat(qDegreesToRadians(m_Pitch), osg::Vec3d(0,1,0));
    }
}

void FirstPersonCameraController::setPitchAndYawFromDir(const osg::Vec3d& dir)
{
    osg::Vec3d angles = CalliperMath::directionToEulerAnglesSimple(dir);
    double& pitch = angles[0];
    double& yaw = angles[1];

    if ( pitch < -MAX_PITCH_DELTA )
    {
        pitch = -MAX_PITCH_DELTA;
    }
    else if ( pitch > MAX_PITCH_DELTA )
    {
        pitch = MAX_PITCH_DELTA;
    }

    if ( m_Pitch != pitch || m_Yaw != yaw )
    {
        m_Pitch = pitch;
        m_Yaw = yaw;
        m_Signals->notifyUpdated();
    }
}
