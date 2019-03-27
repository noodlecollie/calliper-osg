#include "firstpersoncameracontroller.h"

#include <QtMath>
#include <QtDebug>
#include "OSG/osgdefs.h"
#include "Math/callipermath.h"
#include "Math/eulerangles.h"

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

void FirstPersonCameraController::clearAllMovement()
{
    m_MovementFlags = 0;
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
    // If called with an invalid time delta, just notify of the update but
    // don't bother redundantly calculating any velocity.
    if ( timeDeltaSec > 0.0f )
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
    }

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
    // The rules are as follows:
    // getInverseMatrix() should return the actual world-to-camera matrix that gets set on the camera.
    // Therefore we can calculate camera-centric motion in getMatrix() and pass the inverse through
    // getInverseMatrix().
    // Since the camera is already in right-handed Y-up co-ordinates (ie. Z pointing towards the viewer),
    // the very first transformation done on the camera should be the conversion from Y-up to Z-up.
    // The rest of the transformations on the camera can then all be done in Z-up space.

    osg::Matrixd mat;

    // 1. Rotate the camera on X so that +Y points forward and +Z points up.
    // This means rotating the camera clockwise.
    mat *= CalliperMath::matRotation(CalliperMath::CardinalAxis::X, 90);

    // 2. Rotate the camera on Z so that +X points forward.
    mat *= CalliperMath::matRotation(CalliperMath::CardinalAxis::Z, -90);

    // 3. Rotate the camera according to the Euler angles.
    mat *= EulerAngles::matForAngles(m_Pitch, m_Yaw, 0);

    // 4. Translate the camera to its position in the world.
    mat *= osg::Matrixd::translate(m_Position);

    return mat;
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
    rotation = EulerAngles::quatForAngles(m_Pitch, m_Yaw, 0);
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
    osg::Matrixd transform = EulerAngles::matForAngles(m_Pitch, m_Yaw, 0);

    // Today I learned: OSG matrices are row-major, which means they must post-multiply vertices.

    if ( forward )
    {
        *forward = osg::Vec3d(1,0,0) * transform;
    }

    if ( left )
    {
        *left = osg::Vec3d(0,1,0) * transform;
    }
}

void FirstPersonCameraController::setPitchAndYawFromDir(const osg::Vec3d& dir)
{
    osg::Vec3d angles = EulerAngles::directionToEulerAnglesSimple(dir);
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
