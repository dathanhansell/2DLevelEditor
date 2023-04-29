#include "drawable.h"
#include <Box2D.h>

class Tile {
public:
    Tile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size);
    virtual ~Tile();

    virtual void update(float deltaTime);
    virtual void draw(QMatrix4x4& projection, QMatrix4x4& view);

    // Add methods for collision checking
    bool isCollidingWith(const Tile *other) const;
    Drawable* getDrawable() const;
    b2Body* getBody() const;

protected:
    Drawable* drawable;
    b2Body* body;
};
