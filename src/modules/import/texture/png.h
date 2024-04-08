//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_IMPORT_PNG_H
#define SHAPEREALITY_IMPORT_PNG_H

#include <asset/asset_id.h>
#include <asset/import_registry.h>
#include <common/result.h>

#include <filesystem>

namespace asset
{
    [[nodiscard]] ImportResult importPng(AssetDatabase& assets, std::filesystem::path const& absolutePath);
}

#endif //SHAPEREALITY_IMPORT_PNG_H
