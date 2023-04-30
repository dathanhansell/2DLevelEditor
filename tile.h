#include "drawable.h"
#include <Box2D.h>

class Tile {
public:
    Tile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size, bool hasCollision = true);
    virtual ~Tile();

    virtual void update(float deltaTime);
    virtual void draw(QMatrix4x4& projection, QMatrix4x4& view);

    // Add methods for collision checking
    bool isCollidingWith(const Tile *other) const;
    Drawable* getDrawable() const;
    b2Body* getBody() const;
    void setPosition(QVector2D pos);
    void setVisible(bool isVisible);

protected:
    bool visible = true;
    Drawable* drawable;
    b2Body* body;
};
