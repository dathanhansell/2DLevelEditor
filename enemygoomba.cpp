#include "enemygoomba.h"

EnemyGoomba::EnemyGoomba(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : InteractableTile(drawable, world, pos, size)
{
    body->SetType(b2_dynamicBody);

    body->DestroyFixture(body->GetFixtureList());

    b2PolygonShape box;
    box.SetAsBox(size.x / 2, size.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    body->SetEnabled(true);
    body->SetAwake(true);
    body->SetFixedRotation(true);

    speed = 1.0f;
    direction = 1;
}

void EnemyGoomba::update(float deltaTime)
{
    InteractableTile::update(deltaTime);

    float desiredSpeed = direction * speed;
    float velocityChange = desiredSpeed - body->GetLinearVelocity().x;
    float impulse = body->GetMass() * velocityChange;

    body->ApplyLinearImpulse(b2Vec2(impulse, 0.0f), body->GetWorldCenter(), true);
}

void EnemyGoomba::onCollision(const Tile* other)
{

    if (other != nullptr && typeid(*other) == typeid(PlayerTile))
    {
        b2Vec2 relativePosition = other->getBody()->GetPosition() - body->GetPosition();

        if (relativePosition.y > 0)
        {
            // The player is above the Goomba, so the Goomba is squished
            disappear();
        }
    }
    else if (other != nullptr)
    {
        b2Vec2 relativePosition = other->getBody()->GetPosition() - body->GetPosition();
        float tolerance = 0.1f; // Add a tolerance to avoid detecting ground as wall due to floating point errors

        // Check if the collision is in the horizontal direction
        if (abs(relativePosition.y) > tolerance)
        {
            // Handle collision with the platform or other objects
            direction *= -1;
        }
    }
}

void EnemyGoomba::disappear()
{
    setVisible(false);
    body->SetEnabled(false);
}
void EnemyGoomba::set()
{
    startPosition = {drawable->getPosition().x(),drawable->getPosition().y()};
}

void EnemyGoomba::reset()
{
    setVisible(true);
    body->SetEnabled(true);
    setPosition(startPosition);
    body->SetLinearVelocity(b2Vec2(0, 0));
}


