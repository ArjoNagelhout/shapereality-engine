//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_GLTF_H
#define SHAPEREALITY_GLTF_H

#include <filesystem>

#include <renderer/mesh.h>

#include <asset/import_registry.h>

namespace asset
{
    // metadata for how to import the gltf object hierarchy
    struct GltfImportDescriptor
    {

    };

    struct GltfImportResult
    {
        bool success;
    };

    struct GltfImportParameters
    {
        renderer::VertexAttribute_ vertexAttributesToImport = renderer::VertexAttribute_All;
    };

    // how do we handle texture importing? do import a "model" and then import the textures
    [[nodiscard]] GltfImportResult importGltf(graphics::IDevice* device,
                                              std::filesystem::path const& source,
                                              GltfImportDescriptor const& descriptor,
                                              std::vector<std::unique_ptr<renderer::Mesh>>& outMeshes);

    /**
     * todo: import:
     *      - skinning,
     *      - animation
     *      - lights
     *      - cameras
     *      - material variants
     */
    [[nodiscard]] ImportResult importGltfNew(AssetDatabase& assetDatabase, std::filesystem::path const& inputFile);
}

#endif //SHAPEREALITY_GLTF_H
