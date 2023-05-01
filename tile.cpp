#include "tile.h"

Tile::Tile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size, bool hasCollision)
    : drawable(drawable) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_staticBody;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(size.x / 2.1, size.y / 2.1);
    body->SetFixedRotation(true);
    if (hasCollision) {
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);
    }
}

Tile::~Tile() {
    delete drawable;
    body->GetWorld()->DestroyBody(body);
}

Drawable* Tile::getDrawable() const {
    return drawable;
}

b2Body* Tile::getBody() const{
    return body;
}

void Tile::setPosition(QVector2D pos){
    body->SetTransform({float(pos.x()),float(pos.y())},0);
    drawable->setPosition({body->GetPosition().x,body->GetPosition().y});
}

void Tile::update(float deltaTime) {
    if (drawable) {
        drawable->setPosition({body->GetPosition().x,body->GetPosition().y});
    }
}

void Tile::draw(QMatrix4x4& projection, QMatrix4x4& view) {
    if (drawable && visible) {
        drawable->draw(projection, view);
    }
}

bool Tile::isCollidingWith(const Tile* other) const {
    for (b2ContactEdge* edge = body->GetContactList(); edge; edge = edge->next) {
        if (edge->other == other->body && edge->contact && edge->contact->IsTouching()) {
            return true;
        }
    }
    return false;
}
void Tile::setVisible(bool isVisible) {
    visible = isVisible;
    body->SetEnabled(isVisible);
}
