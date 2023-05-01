#ifndef COINTILE_H
#define COINTILE_H

#include "playertile.h"

class CoinTile : public InteractableTile {
public:
    CoinTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size);

    void update(float deltaTime) override;
    void onCollision(const Tile* other) override;

    void disappear();
    void set() override;
    void reset() override;

private:
    QVector2D startPosition;
};

#endif
