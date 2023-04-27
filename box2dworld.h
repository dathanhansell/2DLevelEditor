#ifndef BOX2DWORLD_H
#define BOX2DWORLD_H

#include <box2d.h>

class Box2DWorld
{
public:
    Box2DWorld();
    void setSelectedBlockType(int blockType);

private:
    b2World *m_world;
    int m_selectedBlockType;
};

#endif // BOX2DWORLD_H
