#include "tilefactory.h"
#include "tilemesh.h"

TileFactory::TileFactory(b2World *world,TextureManager* tmanager, ShaderManager* smanager)
    : box2dWorld(world),textureManager(tmanager) ,shaderManager(smanager)
{
    loadTextures();
}
TileFactory::~TileFactory(){
    delete textureManager;
    delete shaderManager;
    delete box2dWorld;
}

void TileFactory::loadTextures()
{
    loadTextureFromPrefix("");
    loadTextureFromPrefix("static");
    loadTextureFromPrefix("interactable");
    loadTextureFromPrefix("entities");
    loadTextureFromPrefix("background");
}

Tile *TileFactory::createTile(const QString &textureName, const b2Vec2 &pos)
{
    TileMesh *tileMesh = new TileMesh();

    Drawable *drawable = new Drawable(shaderManager->getShader("texture"), textureManager->getTexture(textureName), tileMesh);
    b2Vec2 tileSize(1, 1);

    return new Tile(drawable, box2dWorld, pos, tileSize);
}

InteractableTile *TileFactory::createInteractableTile(const QString &textureName, const b2Vec2 &pos, InteractableType type)
{
    TileMesh *tileMesh = new TileMesh();

    Drawable *drawable = new Drawable(shaderManager->getShader("texture"), textureManager->getTexture(textureName), tileMesh);
    b2Vec2 tileSize(1, 1);

    //implement custom behavior for InteractableTile based on the given InteractableType
    return new InteractableTile(drawable, box2dWorld, pos, tileSize);
}

PlayerTile *TileFactory::createPlayerTile(const QString &textureName, const b2Vec2 &pos)
{
    TileMesh *tileMesh = new TileMesh();

    Drawable *drawable = new Drawable(shaderManager->getShader("texture"), textureManager->getTexture(textureName), tileMesh);
    b2Vec2 tileSize(1, 1);

    return new PlayerTile(drawable, box2dWorld, pos, tileSize);
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
