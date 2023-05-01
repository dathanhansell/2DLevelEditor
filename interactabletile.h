#ifndef INTERCTABLETILE_H
#define INTERCTABLETILE_H
#include "tile.h"
#include <vector>
class InteractableTile : public Tile {
public:
    InteractableTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size, bool hasCollision = true)
        : Tile(drawable, world, pos, size,hasCollision) {}
    virtual void set(){}
    virtual void reset(){}
    void onCollisionNextUpdate(Tile* other);
    // Respond to collisions
    virtual void onCollision(const Tile* other) {}
protected:
    void clearCollisions();
    void update(float deltaTime) override;
    std::vector<Tile*> collidedTiles;
};
#endif
