#include "applicationmodel.h"

#include "OSG/pushwarnings.h"
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include "OSG/popwarnings.h"

#include <QtMath>
#include "Core/cvector.h"

ApplicationModel::ApplicationModel()
{
    setUpSampleScene();
}

osg::ref_ptr<osg::Node> ApplicationModel::sampleSceneGraph() const
{
    return m_SampleSceneGraph;
}

void ApplicationModel::setUpSampleScene()
{
    osg::Group* root = new osg::Group;
    m_SampleSceneGraph = root;

    osg::Sphere* sphereOrigin = new osg::Sphere(CVector3D(0,0,0), 3);
    osg::ShapeDrawable* sdOrigin = new osg::ShapeDrawable(sphereOrigin);
    sdOrigin->setColor(CVector4D(1,1,1,1));
    osg::Geode* geodeOrigin = new osg::Geode();
    geodeOrigin->addDrawable(sdOrigin);

    osg::StateSet* stateSetOrigin = geodeOrigin->getOrCreateStateSet();
    osg::Material* materialOrigin = new osg::Material;
    materialOrigin->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetOrigin->setAttributeAndModes(materialOrigin, osg::StateAttribute::ON);
    stateSetOrigin->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Cone* coneX = new osg::Cone(CVector3D(0,0,0), 3, 8);
    osg::ShapeDrawable* sdX = new osg::ShapeDrawable(coneX);
    sdX->setColor(CVector4D(1,0,0,1));
    osg::Geode* geodeX = new osg::Geode();
    geodeX->addDrawable(sdX);

    osg::StateSet* stateSetX = geodeX->getOrCreateStateSet();
    osg::Material* materialX = new osg::Material;
    materialX->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetX->setAttributeAndModes(materialX, osg::StateAttribute::ON);
    stateSetX->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Cone* coneY = new osg::Cone(CVector3D(0,0,0), 3, 8);
    osg::ShapeDrawable* sdY = new osg::ShapeDrawable(coneY);
    sdY->setColor(CVector4D(0,1,0,1));
    osg::Geode* geodeY = new osg::Geode();
    geodeY->addDrawable(sdY);

    osg::StateSet* stateSetY = geodeY->getOrCreateStateSet();
    osg::Material* materialY = new osg::Material;
    materialY->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetY->setAttributeAndModes(materialY, osg::StateAttribute::ON);
    stateSetY->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Cone* coneZ = new osg::Cone(CVector3D(0,0,0), 3, 8);
    osg::ShapeDrawable* sdZ = new osg::ShapeDrawable(coneZ);
    sdZ->setColor(CVector4D(0,0,1,1));
    osg::Geode* geodeZ = new osg::Geode();
    geodeZ->addDrawable(sdZ);

    osg::StateSet* stateSetZ = geodeZ->getOrCreateStateSet();
    osg::Material* materialZ = new osg::Material;
    materialZ->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetZ->setAttributeAndModes(materialZ, osg::StateAttribute::ON);
    stateSetZ->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::PositionAttitudeTransform* transformX = new osg::PositionAttitudeTransform();
    transformX->setPosition(CVector3D(10,0,0).asDouble());
    transformX->setAttitude(osg::Quat(qDegreesToRadians(90.0f), CVector3D(0,1,0)));
    transformX->addChild(geodeX);

    osg::PositionAttitudeTransform* transformY = new osg::PositionAttitudeTransform();
    transformY->setPosition(CVector3D(0,10,0).asDouble());
    transformY->setAttitude(osg::Quat(qDegreesToRadians(-90.0f), CVector3D(1,0,0)));
    transformY->addChild(geodeY);

    osg::PositionAttitudeTransform* transformZ = new osg::PositionAttitudeTransform();
    transformZ->setPosition(CVector3D(0,0,10).asDouble());
    transformZ->addChild(geodeZ);

    root->addChild(geodeOrigin);
    root->addChild(transformX);
    root->addChild(transformY);
    root->addChild(transformZ);
}
