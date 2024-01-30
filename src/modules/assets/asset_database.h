//
// Created by Arjo Nagelhout on 30/01/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include <filesystem>

namespace assets
{
    using AssetId = size_t;

    // asset database allows retrieving
    // todo: refactor to have a distinction between
    // runtime asset importing and compile time (editing time)
    class AssetDatabase
    {
    public:
        explicit AssetDatabase();

        ~AssetDatabase();

        // retrieve an asset
        // if it was not already loaded in memory
        [[nodiscard]] int getAsset(AssetId id);

        // creates an asynchronous operation that executes the lambda on completion
        void getAssetAsync(std::function<void()> onComplete);

    private:
        // this is the place where sources are stored
        std::filesystem::path sourceDirectory;

        // caches are stored
        std::filesystem::path cacheDirectory;

        // assets

    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
