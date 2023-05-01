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
    void setPosition(const QVector2D &position);
    void setScale(const QVector2D &scale);
    void setRotation(const QQuaternion &rotation);
    QVector2D getPosition() const;

    virtual void draw(const QMatrix4x4 &projection, const QMatrix4x4 &view);

protected:
    QVector3D twoToThree(QVector2D two);
    QOpenGLShaderProgram *shader_;
    QOpenGLTexture *texture_;
    Mesh *mesh_;
    QVector2D position_;
    QVector2D scale_;
    QQuaternion rotation_;
};

#endif
