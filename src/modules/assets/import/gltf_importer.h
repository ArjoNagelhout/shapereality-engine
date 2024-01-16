//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_GLTF_IMPORTER_H
#define SHAPEREALITY_GLTF_IMPORTER_H

#include <filesystem>

#include "renderer/mesh.h"

namespace assets
{
    // metadata for how to import the gltf object hierarchy
    struct GltfImportDescriptor
    {

    };

    struct GltfImportResult
    {
        bool success;
    };

    // how do we handle texture importing? do import a "model" and then import the textures
    [[nodiscard]] GltfImportResult importGltf(graphics::IDevice* pDevice,
                                              std::filesystem::path const& source,
                                              GltfImportDescriptor const& descriptor,
                                              std::vector<std::unique_ptr<renderer::Mesh>>& outMeshes);
}

#endif //SHAPEREALITY_GLTF_IMPORTER_H