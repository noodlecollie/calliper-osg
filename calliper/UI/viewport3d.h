#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include "UI/osgviewwidget.h"

#include "OSG/pushwarnings.h"
#include <osg/Camera>
#include <osgGA/TrackballManipulator>
#include "OSG/popwarnings.h"

class Viewport3D : public OSGViewWidget
{
    Q_OBJECT
public:
    explicit Viewport3D(QWidget *parent = nullptr);
    virtual ~Viewport3D();

protected:
    virtual void resizeGL(int newWidth, int newHeight) override;

private:
    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<osgGA::TrackballManipulator> m_TrackballManipulator;
};

#endif // VIEWPORT3D_H
