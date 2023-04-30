#include "tilefactory.h"
#include "tilemesh.h"

TileFactory::TileFactory(b2World *world,TextureManager* tmanager, ShaderManager* smanager)
    : box2dWorld(world),textureManager(tmanager) ,shaderManager(smanager)
{
    loadTextures();
}
TileFactory::~TileFactory(){
}

void TileFactory::loadTextures()
{
    loadTextureFromPrefix("");
    loadTextureFromPrefix("static");
    loadTextureFromPrefix("interactable");
    loadTextureFromPrefix("entities");
    loadTextureFromPrefix("background");
}

Tile *TileFactory::createTile(const QString &textureName, const b2Vec2 &pos, bool hasCollision)
{
    TileMesh *tileMesh = new TileMesh();

    Drawable *drawable = new Drawable(shaderManager->getShader("texture"), textureManager->getTexture(textureName), tileMesh);
    b2Vec2 tileSize(1, 1);
    if(textureName == "mario")
        return new PlayerTile(drawable, box2dWorld, pos, tileSize);
    else if(textureName == "goomba")
        return new EnemyGoomba(drawable, box2dWorld, pos, tileSize);
    else
        return new Tile(drawable, box2dWorld, pos, tileSize,hasCollision);
}

void TileFactory::loadTextureFromPrefix(const QString &prefix)
{
    QDirIterator it(":/"+prefix+"/textures/"+prefix+"/");
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        if (fileInfo.isFile()) {
            QString textureName = fileInfo.baseName();
            textureManager->addTexture(textureName, fileInfo.absoluteFilePath());
        }
    }
}
