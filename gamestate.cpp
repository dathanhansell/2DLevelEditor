#include "gamestate.h"

GameState::GameState(QObject *parent) : QObject(parent), playerTile(nullptr), box2dWorld(nullptr)
{
}

GameState::~GameState()
{
    for (auto &obj : drawableObjects)
    {
        delete obj;
    }

    for (auto &tile : tileList)
    {
        delete tile;
    }
}

void GameState::initialize(ShaderManager *shaderManager)
{
    box2dWorld = new b2World(b2Vec2(0, -9.8));
    TileMesh *tilemesh = new TileMesh();
    GridMesh *gridmesh = new GridMesh();

    Drawable *gridObject = new Drawable(shaderManager->getShader("texture"), new QOpenGLTexture(QImage(":/textures/white.png").mirrored()), gridmesh);

    drawableObjects.push_back(gridObject);
    Drawable *playerDrawable = new Drawable(shaderManager->getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);

    playerTile = new PlayerTile(playerDrawable, box2dWorld, b2Vec2(0, 5), b2Vec2(1, 1));
    drawableObjects.push_back(playerTile->getDrawable());
    tileList.push_back(playerTile);
}

void GameState::update(float deltaTime)
{
    for (Tile *tile : tileList)
    {
        tile->update(deltaTime);
    }

    for (Tile *tile : tileList)
    {
        if (InteractableTile *itile = dynamic_cast<InteractableTile *>(tile))
        {
            for (Tile *other : tileList)
            {
                if (tile != other && tile->isCollidingWith(other))
                {
                    itile->onCollision(other);
                }
            }
        }
    }
}

void GameState::render(QMatrix4x4 &projection, QMatrix4x4 &view)
{
    for (auto &obj : drawableObjects)
    {
        obj->draw(projection, view);
    }
}

void GameState::handleKeyPressEvent(QKeyEvent *event)
{
    if (playerTile)
    {
        playerTile->handleKeyEvent(event);
    }
}

void GameState::handleKeyReleaseEvent(QKeyEvent *event)
{
    if (playerTile)
    {
        playerTile->handleKeyEvent(event);
    }
}
