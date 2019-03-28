#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include <osgGA/StandardManipulator>

#include <QObject>
#include <QtGlobal>
#include <QSize>
#include <QScopedPointer>

#include "UI/orthographiccameracontrollersignals.h"

class OrthographicCameraController : public osgGA::StandardManipulator
{
    META_Object(osgGA, OrthographicCameraController)
public:
    static constexpr float ZOOM_MIN = 0.001f;
    static constexpr float ZOOM_MAX = 100.0f;

    static constexpr quint32 VIEWMODE_AXIS_NEGATIVE = (1 << 2);
    static constexpr quint32 VIEWMODE_AXIS_MASK = 0x3;
    enum class ViewMode
    {
        Front = 0 | VIEWMODE_AXIS_NEGATIVE, // Looking down -X
        Right = 1,                          // Looking down +Y
        Top = 2 | VIEWMODE_AXIS_NEGATIVE    // Looking down -Z
    };

    OrthographicCameraController();
    OrthographicCameraController(const OrthographicCameraController& other,
                                 const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

    // World units per pixel.
    // The smaller the zoom, the larger the object.
    float zoom() const;
    void setZoom(float val);

    osg::Vec2d translation() const;
    void setTranslation(const osg::Vec2d& vec);

    ViewMode viewMode() const;
    void setViewMode(ViewMode mode);

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

private:
    float m_Zoom;               // World units per pixel.
    ViewMode m_ViewMode;
    osg::Vec2d m_Translation;   // World units east and north
    QScopedPointer<OrthographicCameraControllerSignals> m_Signals;
};

#endif // ORTHOGRAPHICCAMERACONTROLLER_H
