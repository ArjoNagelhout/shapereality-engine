//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_IMPORT_GLTF_H
#define SHAPEREALITY_IMPORT_GLTF_H

#include <filesystem>

#include <renderer/mesh.h>
#include <asset/import_registry.h>

namespace import_::gltf
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
    [[nodiscard]] asset::ImportResult importGltfNew(asset::AssetDatabase& assetDatabase, std::filesystem::path const& inputFile);
}

#endif //SHAPEREALITY_IMPORT_GLTF_H
