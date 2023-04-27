// cube.h
#ifndef CUBE_H
#define CUBE_H

#include "mesh.h"

class TileMesh : public Mesh
{
public:
    TileMesh();
    ~TileMesh();

protected:
    void setupGeometry() override;
};

#endif // CUBE_H
