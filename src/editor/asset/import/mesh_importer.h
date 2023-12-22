//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_MESH_IMPORTER_H
#define SHAPEREALITY_MESH_IMPORTER_H

#include <filesystem>

#include "renderer/mesh.h"

namespace asset
{
	struct MeshImportDescriptor
	{

	};

	// RAII mesh importer.
	// The memory will be deallocated when the MeshImporter is destroyed.
	class MeshImporter final
	{
	public:
		/**
		 * @param source absolute file path from which to load the mesh
		 * @param descriptor parameters for importing the mesh
		 */
		explicit MeshImporter(std::filesystem::path source, MeshImportDescriptor const& descriptor);
		~MeshImporter();

		// get the mesh
		[[nodiscard]] std::unique_ptr<renderer::Mesh> getMesh() const;
	}
}

#endif //SHAPEREALITY_MESH_IMPORTER_H
