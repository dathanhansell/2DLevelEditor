#include "playertile.h"
#include "enemygoomba.h"

PlayerTile::PlayerTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : InteractableTile(drawable, world, pos, size)
{
    // Change the body type to dynamic.
    body->SetType(b2_dynamicBody);

    // Destroy the existing fixture and create a new fixture
    body->DestroyFixture(body->GetFixtureList());

    b2PolygonShape box;
    box.SetAsBox(size.x/2,size.y/2); // Adjust the collider size and positioning

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f; // Disable friction on the player collider

    body->CreateFixture(&fixtureDef);
    body->SetEnabled(true);
    body->SetAwake(true);
    body->SetFixedRotation(true);
}

void PlayerTile::update(float deltaTime)
{
    InteractableTile::update(deltaTime);

    const float maxSpeed = speed; // Set a max speed for movement
    const float dampingFactor = 3.0f; // Set a damping factor for smooth stopping
    float desiredSpeed = 0.0f;

    if(left)
        desiredSpeed -= maxSpeed;
    if(right)
        desiredSpeed += maxSpeed;

    float velocityChange = desiredSpeed - body->GetLinearVelocity().x;
    float impulse = body->GetMass() * velocityChange;

    body->ApplyLinearImpulse(b2Vec2(impulse, 0.0f), body->GetWorldCenter(), true);

    if(jump && isOnGround) {
        isOnGround = false;
        float jumpForce = speed * 35;
        body->ApplyForceToCenter(b2Vec2(0.0, jumpForce), true);
    }

    if(!left && !right) {
        // Damp movement when no input is present
        body->SetLinearDamping(dampingFactor);
    } else {
        body->SetLinearDamping(0.0f);
    }
}
void PlayerTile::reset(){
    setPosition({0,0});
}

void PlayerTile::onCollision(const Tile* other)
{
    if(other != nullptr)
    {
        if (dynamic_cast<const EnemyGoomba*>(other)) {
            b2Vec2 relativePosition = other->getBody()->GetPosition() - body->GetPosition();

            if (relativePosition.y > 0) {
                // Player jumps on Goomba
                // Handle Goomba death logic (if any)
            } else {
                // Player collides with Goomba horizontally
                setPlayerDead(true);
            }
        }
        b2Vec2 relativePosition =  other->getBody()->GetPosition()-body->GetPosition();

        // Set friction to 0.0f for both the player and the ground/platform colliders when the player is in mid-air
        b2Fixture* fixture = body->GetFixtureList();
        if (relativePosition.y < 0) {
            isOnGround = true;
            //fixture->SetFriction(0.3f);
        } else {
            isOnGround = false;
            fixture->SetFriction(0.0f);
        }
    }
    else
    {
        isOnGround = false;
    }
}
void PlayerTile::addPlayerDeathListener(const std::function<void()>& listener)
{
    playerDeathListeners.push_back(listener);
}

void PlayerTile::setPlayerDead(bool isDead)
{
    if (playerIsDead != isDead) {
        playerIsDead = isDead;
        if (isDead) {
            onPlayerDeath();
        }
    }
}

void PlayerTile::onPlayerDeath()
{
    for (auto& listener : playerDeathListeners) {
        listener();
    }
}
void PlayerTile::handleKeyEvent(QKeyEvent *event) {

    bool pressed = event->type() == QEvent::KeyPress;
    switch (event->key()) {
    case Qt::Key_A:
        left = pressed;
        break;
    case Qt::Key_D:
        right = pressed;
        break;
    case Qt::Key_Space:
        jump = pressed;
        break;
    }
}
