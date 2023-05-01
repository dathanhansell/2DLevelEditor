#ifndef BREAKBLOCK_H
#define BREAKBLOCK_H

#include "playertile.h"

class BreakBlock : public InteractableTile {
public:
    BreakBlock(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size);

    void update(float deltaTime) override;
    void onCollision(const Tile* other) override;

    void disappear();
    void set() override;
    void reset() override;

private:
    float speed;
    int direction;
    QVector2D startPosition;
};

#endif
