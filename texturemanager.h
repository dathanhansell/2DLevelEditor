#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QString>
#include <QMap>
#include <QOpenGLTexture>

class TextureManager
{
public:
    TextureManager();
    void eraseAll();

    QOpenGLTexture* getTexture(const QString& textureName) const;
    void addTexture(const QString& textureName, const QString& texturePath);

private:
    QMap<QString, QOpenGLTexture*> textures;
};

#endif // TEXTUREMANAGER_H
