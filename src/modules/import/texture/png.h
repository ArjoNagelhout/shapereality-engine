//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_IMPORT_PNG_H
#define SHAPEREALITY_IMPORT_PNG_H

#include <asset/asset_id.h>
#include <asset/import_registry.h>

#include <filesystem>

namespace import_::texture
{
    [[nodiscard]] asset::ImportResult importPng(asset::AssetDatabase& assetDatabase, std::filesystem::path const& inputFile);
}

#endif //SHAPEREALITY_IMPORT_PNG_H
