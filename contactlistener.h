#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H
#include "box2d.h"
#include "interactabletile.h"
class ContactListener : public b2ContactListener {
public:
    ContactListener() = default;

    void BeginContact(b2Contact* contact) override;

    void EndContact(b2Contact* contact) override;
};
#endif
