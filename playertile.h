#ifndef PLAYERTILE_H
#define PLAYERTILE_H
#include <QKeyEvent>
#include "interactabletile.h"

class PlayerTile : public InteractableTile
{
public:
    PlayerTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size);
    void update(float deltaTime) override;
    void onCollision(const Tile* other) override;
    void handleKeyEvent(QKeyEvent *event);
private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);

private:
    float speed = 5.0f;
    bool left = false, right = false, jump = false;


    bool isOnGround = false;
};

#endif // PLAYERTILE_H
