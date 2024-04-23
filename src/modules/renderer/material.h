//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_MATERIAL_H
#define SHAPEREALITY_MATERIAL_H

#include <graphics/texture.h>
#include <graphics/shader.h>

#include "shader.h"

#include <asset/asset_handle.h>

namespace renderer
{
    struct Material
    {
        Shader* shader;

        asset::Asset texture;
    };
}

#endif //SHAPEREALITY_MATERIAL_H
