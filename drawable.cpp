// drawableobject.cpp
#include "drawable.h"

Drawable::Drawable(QOpenGLShaderProgram *shader,
                               QOpenGLTexture *texture,
                               Mesh *mesh)
    : shader_(shader), texture_(texture), mesh_(mesh), position_(QVector3D(0.0, 0.0, 0.0))
{
}

Drawable::~Drawable()
{
}

void Drawable::setShader(QOpenGLShaderProgram *shader)
{
    shader_ = shader;
}

void Drawable::setTexture(QOpenGLTexture *texture)
{
    texture_ = texture;
}

void Drawable::setGeometry(Mesh *mesh)
{
    mesh_ = mesh;
}

void Drawable::setPosition(const QVector3D &position)
{
    position_ = position;
}

void Drawable::draw(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
    if (shader_ && shader_->bind())
    {
        QMatrix4x4 model;
        model.translate(position_);
        shader_->setUniformValue("mvp_matrix", projection * view * model);
        shader_->setUniformValue("texture", 0);
        if (texture_) {
            texture_->bind();
        }

        if (mesh_) {
            mesh_->bind();
            mesh_->draw();
            mesh_->release();
        }

        shader_->release();
    }
}
