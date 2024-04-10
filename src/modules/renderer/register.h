//
// Created by Arjo Nagelhout on 27/03/2024.
//

#ifndef SHAPEREALITY_REGISTER_RENDERER_H
#define SHAPEREALITY_REGISTER_RENDERER_H

#include <reflection/reflection.h>
#include <asset/asset_type_registry.h>

namespace renderer
{
    void register_(reflection::Reflection& reflection);

    void register_(asset::AssetTypeRegistry& assetTypes);
}

#endif //SHAPEREALITY_REGISTER_RENDERER_H
