// mesh.h
#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector2D>
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};
class Mesh : public QOpenGLFunctions
{
public:
    Mesh();
    virtual ~Mesh();

    void create();
    void destroy();

    void bind();
    void release();

    virtual void draw();

protected:
    virtual void setupGeometry() = 0;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // MESH_H
