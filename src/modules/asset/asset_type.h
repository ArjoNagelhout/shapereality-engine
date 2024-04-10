//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_ASSET_TYPE_H
#define SHAPEREALITY_ASSET_TYPE_H

#include <string>
#include <fmt/format.h>

namespace asset
{
    /*
     * data for deserializing and serializing a specific asset
     * type. gets registered in a `AssetInfoRegistry`
     */
    struct AssetType
    {
        std::string const fileExtension;
    };
}

#endif //SHAPEREALITY_ASSET_TYPE_H
