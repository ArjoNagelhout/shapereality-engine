//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_ASSET_INFO_H
#define SHAPEREALITY_ASSET_INFO_H

#include <string>

namespace asset
{
    /*
     * data for deserializing and serializing a specific asset
     * type. gets registered in a `AssetInfoRegistry`
     */
    struct AssetInfo
    {
        std::string fileExtension;
    };
}

#endif //SHAPEREALITY_ASSET_INFO_H
