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
    struct GltfImportParameters
    {
        renderer::VertexAttribute_ vertexAttributesToImport = renderer::VertexAttribute_All;
    };

    [[nodiscard]] asset::ImportResult importGltfNew(asset::AssetDatabase& assetDatabase, std::filesystem::path const& inputFile);
}

#endif //SHAPEREALITY_IMPORT_GLTF_H
