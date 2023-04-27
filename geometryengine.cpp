// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer),
    gridArrayBuf(QOpenGLBuffer::VertexBuffer), // initialize grid vertex buffer
    gridIndexBuf(QOpenGLBuffer::IndexBuffer) // initialize grid index buffer
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initCubeGeometry();
    initGridGeometry(100); // default grid size = 10
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initCubeGeometry()
{
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(0, 0,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, 0,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(0,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3


    };

    GLushort indices[] = {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)

    };

    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));


    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
    //! [1]
}
void GeometryEngine::initGridGeometry(int gridSize)
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
    gridArrayBuf.create();
    gridArrayBuf.bind();
    gridArrayBuf.allocate(vertices.constData(), vertices.size() * sizeof(VertexData));

    // Transfer index data to grid VBO
    gridIndexBuf.create();
    gridIndexBuf.bind();
    gridIndexBuf.allocate(indices.constData(), indices.size() * sizeof(GLushort));
}

//! [2]
void GeometryEngine::drawTileGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
}
void GeometryEngine::drawGridGeometry(QOpenGLShaderProgram *program, int gridSize)
{
    if (gridSize < 1)
        return;

    // Tell OpenGL which VBOs to use
    gridArrayBuf.bind();
    gridIndexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw grid geometry using indices from the grid index buffer
    glDrawElements(GL_LINES, gridSize * gridSize * 4, GL_UNSIGNED_SHORT, nullptr); // use GL_LINES instead of GL_TRIANGLES
}
//! [2]
