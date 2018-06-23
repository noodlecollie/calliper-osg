#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H

#include "OSG/pushwarnings.h"
#include <osgGA/StandardManipulator>
#include "OSG/popwarnings.h"

class OrthographicCameraController : public osgGA::StandardManipulator
{
    META_Object(osgGA, OrthographicCameraController)
public:
    static constexpr quint32 VIEWMODE_AXIS_NEGATIVE = (1 << 2);
    enum ViewMode
    {
        Back = 0,   // Looking down +X
        Right = 1,  // Looking down +Y
        Bottom = 2, // Looking down +Z
        Front = Back | VIEWMODE_AXIS_NEGATIVE,
        Left = Right | VIEWMODE_AXIS_NEGATIVE,
        Top = Bottom | VIEWMODE_AXIS_NEGATIVE
    };

    OrthographicCameraController();
    OrthographicCameraController(const OrthographicCameraController& other,
                                 const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

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
    float m_Zoom;   // Pixels per world unit
    ViewMode m_ViewMode;
};

#endif // ORTHOGRAPHICCAMERACONTROLLER_H
