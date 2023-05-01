#include "breakblock.h"

BreakBlock::BreakBlock(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : InteractableTile(drawable, world, pos, size)
{
}

void BreakBlock::update(float deltaTime)
{
    InteractableTile::update(deltaTime);

    float desiredSpeed = direction * speed;
    float velocityChange = desiredSpeed - body->GetLinearVelocity().x;
    float impulse = body->GetMass() * velocityChange;

    body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}

void BreakBlock::onCollision(const Tile* other)
{

    if (other != nullptr && typeid(*other) == typeid(PlayerTile))
    {
        b2Vec2 relativePosition = other->getBody()->GetPosition() - body->GetPosition();

        if (relativePosition.y < 0)
        {
            disappear();
        }
    }
}

void BreakBlock::disappear()
{
    setVisible(false);
    body->SetEnabled(false);
}
void BreakBlock::set()
{
    startPosition = {drawable->getPosition().x(),drawable->getPosition().y()};
}

void BreakBlock::reset()
{
    setVisible(true);
    body->SetEnabled(true);
    setPosition(startPosition);
    body->SetLinearVelocity(b2Vec2(0, 0));
}


