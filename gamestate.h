#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QVector>
#include "playertile.h"
#include "gridmesh.h"
#include "tilemesh.h"
#include "drawable.h"
#include "shadermanager.h"
#include "Box2D.h"

class GameState : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GameState(QObject *parent = nullptr);
    ~GameState();

    void initialize(ShaderManager *shaderManager);
    void update(float deltaTime);
    void render(QMatrix4x4 &projection, QMatrix4x4 &view);
    void handleKeyPressEvent(QKeyEvent *event);
    void handleKeyReleaseEvent(QKeyEvent *event);

    QVector<Tile *> &getTileList() { return tileList; }

private:
    QVector<Tile *> tileList;
    PlayerTile *playerTile;
    QVector<Drawable *> drawableObjects;

    b2World *box2dWorld;
};

#endif // GAMESTATE_H
