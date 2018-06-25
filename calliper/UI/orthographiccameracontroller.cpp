#include "orthographiccameracontroller.h"

#include <QtMath>
#include <QtDebug>

namespace
{
    // TODO: Actually define world bounds somewhere - probably in the application model.
    // At the moment this is just 1 so that lighting works. Apparently the shading is
    // affected by the Z clip plane distance - maybe fog-related?
    constexpr float Z_CLIP_BOUND = 1.0f;
    constexpr float ZOOM_MIN = 0.001f;
    constexpr float ZOOM_MAX = 100.0f;
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
    setByInverseMatrix(osg::Matrixd::inverse(matrix));
}

void OrthographicCameraController::setByInverseMatrix(const osg::Matrixd &matrix)
{
    setTranslationViewModeAware(osg::Vec3d(matrix(0, 3), matrix(1, 3), matrix(2, 3)));

    // Scale corresponds to the height of the view (see getMatrix()), so we pull values out from the matrix that correspond to that axis.
    // As per https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix,
    // matrix component (1,1) is 2.0 / (top - bottom), so convert to top - bottom (which will be the resulting bounds given by the zoom)
    float zoom = 2.0f / matrix(1,1);

    // Value is now equivalent to height / zoom, and we want zoom.
    setZoom(static_cast<float>(m_ViewportSize.height()) / zoom);

    // We also need the translation components. These can be obtained from matrix elements (0,3) and (1,3).
    // Taking the original calculation of (1,3):

    // (-t-b)/(t-b) = (-(-hb + ty) - (hb + ty)) / ((-hb + ty) - (hb + ty))
    //              = ((hb - ty) - (hb + ty)) / ((-hb + ty) - (hb + ty))
    //              = (hb - ty - hb - ty) / (ty - hb - hb - ty)
    //              = -2ty / (-2ty -2hb)
    //              = -2ty / -2(ty + hb)
    //              = -2 / -2 * ty / ty + hb
    // (-t-b)/(t-b) = vy (for brevity) = ty / (ty + hb)
    // vy = ty / (ty + hb)
    // vy(ty) + vy(hb) = ty
    // vy(ty) - ty = -vy(hb)
    // ty(v - 1) = -vy(hb)
    // ty = -vy(hb) / (vy - 1)

    // Given in the alternative case hb is just modified by an extra coefficient for aspect ratio,
    // we can say (-r-l)/(r-l) = vx = tx / (tx + ar(hb))
    // vx = tx / (tx + ar(hb))
    // vx(tx) + vx(ar(hb)) = tx
    // vx(tx) - tx = -vx(ar(hb))
    // tx(vx - 1) = -vx(ar(hb))
    // tx = -vx(ar(hb)) / (vx - 1)

    // We can code this up as:
    const float aspectRatio = static_cast<float>(m_ViewportSize.width()) / static_cast<float>(m_ViewportSize.height());
    const float worldUnitBounds = static_cast<float>(m_ViewportSize.height()) / m_Zoom;
    const float halfWorldUnitBounds = worldUnitBounds / 2.0f;
    const float vx = matrix(0, 3);
    const float vy = matrix(1, 3);

    osg::Vec2d trans((-1.0f * vx * aspectRatio * halfWorldUnitBounds) / (vx - 1),
                     (-1.0f * vy * halfWorldUnitBounds) / (vy - 1));
    setTranslation(trans);
}

osg::Matrixd OrthographicCameraController::getMatrix() const
{
    return osg::Matrixd::inverse(getInverseMatrix());
}

osg::Matrixd OrthographicCameraController::getInverseMatrix() const
{
    const float worldUnitBounds = static_cast<float>(m_ViewportSize.height()) / m_Zoom;
    const float halfWorldUnitBounds = worldUnitBounds / 2.0f;

    const float left = -halfWorldUnitBounds + m_Translation[0];
    const float right = halfWorldUnitBounds + m_Translation[0];
    const float bottom = -halfWorldUnitBounds + m_Translation[1];
    const float top = halfWorldUnitBounds + m_Translation[1];
    const float near = -Z_CLIP_BOUND;
    const float far = Z_CLIP_BOUND;

    return osg::Matrixd::ortho(left, right, bottom, top, near, far);
}

void OrthographicCameraController::setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation)
{
    Q_UNUSED(rotation);
    setTranslationViewModeAware(eye);
}

void OrthographicCameraController::setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{
    Q_UNUSED(center);
    Q_UNUSED(up);
    setTranslationViewModeAware(eye);
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Quat &rotation) const
{
    eye = eyePosition();
    rotation = rotationForViewAxis();
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Vec3d &center, osg::Vec3d &up) const
{
    eye = eyePosition();
    center = eyePosition() + (viewAxis() * -2 * Z_CLIP_BOUND);
    up = upAxis();
}

void OrthographicCameraController::setTranslationViewModeAware(const osg::Vec3d &translation)
{
    const quint32 mode = static_cast<quint32>(m_ViewMode);
    const quint32 viewAxis = mode & VIEWMODE_AXIS_MASK;
    const bool viewNegative = mode & VIEWMODE_AXIS_NEGATIVE;
    osg::Vec2d trans;

    switch ( viewAxis )
    {
        case 0: // X
        {
            trans[0] = translation[1];
            trans[1] = translation[2];

            if ( !viewNegative )
            {
                trans[0] *= -1.0f;
            }

            break;
        }

        case 1: // Y
        {
            trans[0] = translation[0];
            trans[1] = translation[2];

            if ( viewNegative )
            {
                trans[0] *= -1.0f;
            }

            break;
        }

        default: // Z
        {
            trans[0] = translation[0];
            trans[1] = translation[1];

            if ( !viewNegative )
            {
                trans[1] *= 1.0f;
            }

            break;
        }
    }

    setTranslation(trans);
}

osg::Vec3d OrthographicCameraController::viewAxis() const
{
    const quint32 mode = static_cast<quint32>(m_ViewMode);
    const quint32 viewAxis = mode & VIEWMODE_AXIS_MASK;
    const bool viewNegative = mode & VIEWMODE_AXIS_NEGATIVE;

    osg::Vec3d vec;
    vec[viewAxis] = viewNegative ? -1.0f : 1.0f;
    return vec;
}

void OrthographicCameraController::eastAndNorthAxes(osg::Vec3d* east, osg::Vec3d* north) const
{
    const quint32 mode = static_cast<quint32>(m_ViewMode);
    const quint32 viewAxis = mode & VIEWMODE_AXIS_MASK;
    const bool viewNegative = mode & VIEWMODE_AXIS_NEGATIVE;

    switch ( viewAxis )
    {
        case 0: // X
        {
            if ( east )
            {
                *east = osg::Vec3d(0, viewNegative ? 1 : -1, 0);
            }

            if ( north )
            {
                *north = osg::Vec3d(0, 0, 1);
            }

            break;
        }

        case 1: // Y
        {
            if ( east )
            {
                *east = osg::Vec3d(viewNegative ? -1 : 1, 0, 0);
            }

            if ( north )
            {
                *north = osg::Vec3d(0, 0, 1);
            }

            break;
        }

        default: // Z
        {
            if ( east )
            {
                *east = osg::Vec3d(viewNegative ? 1 : -1, 0, 0);
            }

            if ( north )
            {
                *north = osg::Vec3d(0, 1, 0);
            }

            break;
        }
    }
}

// Assuming by default we're looking down +Y with +Z as up
osg::Quat OrthographicCameraController::rotationForViewAxis() const
{
    const quint32 mode = static_cast<quint32>(m_ViewMode);
    const quint32 viewAxis = mode & VIEWMODE_AXIS_MASK;
    const bool viewNegative = mode & VIEWMODE_AXIS_NEGATIVE;

    switch ( viewAxis )
    {
        case 0: // X
        {
            return viewNegative
                    ? osg::Quat(qDegreesToRadians(90.0f), osg::Vec3f(0, 0, 1))
                    : osg::Quat(qDegreesToRadians(-90.0f), osg::Vec3f(0, 0, 1));
        }

        case 1: // Y
        {
            return viewNegative
                    ? osg::Quat(qDegreesToRadians(180.0f), osg::Vec3f(0, 0, 1))
                    : osg::Quat();
        }

        default: // Z
        {
            return viewNegative
                    ? osg::Quat(qDegreesToRadians(-90.0f), osg::Vec3f(1, 0, 0))
                    : osg::Quat(qDegreesToRadians(180.0f), osg::Vec3f(0, 1, 0)) * osg::Quat(qDegreesToRadians(-90.0f), osg::Vec3f(1, 0, 0));
            break;
        }
    }
}

osg::Vec3d OrthographicCameraController::eyePosition() const
{
    osg::Vec3d east;
    osg::Vec3d north;
    eastAndNorthAxes(&east, &north);
    return (east * m_Translation[0]) +
           (north * m_Translation[1]) +
           (viewAxis() * -Z_CLIP_BOUND);
}

osg::Vec3d OrthographicCameraController::upAxis() const
{
    return (static_cast<quint32>(m_ViewMode) & VIEWMODE_AXIS_MASK) == 2
        ? osg::Vec3d(0, 1, 0)
        : osg::Vec3d(0, 0, 1);
}
