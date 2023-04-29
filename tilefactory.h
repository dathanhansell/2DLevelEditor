#pragma once
#include "playertile.h""
#include "texturemanager.h"
#include "shadermanager.h"
#include <Box2D.h>
#include <QListWidget>
#include <QDirIterator>
#include <QFileInfo>

enum class InteractableType {
    Coin,
    BreakBlock,
};

enum class EntityType {
    Goomba,
    Koopa,
};

class TileFactory {
public:
    TileFactory(b2World *world,TextureManager* tmanager, ShaderManager* smanager);
    ~TileFactory();
    // Load texture for tiles.
    void loadTextures();
    Tile *createTile(const QString &textureName, const b2Vec2 &pos);
    InteractableTile *createInteractableTile(const QString &textureName, const b2Vec2 &pos, InteractableType type);
    PlayerTile *createPlayerTile(const QString &textureName, const b2Vec2 &pos);
private:
    b2World *box2dWorld;
    TextureManager* textureManager;
    ShaderManager* shaderManager;

    void loadTextureFromPrefix(const QString &prefix);
};
