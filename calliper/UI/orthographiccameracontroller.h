#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include "OSG/pushwarnings.h"
#include <osgGA/StandardManipulator>
#include "OSG/popwarnings.h"

#include <QObject>
#include <QtGlobal>
#include <QSize>
#include <QScopedPointer>

#include "UI/orthographiccameracontrollersignals.h"

class OrthographicCameraController : public osgGA::StandardManipulator
{
    META_Object(osgGA, OrthographicCameraController)
public:
    static constexpr quint32 VIEWMODE_AXIS_NEGATIVE = (1 << 2);
    static constexpr quint32 VIEWMODE_AXIS_MASK = 0x3;
    enum class ViewMode
    {
        Back = 0,   // Looking down +X
        Right = 1,  // Looking down +Y
        Bottom = 2, // Looking down +Z
        Front = ViewMode::Back | VIEWMODE_AXIS_NEGATIVE,
        Left = ViewMode::Right | VIEWMODE_AXIS_NEGATIVE,
        Top = ViewMode::Bottom | VIEWMODE_AXIS_NEGATIVE
    };

    OrthographicCameraController();
    OrthographicCameraController(const OrthographicCameraController& other,
                                 const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

    QSize viewportSize() const;
    void setViewportSize(const QSize& size);

    float zoom() const;
    void setZoom(float val);

    osg::Vec2d translation() const;
    void setTranslation(const osg::Vec2d& vec);

    OrthographicCameraControllerSignals* signalAdapter();
    const OrthographicCameraControllerSignals* signalAdapter() const;

    virtual void init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;

    // CameraManipulator implementation.
    // Translation and scale are used; rotation is ignored.
    // Matrix is assumed to correspond to the current view mode.
    virtual void setByMatrix(const osg::Matrixd &matrix) override;
    virtual void setByInverseMatrix(const osg::Matrixd &matrix) override;
    virtual osg::Matrixd getMatrix() const override;
    virtual osg::Matrixd getInverseMatrix() const override;

    // StandardManipulator implementation.
    // Only the eye position is used; other parameters are ignored.
    virtual void setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation) override;
    virtual void setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up) override;
    virtual void getTransformation(osg::Vec3d &eye, osg::Quat &rotation) const override;
    virtual void getTransformation(osg::Vec3d &eye, osg::Vec3d& center, osg::Vec3d& up) const override;

signals:
    void updated();

private:
    void setTranslationViewModeAware(const osg::Vec3d& translation);
    osg::Vec3d viewAxis() const;
    void eastAndNorthAxes(osg::Vec3d* east, osg::Vec3d* north) const;
    osg::Quat rotationForViewAxis() const;
    osg::Vec3d eyePosition() const;
    osg::Vec3d upAxis() const;

    float m_Zoom;               // Pixels per world unit
    ViewMode m_ViewMode;
    osg::Vec2d m_Translation;    // World units east and north
    QSize m_ViewportSize;
    QScopedPointer<OrthographicCameraControllerSignals> m_Signals;
};

#endif // ORTHOGRAPHICCAMERACONTROLLER_H
