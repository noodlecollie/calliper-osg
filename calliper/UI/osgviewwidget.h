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

    osg::Node* rootNode() const;
    void setRootNode(osg::Node* node);

protected:
    osgGA::EventQueue* getEventQueue() const;

    virtual void paintGL() override;
    virtual void resizeGL(int newWidth, int newHeight) override;
    virtual void initializeGL() override;

    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_GraphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> m_Viewer;
    qreal m_Scale;
};

#endif // OSGVIEWWIDGET_H
