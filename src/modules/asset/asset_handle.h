//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_ASSET_HANDLE_H
#define SHAPEREALITY_ASSET_HANDLE_H

#include <string>

#include <common/result.h>

#include <renderer/mesh.h>
#include <graphics/texture.h>
#include <scene/scene.h>

#include "asset_id.h"

namespace asset
{
    /**
     * To support storing asset handles of different types
     * inside a container such as a std::vector, we create the
     * type AssetHandleBase
     */
    class AssetHandleBase
    {
    public:
        explicit AssetHandleBase(AssetId id);

        ~AssetHandleBase();

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

    template<typename Type>
    class AssetHandle : public AssetHandleBase
    {
    public:
        template<typename... Args>
        explicit AssetHandle(AssetId id, Args&&... args)
            : AssetHandleBase(std::move(id)), asset(std::forward<Args>(args)...)
        {

        }

        ~AssetHandle() = default;

        // delete copy constructor and assignment operator
        AssetHandle(AssetHandle const&) = delete;

        AssetHandle& operator=(AssetHandle const&) = delete;

        [[nodiscard]] Type& get()
        {
            return *asset.get();
        }

    private:
        Type asset;
    };

    using AssetBase = std::shared_ptr<AssetHandleBase>;

    template<typename Type>
    using Asset = std::shared_ptr<AssetHandle<Type>>;

    /**
     * @tparam Type the asset type to use
     * @param args arguments for constructing the AssetHandle, this is the AssetId and
     * arguments for the constructor of the Type
     * @return a shared pointer to the constructed asset
     */
    template<typename Type, typename... Args>
    Asset<Type> makeAsset(Args&&... args)
    {
        return std::make_shared<AssetHandle<Type>>(std::forward<Args>(args)...);
    }

    // example handles, might need to be removed from this file as the asset database
    // should be generic and not know about the different asset types.
    using MeshHandle = AssetHandle<renderer::Mesh_>;
    using TextureHandle = AssetHandle<graphics::ITexture>;
    using SceneHandle = AssetHandle<scene::Scene>;

    // todo: move to the right file
    constexpr std::string_view kAssetFileExtensionMesh = "mesh";
    constexpr std::string_view kAssetFileExtensionTexture = "texture";
    constexpr std::string_view kAssetFileExtensionScene = "scene";
}

#endif //SHAPEREALITY_ASSET_HANDLE_H
