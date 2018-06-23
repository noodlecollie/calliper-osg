#include "orthographiccameracontroller.h"

OrthographicCameraController::OrthographicCameraController()
    : osgGA::StandardManipulator(0) // No flags - we want full control
{
}

OrthographicCameraController::OrthographicCameraController(const OrthographicCameraController &other,
                                                           const osg::CopyOp &copyOp)
    : osg::Object(other, copyOp),
      osg::Callback(other, copyOp),
      osgGA::StandardManipulator(other, copyOp),
      m_Zoom(1.0f),
      m_ViewMode(Top)
{
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

void OrthographicCameraController::setByInverseMatrix(const osg::Matrixd &matrix) override
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
    eye = osg::Vec3d();
    rotation = osg::Quat();
}

void OrthographicCameraController::getTransformation(osg::Vec3d &eye, osg::Vec3d &center, osg::Vec3d &up) const
{
    // TODO
    eye = osg::Vec3d();
    center = osg::Vec3d();
    up = osg::Vec3d();
}
