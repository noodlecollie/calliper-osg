#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "UI/osgviewwidget.h"
#include "UI/orthographiccameracontroller.h"

class Viewport2D : public OSGViewWidget
{
    Q_OBJECT
public:
    explicit Viewport2D(QWidget *parent = nullptr);
    virtual ~Viewport2D();

protected:
    virtual void resizeGL(int newWidth, int newHeight) override;

private:
    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<OrthographicCameraController> m_OrthoController;
};

#endif // VIEWPORT2D_H
