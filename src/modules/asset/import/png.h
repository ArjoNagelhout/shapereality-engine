//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_PNG_H
#define SHAPEREALITY_PNG_H

#include <asset/asset_id.h>
#include <asset/import_registry.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace asset
{
    [[nodiscard]] std::vector<Asset> importPng(fs::path const& absolutePath);
}

#endif //SHAPEREALITY_PNG_H
