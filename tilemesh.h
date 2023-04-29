// cube.h
#ifndef TILEMESH_H
#define TILEMESH_H

#include "mesh.h"

class TileMesh : public Mesh
{
public:
    TileMesh();
    ~TileMesh();

protected:
    void setupGeometry() override;
    void draw() override;
};

#endif // CUBE_H
