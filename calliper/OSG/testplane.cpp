#include "testplane.h"

TestPlane::TestPlane(float size) :
    m_Size(0.0f)
{
    setSize(size);
}

TestPlane::TestPlane(const TestPlane &other, const osg::CopyOp &copyOp) :
    osg::Object(other, copyOp),
    m_Size(other.m_Size)
{
}

bool TestPlane::isValid() const
{
    return m_Size > 0.0f;
}

float TestPlane::size() const
{
    return m_Size;
}

void TestPlane::setSize(float size)
{
    if ( size < 0.0f )
    {
        size = 0.0f;
    }

    m_Size = size;
}

osg::Vec3d TestPlane::position() const
{
    return m_Position;
}

void TestPlane::setPosition(const osg::Vec3d &pos)
{
    m_Position = pos;
}

void TestPlane::accept(BuildTestPlaneGeometryVisitor &visitor) const
{
    visitor.apply(*this);
}

TestPlaneDrawable::TestPlaneDrawable(const osg::ref_ptr<TestPlane> &plane) :
    osg::Geometry(),
    m_Plane(plane)
{
}

TestPlaneDrawable::TestPlaneDrawable(const TestPlaneDrawable &other, const osg::CopyOp &copyop) :
    osg::Geometry(other, copyop),
    m_Plane(other.m_Plane)
{
}

void TestPlaneDrawable::build()
{
    setVertexArray(0);
    setNormalArray(0);
    setColorArray(0);
    setSecondaryColorArray(0);
    setFogCoordArray(0);
    getTexCoordArrayList().clear();
    getVertexAttribArrayList().clear();
    getPrimitiveSetList().clear();

    if (m_Plane)
    {
        BuildTestPlaneGeometryVisitor visitor(this);
        m_Plane->accept(visitor);
    }
}

BuildTestPlaneGeometryVisitor::BuildTestPlaneGeometryVisitor(osg::Geometry *geometry) :
    m_Geometry(geometry)
{
    m_Vertices = dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
    m_Normals = dynamic_cast<osg::Vec3Array*>(geometry->getNormalArray());
    m_TexCoords = dynamic_cast<osg::Vec2Array*>(geometry->getTexCoordArray(0));
}

void BuildTestPlaneGeometryVisitor::apply(const TestPlane &plane)
{
    const osg::Vec3d pos = plane.position();
    setMatrix(osg::Matrixd::translate(pos.x(), pos.y(), pos.z()));

    begin(GL_QUADS);

    const osg::Vec3d normal(0,0,1);
    const float halfSize = plane.size() / 2.0f;

    for ( quint32 index = 0; index < 4; ++index )
    {
        const float x = (index == 1 || index == 2) ? halfSize : -halfSize;
        const float y = (index >= 2) ? halfSize : -halfSize;
        m_Vertices->push_back(osg::Vec3d(x, y, 0));

        const float texX = (index == 1 || index == 2) ? 1 : 0;
        const float texY = (index >= 2) ? 1 : 0;
        m_TexCoords->push_back(osg::Vec2d(texX, texY));

        m_Normals->push_back(normal);
    }

    end();
}

void BuildTestPlaneGeometryVisitor::begin(GLenum mode)
{
    m_Mode = mode;
    m_StartIndex = m_Vertices->size();
}

void BuildTestPlaneGeometryVisitor::end()
{
    if ( m_StartIndex >= m_Vertices->size() )
    {
        return;
    }

    bool smallPrimitiveSet = m_Vertices->size() < 65536;

    // TODO: Add triangle fan/strip as an option here?
    if ( m_Mode == GL_QUADS )
    {
        osg::ref_ptr<osg::DrawElements> primitives = smallPrimitiveSet ?
            static_cast<osg::DrawElements*>(new osg::DrawElementsUShort(GL_TRIANGLES)) :
            static_cast<osg::DrawElements*>(new osg::DrawElementsUInt(GL_TRIANGLES));

        m_Geometry->addPrimitiveSet(primitives.get());

        for ( quint32 i = m_StartIndex; i < m_Vertices->size(); i += 4 )
        {
            quint32 p0 = i;
            quint32 p1 = i + 1;
            quint32 p2 = i + 2;
            quint32 p3 = i + 3;

            primitives->addElement(p0);
            primitives->addElement(p1);
            primitives->addElement(p3);

            primitives->addElement(p1);
            primitives->addElement(p2);
            primitives->addElement(p3);
        }
    }
    else if ( m_Mode == GL_QUAD_STRIP )
    {
        osg::ref_ptr<osg::DrawElements> primitives = smallPrimitiveSet ?
            static_cast<osg::DrawElements*>(new osg::DrawElementsUShort(GL_TRIANGLES)) :
            static_cast<osg::DrawElements*>(new osg::DrawElementsUInt(GL_TRIANGLES));

        m_Geometry->addPrimitiveSet(primitives.get());

        for ( quint32 i = m_StartIndex; i < m_Vertices->size() - 2; i += 2 )
        {
            quint32 p0 = i;
            quint32 p1 = i + 1;
            quint32 p2 = i + 2;
            quint32 p3 = i + 3;

            primitives->addElement(p0);
            primitives->addElement(p1);
            primitives->addElement(p2);

            primitives->addElement(p1);
            primitives->addElement(p3);
            primitives->addElement(p2);
        }
    }
    else
    {
        m_Geometry->addPrimitiveSet(new osg::DrawArrays(m_Mode, m_StartIndex, m_Vertices->size() - m_StartIndex));
    }

    for ( quint32 i = m_StartIndex; i < m_Vertices->size(); ++i )
    {
        osg::Vec3& v = (*m_Vertices)[i];
        v = v * m_Matrix;

        osg::Vec3& n = (*m_Normals)[i];
        n = m_InverseMatrix * n;
        n.normalize();
    }

    m_Vertices->dirty();
    m_Normals->dirty();
    m_TexCoords->dirty();
    m_Geometry->dirtyGLObjects();

    m_StartIndex = m_Vertices->size();
}

void BuildTestPlaneGeometryVisitor::setMatrix(const osg::Matrixd& m)
{
    m_Matrix = m;

    m_InverseMatrix.invert(m);
    m_InverseMatrix.setTrans(0.0, 0.0, 0.0);
}
