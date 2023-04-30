#include "texturemanager.h"
#include <QDebug>

TextureManager::TextureManager()
{}


void TextureManager::eraseAll()
{
    for (auto texture : textures.values())
    {
        delete texture;
    }
}
QOpenGLTexture* TextureManager::getTexture(const QString& textureName) const
{
    return textures.value(textureName, nullptr);
}

void TextureManager::addTexture(const QString& textureName, const QString& texturePath)
{
    QImage image(texturePath);

    if (image.isNull())
    {
        qDebug() << "Failed to load texture: " << texturePath;
        return;
    }

    // Ensure the image has an alpha channel
    image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);

    QOpenGLTexture* texture = new QOpenGLTexture(image.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);

    // Add alpha channel settings
    //texture->setFormat(QOpenGLTexture::RGBA8_UNorm);


    textures.insert(textureName, texture);
}
