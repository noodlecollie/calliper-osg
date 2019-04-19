#include "applicationmodel.h"

#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include "osg/testplane.h"

#include <QtMath>

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
    m_SampleSceneGraph = osg::ref_ptr<osg::Group>(new osg::Group());
    m_SampleSceneGraph->addChild(createAxisMarker());
}

osg::ref_ptr<osg::Group> ApplicationModel::createAxisMarker()
{
    osg::ref_ptr<osg::Group> root(new osg::Group());

    osg::Sphere* sphereOrigin = new osg::Sphere(osg::Vec3(0,0,0), 3);
    osg::ShapeDrawable* sdOrigin = new osg::ShapeDrawable(sphereOrigin);
    sdOrigin->setColor(osg::Vec4(1,1,1,1));
    osg::Geode* geodeOrigin = new osg::Geode();
    geodeOrigin->addDrawable(sdOrigin);

    osg::StateSet* stateSetOrigin = geodeOrigin->getOrCreateStateSet();
    osg::Material* materialOrigin = new osg::Material;
    materialOrigin->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetOrigin->setAttributeAndModes(materialOrigin, osg::StateAttribute::ON);
    stateSetOrigin->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Cone* coneX = new osg::Cone(osg::Vec3(0,0,0), 3, 8);
    osg::ShapeDrawable* sdX = new osg::ShapeDrawable(coneX);
    sdX->setColor(osg::Vec4(1,0,0,1));
    osg::Geode* geodeX = new osg::Geode();
    geodeX->addDrawable(sdX);

    osg::StateSet* stateSetX = geodeX->getOrCreateStateSet();
    osg::Material* materialX = new osg::Material;
    materialX->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetX->setAttributeAndModes(materialX, osg::StateAttribute::ON);
    stateSetX->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Cone* coneY = new osg::Cone(osg::Vec3(0,0,0), 3, 8);
    osg::ShapeDrawable* sdY = new osg::ShapeDrawable(coneY);
    sdY->setColor(osg::Vec4(0,1,0,1));
    osg::Geode* geodeY = new osg::Geode();
    geodeY->addDrawable(sdY);

    osg::StateSet* stateSetY = geodeY->getOrCreateStateSet();
    osg::Material* materialY = new osg::Material;
    materialY->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetY->setAttributeAndModes(materialY, osg::StateAttribute::ON);
    stateSetY->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Cone* coneZ = new osg::Cone(osg::Vec3(0,0,0), 3, 8);
    osg::ShapeDrawable* sdZ = new osg::ShapeDrawable(coneZ);
    sdZ->setColor(osg::Vec4(0,0,1,1));
    osg::Geode* geodeZ = new osg::Geode();
    geodeZ->addDrawable(sdZ);

    osg::StateSet* stateSetZ = geodeZ->getOrCreateStateSet();
    osg::Material* materialZ = new osg::Material;
    materialZ->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSetZ->setAttributeAndModes(materialZ, osg::StateAttribute::ON);
    stateSetZ->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::PositionAttitudeTransform* transformX = new osg::PositionAttitudeTransform();
    transformX->setPosition(osg::Vec3d(10,0,0));
    transformX->setAttitude(osg::Quat(qDegreesToRadians(90.0f), osg::Vec3(0,1,0)));
    transformX->addChild(geodeX);

    osg::PositionAttitudeTransform* transformY = new osg::PositionAttitudeTransform();
    transformY->setPosition(osg::Vec3d(0,10,0));
    transformY->setAttitude(osg::Quat(qDegreesToRadians(-90.0f), osg::Vec3(1,0,0)));
    transformY->addChild(geodeY);

    osg::PositionAttitudeTransform* transformZ = new osg::PositionAttitudeTransform();
    transformZ->setPosition(osg::Vec3d(0,0,10));
    transformZ->addChild(geodeZ);

    root->addChild(geodeOrigin);
    root->addChild(transformX);
    root->addChild(transformY);
    root->addChild(transformZ);

    return root;
}
