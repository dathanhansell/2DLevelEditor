#include "shadermanager.h"

ShaderManager::ShaderManager()
{}

ShaderManager::~ShaderManager()
{
    for (auto shader : shaders.values())
    {
        delete shader;
    }
}

QOpenGLShaderProgram* ShaderManager::getShader(const QString& shaderName) const
{
    return shaders.value(shaderName, nullptr);
}

void ShaderManager::addShader(const QString& shaderName, const QString& vertexShaderPath, const QString& fragmentShaderPath)
{
    QOpenGLShaderProgram* program = new QOpenGLShaderProgram;

    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath))
    {
        qDebug() << "Failed to load vertex shader: " << vertexShaderPath;
        delete program;
        return;
    }

    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath))
    {
        qDebug() << "Failed to load fragment shader: " << fragmentShaderPath;
        delete program;
        return;
    }

    if (!program->link())
    {
        qDebug() << "Failed to link shader program";
        delete program;
        return;
    }

    shaders.insert(shaderName, program);
}
