//
// Created by Arjo Nagelhout on 27/12/2023.
//

#ifndef SHAPEREALITY_MESH_RENDERER_H
#define SHAPEREALITY_MESH_RENDERER_H

#include "math/matrix.h"
#include "math/vector.h"
#include "math/quaternion.h"

namespace renderer
{
    class Mesh;

    class Material;

    struct MeshRendererComponent final
    {
        Mesh* pMesh; // unowned pointer
        Material* pMaterial; // unowned pointer
    };
}

#endif //SHAPEREALITY_MESH_RENDERER_H
