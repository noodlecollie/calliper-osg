#include "orthographiccameracontroller.h"

#include <QtMath>
#include <QtDebug>

#include "OSG/osgdefs.h"

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
        virtual osg::Vec3d cameraPositionInWorldSpace(const osg::Vec2d& translation) const = 0;

        // Pseudo-camera space is left-handed Z-up, where the camera is at 0 and looking down +Y.
        // This then gets switched to OpenGL camera space (right-handed Y-up) later.
        virtual const osg::Matrixd& viewSpaceToPseudoCameraSpace() const = 0;
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

        osg::Vec3d cameraPositionInWorldSpace(const osg::Vec2d &translation) const override
        {
            return (viewAxis() * -WORLD_MAX_ABS_COORD) +
                   osg::Vec3d(0, translation[0], translation[1]);
        }

        const osg::Matrixd& viewSpaceToPseudoCameraSpace() const override
        {
            static const osg::Matrixd mat = osg::Matrix::rotate(osg::Quat(qDegreesToRadians(-90.0f), osg::Vec3d(0,0,1)));
            return mat;
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

        osg::Vec3d cameraPositionInWorldSpace(const osg::Vec2d &translation) const override
        {
            return (viewAxis() * -WORLD_MAX_ABS_COORD) +
                   osg::Vec3d(translation[0], 0, translation[1]);
        }

        const osg::Matrixd& viewSpaceToPseudoCameraSpace() const override
        {
            static const osg::Matrixd mat;  // Identity!
            return mat;
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

        osg::Vec3d cameraPositionInWorldSpace(const osg::Vec2d &translation) const override
        {
            return (viewAxis() * -WORLD_MAX_ABS_COORD) +
                   osg::Vec3d(translation[0], translation[1], 0);
        }

        const osg::Matrixd& viewSpaceToPseudoCameraSpace() const override
        {
            static const osg::Matrixd mat = osg::Matrix::rotate(osg::Quat(qDegreesToRadians(90.0f), osg::Vec3d(1,0,0)));
            return mat;
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
      m_ViewportSize(1,1),
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
      m_ViewportSize(other.m_ViewportSize),
      m_Signals(new OrthographicCameraControllerSignals())
{
}

QSize OrthographicCameraController::viewportSize() const
{
    return m_ViewportSize;
}

void OrthographicCameraController::setViewportSize(const QSize &size)
{
    m_ViewportSize = size;

    if ( m_ViewportSize.width() < 1 )
    {
        m_ViewportSize.setWidth(1);
    }

    if ( m_ViewportSize.height() < 1 )
    {
        m_ViewportSize.setHeight(1);
    }
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
    m_Signals->notifyUpdated();
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
    // TODO
}

void OrthographicCameraController::setByInverseMatrix(const osg::Matrixd &matrix)
{
    // TODO
}

osg::Matrixd OrthographicCameraController::getMatrix() const
{
    // TODO
    return osg::Matrixd();
}

osg::Matrixd OrthographicCameraController::getInverseMatrix() const
{
    // TODO
    return osg::Matrixd();
}

void OrthographicCameraController::setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation)
{
    // TODO
}

void OrthographicCameraController::setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{
    // TODO
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Quat &rotation) const
{
    // TODO
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Vec3d &center, osg::Vec3d &up) const
{
    // TODO
}
