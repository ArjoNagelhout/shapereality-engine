//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>

namespace asset
{
    AssetHandle::AssetHandle() = default;

    AssetHandle::~AssetHandle() = default;

    AssetDatabase::AssetDatabase(fs::path _inputDirectory, fs::path _loadDirectory)
        : inputDirectory(std::move(_inputDirectory)), loadDirectory(std::move(_loadDirectory))
    {
        std::cout << "created asset database with \n\tinput directory: "
                  << inputDirectory
                  << "\n\tload directory: "
                  << loadDirectory
                  << std::endl;
    }

    AssetDatabase::~AssetDatabase() = default;

    AssetHandle AssetDatabase::get(AssetId const& id)
    {
        return AssetHandle{};
    }
}