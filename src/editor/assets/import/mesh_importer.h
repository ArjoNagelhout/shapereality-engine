//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_MESH_IMPORTER_H
#define SHAPEREALITY_MESH_IMPORTER_H

#include <filesystem>

#include "renderer/mesh.h"

namespace assets
{
	struct MeshImportDescriptor
	{

	};

	// how do we handle texture importing? do import a "model" and then import the textures
	[[nodiscard]] std::unique_ptr<renderer::Mesh>
	importMesh(std::filesystem::path const& source, MeshImportDescriptor const& descriptor, graphics::IDevice* pDevice);
}

#endif //SHAPEREALITY_MESH_IMPORTER_H
