#include "drawable.h"

Drawable::Drawable(QOpenGLShaderProgram *shader,
                   QOpenGLTexture *texture,
                   Mesh *mesh)
    : shader_(shader), texture_(texture), mesh_(mesh), position_(QVector3D(0.0, 0.0, 0.0)), scale_(QVector3D(1.0, 1.0, 1.0)), rotation_(QQuaternion())
{
}

Drawable::~Drawable()
{
}
void Drawable::setScale(const QVector2D &scale)
{
    scale_ = scale;
}

void Drawable::setRotation(const QQuaternion &rotation)
{
    rotation_ = rotation;
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

void Drawable::setPosition(const QVector2D &position)
{
    position_ = position;
}
QVector2D Drawable::getPosition() const{
    return position_;
}
QVector3D Drawable::twoToThree(QVector2D two){
    return {two.x(),two.y(),0};
}

void Drawable::draw(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
    if (shader_ && shader_->bind())
    {
        QMatrix4x4 model;
        model.translate(twoToThree(position_));
        model.rotate(rotation_);
        model.scale(twoToThree(scale_));
        shader_->setUniformValue("mvp_matrix", projection * view * model);
        shader_->setUniformValue("texture", 0);
        if (texture_) {
            texture_->bind();
        }

        if (mesh_) {
            mesh_->bind();
            quintptr offset = 0;

            // Tell OpenGL programmable pipeline how to locate vertex position data
            int vertexLocation = shader_->attributeLocation("a_position");
            shader_->enableAttributeArray(vertexLocation);
            shader_->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

            // Offset for texture coordinate
            offset += sizeof(QVector3D);

            // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
            int texcoordLocation = shader_->attributeLocation("a_texcoord");
            shader_->enableAttributeArray(texcoordLocation);
            shader_->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
            mesh_->draw();
            mesh_->release();
        }

        shader_->release();
    }
}
