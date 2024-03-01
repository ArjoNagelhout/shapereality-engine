//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_PNG_H
#define SHAPEREALITY_PNG_H

#include <asset/asset_id.h>

#include <filesystem>

namespace asset
{
    [[nodiscard]] std::vector<AssetId> importPng(std::filesystem::path const& absolutePath);
}

#endif //SHAPEREALITY_PNG_H
