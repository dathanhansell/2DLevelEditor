// InteractableTile.cpp
#include "interactabletile.h"

void InteractableTile::onCollisionNextUpdate(Tile* other) {
    // Store the tile to be collided with on the next update step
    collidedTiles.push_back(other);
}

void InteractableTile::clearCollisions() {
    collidedTiles.clear();
}

void InteractableTile::update(float deltaTime) {
    // Call onCollision for each collided tile and then clear the list
    for (auto& tile : collidedTiles) {
        onCollision(tile);
    }
    clearCollisions();

    // Call the base update method
    Tile::update(deltaTime);
}
