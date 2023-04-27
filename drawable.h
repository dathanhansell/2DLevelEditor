// drawableobject.h
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector3D>
#include "mesh.h"

class Drawable
{
public:
    Drawable(QOpenGLShaderProgram *shader, QOpenGLTexture *texture, Mesh *mesh);
    virtual ~Drawable();

    void setShader(QOpenGLShaderProgram *shader);
    void setTexture(QOpenGLTexture *texture);
    void setGeometry(Mesh *mesh);
    void setPosition(const QVector3D &position);

    virtual void draw(const QMatrix4x4 &projection, const QMatrix4x4 &view);

protected:
    QOpenGLShaderProgram *shader_;
    QOpenGLTexture *texture_;
    Mesh *mesh_;
    QVector3D position_;
};

#endif // DRAWABLEOBJECT_H
