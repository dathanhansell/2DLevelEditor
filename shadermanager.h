#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QOpenGLShaderProgram>
#include <QMap>

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    QOpenGLShaderProgram* getShader(const QString& shaderName) const;
    void addShader(const QString& shaderName, const QString& vertexShaderPath, const QString& fragmentShaderPath);

private:
    QMap<QString, QOpenGLShaderProgram*> shaders;
};

#endif // SHADERMANAGER_H
