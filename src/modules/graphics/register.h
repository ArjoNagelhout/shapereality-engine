//
// Created by Arjo Nagelhout on 11/04/2024.
//

#ifndef SHAPEREALITY_REGISTER_GRAPHICS_H
#define SHAPEREALITY_REGISTER_GRAPHICS_H

#include <reflection/reflection.h>
#include <asset/asset_type_registry.h>

namespace graphics
{
    void register_(reflection::Reflection& reflection);

    void register_(asset::AssetTypeRegistry& assetTypes);
}

#endif //SHAPEREALITY_REGISTER_GRAPHICS_H
