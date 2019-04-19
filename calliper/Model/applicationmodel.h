#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include "Core/globalsingleton.h"

#include <osg/ref_ptr>
#include <osg/Node>

class ApplicationModel : public GlobalSingleton<ApplicationModel>
{
public:
    ApplicationModel();

    osg::ref_ptr<osg::Node> sampleSceneGraph() const;

private:
    void setUpSampleScene();
    osg::ref_ptr<osg::Group> createAxisMarker();

    osg::ref_ptr<osg::Group> m_SampleSceneGraph;
};

#endif // APPLICATIONMODEL_H
