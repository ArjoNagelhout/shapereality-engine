//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "png.h"

#include <iostream>
#include <asset/asset_database.h>

namespace asset
{
    std::vector<Asset> importPng(AssetDatabase& assets, fs::path const& inputFile)
    {
        std::cout << "import png file from " << assets.absolutePath(inputFile) << std::endl;
        return {
            std::make_shared<AssetHandle>(AssetId{.inputFilePath = inputFile, .artifactPath = "artifact1.sr_texture"}),
            std::make_shared<AssetHandle>(AssetId{.inputFilePath = inputFile, .artifactPath = "artifact2.sr_texture"}),
            std::make_shared<AssetHandle>(AssetId{.inputFilePath = inputFile, .artifactPath = "artifact3.sr_mesh"}),
            std::make_shared<AssetHandle>(AssetId{.inputFilePath = inputFile, .artifactPath = "artifact4.sr_scene"})
        };
    }
}