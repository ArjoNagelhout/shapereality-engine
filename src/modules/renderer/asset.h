//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_RENDERER_ASSET_H
#define SHAPEREALITY_RENDERER_ASSET_H

#include <asset/asset_handle.h>

namespace renderer
{
    using MeshHandle = asset::AssetHandle<renderer::Mesh_>;
    using TextureHandle = asset::AssetHandle<graphics::ITexture>;
    using SceneHandle = asset::AssetHandle<scene::Scene>;

    constexpr std::string_view kAssetFileExtensionMesh = "mesh";
    constexpr std::string_view kAssetFileExtensionTexture = "texture";
    constexpr std::string_view kAssetFileExtensionScene = "scene";
}

#endif //SHAPEREALITY_RENDERER_ASSET_H
