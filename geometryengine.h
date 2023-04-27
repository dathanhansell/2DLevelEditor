#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void drawTileGeometry(QOpenGLShaderProgram *program);
    void drawGridGeometry(QOpenGLShaderProgram *program, int gridSize); // new function

private:
    void initCubeGeometry();
    void initGridGeometry(int gridSize); // new function

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLBuffer gridArrayBuf; // new buffer for grid vertices
    QOpenGLBuffer gridIndexBuf; // new buffer for grid indices
};

#endif // GEOMETRYENGINE_H
