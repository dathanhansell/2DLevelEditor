#include "box2dworld.h"

Box2DWorld::Box2DWorld()
{
    b2Vec2 gravity(0.0f, -9.81f);
    m_world = new b2World(gravity);
    m_selectedBlockType = 0;
}

void Box2DWorld::setSelectedBlockType(int blockType)
{
    m_selectedBlockType = blockType;
}
