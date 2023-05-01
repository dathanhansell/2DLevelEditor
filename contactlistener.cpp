
#include "contactlistener.h"

void ContactListener::BeginContact(b2Contact* contact) {
    Tile* tileA = reinterpret_cast<Tile*>(static_cast<uintptr_t>(contact->GetFixtureA()->GetBody()->GetUserData().pointer));
    Tile* tileB = reinterpret_cast<Tile*>(static_cast<uintptr_t>(contact->GetFixtureB()->GetBody()->GetUserData().pointer));

    InteractableTile* interactableA = dynamic_cast<InteractableTile*>(tileA);
    InteractableTile* interactableB = dynamic_cast<InteractableTile*>(tileB);

    if (interactableA) {
        interactableA->onCollisionNextUpdate(tileB);
    }
    if (interactableB) {
        interactableB->onCollisionNextUpdate(tileA);
    }
}
void ContactListener::EndContact(b2Contact* contact) {
    // Handle end contact logic if necessary
}
