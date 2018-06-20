#include "applicationmodel.h"

#include "OSG/pushwarnings.h"
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/Geode>
#include "OSG/popwarnings.h"

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
    osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3( 0.f, 0.f, 0.f ), 0.25f, 0.5f);
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(cylinder);
    sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));

    // I'll probably get used to this soon, but given this is a placeholder scene,
    // it's probably worth mentioning. OSG's ref_ptr just holds a pointer to a
    // ref-counted object. It's the object itself that holds the ref count, not
    // the pointer. This means we can pass raw pointers around to new ref_ptrs and
    // the ref count won't get out of sync, because it exists only in one place
    // (on the pointed-to object).
    osg::Geode* geode = new osg::Geode();
    m_SampleSceneGraph = geode;
    geode->addDrawable(sd);

    osg::StateSet* stateSet = geode->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
}
