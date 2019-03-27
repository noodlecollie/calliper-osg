#include "orthographiccameracontroller.h"

#include <QtMath>
#include <QtDebug>

#include "Math/callipermath.h"

namespace
{
    // TODO: Actually define world bounds somewhere - probably in the application model.
    constexpr float WORLD_MAX_ABS_COORD = 16384.0f;

    struct IViewModeData
    {
        virtual ~IViewModeData() {}

        virtual const osg::Vec3d& east() const = 0;
        virtual const osg::Vec3d& north() const = 0;
        virtual const osg::Vec3d& viewAxis() const = 0;
        virtual osg::Vec3d centrePositionInWorldSpace(const osg::Vec2d& translation) const = 0;
        virtual osg::Quat rotationForViewAxis() const = 0;
        virtual const osg::Matrixd& rotationMatrixForViewAxis() const = 0;
        virtual osg::Vec2d translationFromWorldEyePosition(const osg::Vec3d& pos) const = 0;

        inline osg::Vec3d cameraPositionInWorldSpace(const osg::Vec2d& translation) const
        {
            return (viewAxis() * -WORLD_MAX_ABS_COORD) +
                   centrePositionInWorldSpace(translation);
        }
    };

    struct ViewModeData_Front : public IViewModeData
    {
        virtual ~ViewModeData_Front() {}

        const osg::Vec3d& east() const override
        {
            static const osg::Vec3d vec(0,1,0); return vec;
        }

        const osg::Vec3d& north() const override
        {
            static const osg::Vec3d vec(0,0,1); return vec;
        }

        const osg::Vec3d& viewAxis() const override
        {
            static const osg::Vec3d vec(-1,0,0); return vec;
        }

        osg::Vec3d centrePositionInWorldSpace(const osg::Vec2d &translation) const override
        {
            return osg::Vec3d(0, translation[0], translation[1]);
        }

        osg::Quat rotationForViewAxis() const override
        {
            return osg::Quat(qDegreesToRadians(90.0f), osg::Vec3d(0,0,1));
        }

        const osg::Matrixd& rotationMatrixForViewAxis() const override
        {
            static const osg::Matrixd mat = osg::Matrix::rotate(rotationForViewAxis());
            return mat;
        }

        osg::Vec2d translationFromWorldEyePosition(const osg::Vec3d &pos) const override
        {
            return osg::Vec2d(pos[1], pos[2]);
        }
    };

    struct ViewModeData_Right : public IViewModeData
    {
        virtual ~ViewModeData_Right() {}

        const osg::Vec3d& east() const override
        {
            static const osg::Vec3d vec(1,0,0); return vec;
        }

        const osg::Vec3d& north() const override
        {
            static const osg::Vec3d vec(0,0,1); return vec;
        }

        const osg::Vec3d& viewAxis() const override
        {
            static const osg::Vec3d vec(0,1,0); return vec;
        }

        osg::Vec3d centrePositionInWorldSpace(const osg::Vec2d &translation) const override
        {
            return osg::Vec3d(translation[0], 0, translation[1]);
        }

        osg::Quat rotationForViewAxis() const override
        {
            return osg::Quat(); // Identity!
        }

        const osg::Matrixd& rotationMatrixForViewAxis() const override
        {
            static const osg::Matrixd mat;  // Identity!
            return mat;
        }

        osg::Vec2d translationFromWorldEyePosition(const osg::Vec3d &pos) const override
        {
            return osg::Vec2d(pos[0], pos[2]);
        }
    };

    struct ViewModeData_Top : public IViewModeData
    {
        virtual ~ViewModeData_Top() {}

        const osg::Vec3d& east() const override
        {
            static const osg::Vec3d vec(1,0,0); return vec;
        }

        const osg::Vec3d& north() const override
        {
            static const osg::Vec3d vec(0,1,0); return vec;
        }

        const osg::Vec3d& viewAxis() const override
        {
            static const osg::Vec3d vec(0,0,-1); return vec;
        }

        osg::Vec3d centrePositionInWorldSpace(const osg::Vec2d &translation) const override
        {
            return osg::Vec3d(translation[0], translation[1], 0);
        }

        osg::Quat rotationForViewAxis() const override
        {
            return osg::Quat(qDegreesToRadians(-90.0f), osg::Vec3d(1,0,0));
        }

        const osg::Matrixd& rotationMatrixForViewAxis() const override
        {
            static const osg::Matrixd mat = osg::Matrix::rotate(rotationForViewAxis());
            return mat;
        }

        osg::Vec2d translationFromWorldEyePosition(const osg::Vec3d &pos) const override
        {
            return osg::Vec2d(pos[0], pos[1]);
        }
    };

    const IViewModeData& viewModeData(OrthographicCameraController::ViewMode mode)
    {
        switch ( mode )
        {
            case OrthographicCameraController::ViewMode::Front:
            {
                static const ViewModeData_Front data;
                return data;
            }

            case OrthographicCameraController::ViewMode::Right:
            {
                static const ViewModeData_Right data;
                return data;
            }

            default:
            {
                static const ViewModeData_Top data;
                return data;
            }
        }
    }
}

OrthographicCameraController::OrthographicCameraController()
    : osgGA::StandardManipulator(0), // No flags - we want full control
      m_Zoom(1.0f),
      m_ViewMode(ViewMode::Top),
      m_Translation(),
      m_Signals(new OrthographicCameraControllerSignals())
{
}

OrthographicCameraController::OrthographicCameraController(const OrthographicCameraController &other,
                                                           const osg::CopyOp &copyOp)
    : osg::Object(other, copyOp),
      osg::Callback(other, copyOp),
      osgGA::StandardManipulator(other, copyOp),
      m_Zoom(other.m_Zoom),
      m_ViewMode(other.m_ViewMode),
      m_Translation(other.m_Translation),
      m_Signals(new OrthographicCameraControllerSignals())
{
}

float OrthographicCameraController::zoom() const
{
    return m_Zoom;
}

void OrthographicCameraController::setZoom(float val)
{
    if ( val < ZOOM_MIN )
    {
        val = ZOOM_MIN;
    }
    else if ( val > ZOOM_MAX )
    {
        val = ZOOM_MAX;
    }

    if ( val == m_Zoom )
    {
        return;
    }

    m_Zoom = val;
    m_Signals->notifyProjectionUpdated();
}

osg::Vec2d OrthographicCameraController::translation() const
{
    return m_Translation;
}

void OrthographicCameraController::setTranslation(const osg::Vec2d& vec)
{
    if ( m_Translation == vec )
    {
        return;
    }

    m_Translation = vec;
    m_Signals->notifyUpdated();
}

OrthographicCameraController::ViewMode OrthographicCameraController::viewMode() const
{
    return m_ViewMode;
}

void OrthographicCameraController::setViewMode(ViewMode mode)
{
    if ( mode == m_ViewMode )
    {
        return;
    }

    m_ViewMode = mode;
    m_Signals->notifyUpdated();
}

OrthographicCameraControllerSignals* OrthographicCameraController::signalAdapter()
{
    return m_Signals.data();
}

const OrthographicCameraControllerSignals* OrthographicCameraController::signalAdapter() const
{
    return m_Signals.data();
}

void OrthographicCameraController::init(const osgGA::GUIEventAdapter &ea,
                                        osgGA::GUIActionAdapter &us)
{
    osgGA::StandardManipulator::init(ea, us);
}

void OrthographicCameraController::setByMatrix(const osg::Matrixd &matrix)
{
    // Ignored for now unless we really need it.
    Q_UNUSED(matrix);
}

void OrthographicCameraController::setByInverseMatrix(const osg::Matrixd &matrix)
{
    // Ignored for now unless we really need it.
    Q_UNUSED(matrix);
}

osg::Matrixd OrthographicCameraController::getMatrix() const
{
    // The rules are as follows:
    // getInverseMatrix() should return the actual world-to-camera matrix that gets set on the camera.
    // Therefore we can calculate camera-centric motion in getMatrix() and pass the inverse through
    // getInverseMatrix().
    // Since the camera is already in right-handed Y-up co-ordinates (ie. Z pointing towards the viewer),
    // the very first transformation done on the camera should be the conversion from Y-up to Z-up.
    // The rest of the transformations on the camera can then all be done in Z-up space.

    // The actual world-space transformations we need to do to the camera are as follows:
    // - Rotate the camera to look down the target axis.
    // - Pull the camera back up this axis until it is flush with the edge of the world.
    //   (The Z planes set on the projection should encompass the entire world.)
    // - Translate the camera in the non-view axes.

    const IViewModeData& vmData = viewModeData(m_ViewMode);
    return CalliperMath::matRotation(CalliperMath::CardinalAxis::X, 90) *
           osg::Matrixd::translate(osg::Vec3d(m_Translation[0], -WORLD_MAX_ABS_COORD, m_Translation[1])) *
           vmData.rotationMatrixForViewAxis();
}

osg::Matrixd OrthographicCameraController::getInverseMatrix() const
{
    return osg::Matrixd::inverse(getMatrix());
}

void OrthographicCameraController::setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation)
{
    Q_UNUSED(rotation);

    m_Translation = viewModeData(m_ViewMode).translationFromWorldEyePosition(eye);
}

void OrthographicCameraController::setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{
    Q_UNUSED(center);
    Q_UNUSED(up);

    m_Translation = viewModeData(m_ViewMode).translationFromWorldEyePosition(eye);
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Quat &rotation) const
{
    const IViewModeData& vmData = viewModeData(m_ViewMode);
    eye = vmData.cameraPositionInWorldSpace(m_Translation);
    rotation = vmData.rotationForViewAxis();
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Vec3d &center, osg::Vec3d &up) const
{
    const IViewModeData& vmData = viewModeData(m_ViewMode);
    eye = vmData.cameraPositionInWorldSpace(m_Translation);
    center = vmData.centrePositionInWorldSpace(m_Translation);
    up = vmData.north();
}
