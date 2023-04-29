#include "tile.h"

Tile::Tile(Drawable* drawable, b2World* world, const b2Vec2& pos, const b2Vec2& size)
    : drawable(drawable) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(size.x / 2, size.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
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

void Tile::update(float deltaTime) {
    qDebug() << "tile base update";
    if (drawable) {
        qDebug() << "Change Pos";
        drawable->setPosition(QVector3D(body->GetPosition().x, body->GetPosition().y, 0));
    }
}

void Tile::draw(QMatrix4x4& projection, QMatrix4x4& view) {
    if (drawable) {
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
