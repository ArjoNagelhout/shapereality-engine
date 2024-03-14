//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_ASSET_HANDLE_H
#define SHAPEREALITY_ASSET_HANDLE_H

#include <string>

#include <common/result.h>

#include "asset_id.h"

namespace asset
{
    class AssetHandle
    {
    public:
        explicit AssetHandle(AssetId id);

        ~AssetHandle();

        [[nodiscard]] AssetId const& id() const;

        [[nodiscard]] bool completed() const;

        [[nodiscard]] bool success() const;

        [[nodiscard]] bool error() const;

        [[nodiscard]] common::ResultCode code() const;

    private:
        AssetId id_;
        bool completed_{};
        common::ResultCode code_{};
    };

    class MeshHandle final : public AssetHandle
    {

    };

    class TextureHandle final : public AssetHandle
    {

    };

    class SceneHandle final : public AssetHandle
    {

    };

    using Asset = std::shared_ptr<AssetHandle>;
}

#endif //SHAPEREALITY_ASSET_HANDLE_H
