#include "cointile.h"

CoinTile::CoinTile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : InteractableTile(drawable, world, pos, size)
{
}

void CoinTile::update(float deltaTime)
{
    InteractableTile::update(deltaTime);
}

void CoinTile::onCollision(const Tile* other)
{

    if (other != nullptr && typeid(*other) == typeid(PlayerTile))
    {
            disappear();
    }
}

void CoinTile::disappear()
{
    setVisible(false);
    body->SetEnabled(false);
}
void CoinTile::set()
{
    startPosition = {drawable->getPosition().x(),drawable->getPosition().y()};
}

void CoinTile::reset()
{
    setVisible(true);
    body->SetEnabled(true);
    setPosition(startPosition);
}


