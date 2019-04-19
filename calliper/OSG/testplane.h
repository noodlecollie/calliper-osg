#ifndef TESTPLANE_H
#define TESTPLANE_H

#include <QtGlobal>
#include <osg/Object>
#include <osg/Geometry>
#include <osg/Array>

// Trimmed-down example of how building of custom geometry works with OSG.
// This is adapted from the structure of the shape geometry-building code.

// Forward-declared for use with TestPlane.
class BuildTestPlaneGeometryVisitor;

// Class that defines an XY plane with all sides being a specific length.
// The bounds of the plane are -size/2 to size/2.
class TestPlane : public osg::Object
{
    META_Object(calliper, TestPlane)
public:
    TestPlane(float size = 1.0f);
    TestPlane(const TestPlane& other, const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

    bool isValid() const;

    float size() const;
    void setSize(float size);

    osg::Vec3d position() const;
    void setPosition(const osg::Vec3d& pos);

    // Specifies that the TestPlane supports the functionality on the visitor.
    // This function just calls through to apply() and passes a reference to itself.
    void accept(BuildTestPlaneGeometryVisitor& visitor) const;

private:
    osg::Vec3d m_Position;
    float m_Size;
};

// Subclass of osg::Geometry that serves to create the builder visitor and run it on the plane object.
class TestPlaneDrawable : public osg::Geometry
{
    META_Object(calliper, TestPlaneDrawable)
public:
    TestPlaneDrawable(const osg::ref_ptr<TestPlane>& plane = osg::ref_ptr<TestPlane>());
    TestPlaneDrawable(const TestPlaneDrawable& other, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

    void build();

private:
    osg::ref_ptr<TestPlane> m_Plane;
};

// This class does the actual geometry building. apply() converts the data in the
// TestPlane to vertex data and inserts this into the passed geometry (the TestPlaneDrawable).
class BuildTestPlaneGeometryVisitor
{
public:
    BuildTestPlaneGeometryVisitor(osg::Geometry* geometry);

    void apply(const TestPlane& plane);

    void begin(GLenum mode);
    void end();

    void setMatrix(const osg::Matrixd& m);

private:
    osg::Geometry* m_Geometry;
    osg::ref_ptr<osg::Vec3Array> m_Vertices;
    osg::ref_ptr<osg::Vec3Array> m_Normals;
    osg::ref_ptr<osg::Vec2Array> m_TexCoords;
    GLenum m_Mode;
    quint32 m_StartIndex;
    osg::Matrixd m_Matrix;
    osg::Matrixd m_InverseMatrix;
};

#endif // TESTPLANE_H
