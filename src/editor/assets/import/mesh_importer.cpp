//
// Created by Arjo Nagelhout on 22/12/2023.
//

#include "mesh_importer.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

namespace assets
{
	std::unique_ptr<renderer::Mesh>
	importMesh(std::filesystem::path const& source, MeshImportDescriptor const& descriptor, graphics::IDevice* pDevice)
	{
		cgltf_options options = {
			.type = cgltf_file_type_gltf,
			.file = cgltf_file_options{

			},
		};
		cgltf_data* data = nullptr;
		cgltf_result result = cgltf_parse_file(&options, source.c_str(), &data);
		if (result == cgltf_result_success)
		{
			/* TODO make awesome stuff */
			cgltf_free(data);
		}

		return std::make_unique<renderer::Mesh>(pDevice);
	}
}