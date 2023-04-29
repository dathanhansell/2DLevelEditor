#include "mesh.h"

Mesh::Mesh()
    :indexBuf(QOpenGLBuffer::IndexBuffer),
    arrayBuf(QOpenGLBuffer::VertexBuffer)
{
    initializeOpenGLFunctions();
}

Mesh::~Mesh()
{
}

void Mesh::create()
{
    arrayBuf.create();
    indexBuf.create();
}

void Mesh::destroy()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}

void Mesh::bind()
{
    arrayBuf.bind();
    indexBuf.bind();
}

void Mesh::release()
{
}

void Mesh::draw()
{

}
