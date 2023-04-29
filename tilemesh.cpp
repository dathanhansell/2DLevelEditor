// tilemesh.cpp
#include "tilemesh.h"

TileMesh::TileMesh()
{
    create();
    setupGeometry();
}

TileMesh::~TileMesh()
{
    destroy();
}

void TileMesh::setupGeometry()
{
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(0, 0,  1.0f), QVector2D(0.0f, 0.0f)},
        {QVector3D( 1.0f, 0,  1.0f), QVector2D(0.33f, 0.0f)},
        {QVector3D(0,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)},
    };
    GLushort indices[] = {
        0,  1,  2,  3,  3,

    };
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 4 * sizeof(VertexData));

    indexBuf.bind();
    indexBuf.allocate(indices, 5 * sizeof(GLushort));

}

void TileMesh::draw(){
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
}
