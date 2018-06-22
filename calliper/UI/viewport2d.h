#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "UI/osgviewwidget.h"

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
};

#endif // VIEWPORT2D_H