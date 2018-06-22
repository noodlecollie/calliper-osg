#ifndef CVECTOR_H
#define CVECTOR_H

// Wrappers that can be used with either Qt or OSG vectors.
// Converting to double precision vectors doesn't gain anything
// because the original vectors are stored as floats, but we
// don't really care about double precision anyway.

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include "OSG/pushwarnings.h"
#include "osg/Vec2f"
#include "osg/Vec2d"
#include "osg/Vec3f"
#include "osg/Vec3d"
#include "osg/Vec4f"
#include "osg/Vec4d"
#include "OSG/popwarnings.h"

class CVector2D : public QVector2D
{
public:
    CVector2D()
        : QVector2D()
    {
    }

    CVector2D(float x, float y)
        : QVector2D(x, y)
    {
    }

    CVector2D(const osg::Vec2f& vec)
        : QVector2D(vec[0], vec[1])
    {
    }

    CVector2D(const osg::Vec2d& vec)
        : QVector2D(vec[0], vec[1])
    {
    }

    CVector2D(const QVector2D& vec)
        : QVector2D(vec)
    {
    }

    operator osg::Vec2f() const
    {
        return osg::Vec2f(x(), y());
    }

    osg::Vec2d asDouble() const
    {
        return osg::Vec2d(x(), y());
    }
};

class CVector3D : public QVector3D
{
public:
    CVector3D()
        : QVector3D()
    {
    }

    CVector3D(float x, float y, float z)
        : QVector3D(x, y, z)
    {
    }

    CVector3D(const osg::Vec3f& vec)
        : QVector3D(vec[0], vec[1], vec[2])
    {
    }

    CVector3D(const osg::Vec3d& vec)
        : QVector3D(vec[0], vec[1], vec[2])
    {
    }

    CVector3D(const QVector3D& vec)
        : QVector3D(vec)
    {
    }

    operator osg::Vec3f() const
    {
        return osg::Vec3f(x(), y(), z());
    }

    osg::Vec3d asDouble() const
    {
        return osg::Vec3d(x(), y(), z());
    }
};

class CVector4D : public QVector4D
{
public:
    CVector4D()
        : QVector4D()
    {
    }

    CVector4D(float x, float y, float z, float w)
        : QVector4D(x, y, z, w)
    {
    }

    CVector4D(const osg::Vec4f& vec)
        : QVector4D(vec[0], vec[1], vec[2], vec[3])
    {
    }

    CVector4D(const osg::Vec4d& vec)
        : QVector4D(vec[0], vec[1], vec[2], vec[3])
    {
    }

    CVector4D(const QVector4D& vec)
        : QVector4D(vec)
    {
    }

    operator osg::Vec4f() const
    {
        return osg::Vec4f(x(), y(), z(), w());
    }

    osg::Vec4d asDouble() const
    {
        return osg::Vec4d(x(), y(), z(), w());
    }
};

#endif // CVECTOR_H
