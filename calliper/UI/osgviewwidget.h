#ifndef OSGVIEWWIDGET_H
#define OSGVIEWWIDGET_H

// Adapted from https://github.com/vicrucann/QtOSG-hello

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>

class OSGViewWidget : public QOpenGLWidget
{
public:
    explicit OSGViewWidget(QWidget* parent = 0);
    virtual ~OSGViewWidget();

protected:
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void initializeGL();

    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual bool event(QEvent* event);

private:
    osgGA::EventQueue* getEventQueue() const;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_GraphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> m_Viewer;
    qreal m_Scale;
};

#endif // OSGVIEWWIDGET_H
