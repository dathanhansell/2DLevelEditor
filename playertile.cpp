#include "playertile.h"
PlayerTile::PlayerTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : InteractableTile(drawable, world, pos, size)
{
    // Change the body type to dynamic.
    body->SetType(b2_dynamicBody);

    // Destroy the existing fixture and create a new fixture
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
}
void PlayerTile::update(float deltaTime)
{
    qDebug() << drawable->getPosition();
    InteractableTile::update(deltaTime);

    b2Vec2 force = b2Vec2(0, 0);
    if (left)
        force.x -= speed;
    if (right)
        force.x += speed;
    if (jump && isOnGround){
        isOnGround = false;
        force.y += speed * 5;
    }
    qDebug() << force.x;
    qDebug() << body->GetPosition().x;
    body->ApplyForceToCenter(force, true);
    qDebug() << body->GetPosition().x;
}

void PlayerTile::onCollision(const Tile* other)
{
    // This is a very simple implementation and may not be perfect for all cases
    if(other != nullptr)
    {
        b2Vec2 relativePosition =  other->getBody()->GetPosition()-body->GetPosition();
        //qDebug() <<relativePosition.y;
        if (relativePosition.y < 0)
            isOnGround = true;
    }
    else
    {
        isOnGround = false;
    }
    qDebug() <<isOnGround;
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
