#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include "UI/osgviewwidget.h"

#include "Settings/genericsetting.h"
#include "Settings/settingssubcategory.h"
#include "UI/firstpersoncameracontroller.h"

class Viewport3D : public OSGViewWidget
{
    Q_OBJECT
public:
    explicit Viewport3D(QWidget *parent = nullptr);
    virtual ~Viewport3D() override;

protected:
    virtual void resizeGL(int newWidth, int newHeight) override;

private slots:
    void updateCameraProjection();

private:
    osg::ref_ptr<osg::Camera> m_Camera;
    osg::ref_ptr<FirstPersonCameraController> m_FPController;
    float m_FOV;

    SettingsSubCategory* m_3DViewSubCat;
    GenericSetting* m_DefaultCameraFOVSetting;
    GenericSetting* m_FarPlaneSetting;
};

#endif // VIEWPORT3D_H
