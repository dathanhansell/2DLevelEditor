#include "enemygoomba.h"

EnemyGoomba::EnemyGoomba(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : InteractableTile(drawable, world, pos, size)
{
    body->SetType(b2_dynamicBody);

    body->DestroyFixture(body->GetFixtureList());

    b2PolygonShape box;
    box.SetAsBox(size.x / 2.1f, size.y / 2.1f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0;

    body->CreateFixture(&fixtureDef);
    body->SetEnabled(true);
    body->SetAwake(true);
    body->SetFixedRotation(true);
    body->SetLinearDamping(0.0f);
    body->SetAngularDamping(0.0f);
    speed = 2.0f;
    direction = 1;
}

void EnemyGoomba::update(float deltaTime)
{
    InteractableTile::update(deltaTime);

    float desiredSpeed = direction * speed;

    float velocityChange = desiredSpeed - body->GetLinearVelocity().x;
    float impulse = velocityChange;
    body->ApplyLinearImpulse(b2Vec2(impulse,0), body->GetWorldCenter(), true);
}

void EnemyGoomba::onCollision(const Tile* other)
{

    if (other != nullptr && typeid(*other) == typeid(PlayerTile))
    {
        b2Vec2 relativePosition = other->getBody()->GetPosition() - body->GetPosition();

        if (relativePosition.y > .9f)
        {
            disappear();
        }
        else{
            //playerdie
        }
    }
    else if (other != nullptr)
    {
        b2Vec2 relativePosition =   other->getBody()->GetPosition()-body->GetPosition();
        float tolerance = 0.01f;


        if (relativePosition.y > tolerance)
        {

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


