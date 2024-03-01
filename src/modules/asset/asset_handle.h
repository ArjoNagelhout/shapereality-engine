//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_ASSET_HANDLE_H
#define SHAPEREALITY_ASSET_HANDLE_H

#include <string>

#include "asset_id.h"

namespace asset
{
    class AssetHandle final
    {
    public:
        enum class State
        {
            NotLoaded,
            Loading,
            Success,
            Error
        };

        explicit AssetHandle(AssetId assetId);

        ~AssetHandle();

        AssetId assetId;

    private:
        State state = State::NotLoaded;
    };

    using Asset = std::shared_ptr<AssetHandle>;
}

#endif //SHAPEREALITY_ASSET_HANDLE_H
