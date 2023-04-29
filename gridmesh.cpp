// tilemesh.cpp
#include "gridmesh.h"
#define gridSize 100

GridMesh::GridMesh()
{
    create();
    setupGeometry();
}

GridMesh::~GridMesh()
{
    destroy();
}

void GridMesh::setupGeometry()
{
    // Calculate the total number of vertices and indices.
    int vertexCount = (gridSize + 1) * (gridSize + 1);
    int indexCount = gridSize * gridSize * 4; // one square has 4 lines, and each line has 2 indices

    QVector<VertexData> vertices(vertexCount);
    QVector<GLushort> indices(indexCount);

    // Initialize vertices
    for (int i = 0; i <= gridSize; ++i) {
        for (int j = 0; j <= gridSize; ++j) {
            QVector3D position(i - gridSize / 2.0f, j - gridSize / 2.0f, 0);
            QVector2D texCoord(i / static_cast<float>(gridSize), j / static_cast<float>(gridSize));
            vertices[i * (gridSize + 1) + j].position = position;
            vertices[i * (gridSize + 1) + j].texCoord = texCoord;
        }
    }

    // Initialize indices
    int index = 0;
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            indices[index++] = i * (gridSize + 1) + j;
            indices[index++] = i * (gridSize + 1) + j + 1;

            indices[index++] = i * (gridSize + 1) + j;
            indices[index++] = (i + 1) * (gridSize + 1) + j;
        }
    }

    // Transfer vertex data to grid VBO

    arrayBuf.bind();
    arrayBuf.allocate(vertices.constData(), vertices.size() * sizeof(VertexData));

    // Transfer index data to grid VBO

    indexBuf.bind();
    indexBuf.allocate(indices.constData(), indices.size() * sizeof(GLushort));

}

void GridMesh::draw(){
    glDrawElements(GL_LINES, gridSize * gridSize * 4, GL_UNSIGNED_SHORT, nullptr);
}
