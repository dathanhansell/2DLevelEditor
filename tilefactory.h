#pragma once
#include "playertile.h"
#include "enemygoomba.h"
#include "texturemanager.h"
#include "shadermanager.h"
#include <Box2D.h>
#include <QListWidget>
#include <QDirIterator>
#include <QFileInfo>

class TileFactory {
public:
    TileFactory(b2World *world,TextureManager* tmanager, ShaderManager* smanager);
    ~TileFactory();
    // Load texture for tiles.
    void loadTextures();
    Tile *createTile(const QString &textureName, const b2Vec2 &pos, bool hasCollision = true);
private:
    b2World *box2dWorld;
    TextureManager* textureManager;
    ShaderManager* shaderManager;

    void loadTextureFromPrefix(const QString &prefix);
};
