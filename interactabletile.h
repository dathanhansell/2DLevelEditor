#include "tile.h"

class InteractableTile : public Tile {
public:
    InteractableTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
        : Tile(drawable, world, pos, size) {}
    virtual void set(){}
    virtual void reset(){}
    // Respond to collisions
    virtual void onCollision(const Tile* other) {}
};
