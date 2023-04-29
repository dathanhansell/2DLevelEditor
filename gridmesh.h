// cube.h
#ifndef GRIDMESH_H
#define GRIDMESH_H

#include "mesh.h"

class GridMesh : public Mesh
{
public:
    GridMesh();
    ~GridMesh();
    void draw() override;

protected:
    void setupGeometry() override;
};

#endif // CUBE_H
