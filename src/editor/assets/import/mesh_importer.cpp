//
// Created by Arjo Nagelhout on 22/12/2023.
//

#include "mesh_importer.h"

#define CGLTF_IMPLEMENTATION

#include "cgltf.h"

#include <iostream>

namespace assets
{
	[[nodiscard]] std::string toString(cgltf_primitive_type value)
	{
		switch (value)
		{
			case cgltf_primitive_type_invalid:
				return "cgltf_primitive_type_invalid";
			case cgltf_primitive_type_points:
				return "cgltf_primitive_type_points";
			case cgltf_primitive_type_lines:
				return "cgltf_primitive_type_lines";
			case cgltf_primitive_type_line_loop:
				return "cgltf_primitive_type_line_loop";
			case cgltf_primitive_type_line_strip:
				return "cgltf_primitive_type_line_strip";
			case cgltf_primitive_type_triangles:
				return "cgltf_primitive_type_triangles";
			case cgltf_primitive_type_triangle_strip:
				return "cgltf_primitive_type_triangle_strip";
			case cgltf_primitive_type_triangle_fan:
				return "cgltf_primitive_type_triangle_fan";
			case cgltf_primitive_type_max_enum:
				return "cgltf_primitive_type_max_enum";
		}
	}

	[[nodiscard]] std::string toString(cgltf_attribute_type value)
	{
		switch (value)
		{
			case cgltf_attribute_type_invalid: return "cgltf_attribute_type_invalid";
			case cgltf_attribute_type_position: return "cgltf_attribute_type_position";
			case cgltf_attribute_type_normal: return "cgltf_attribute_type_normal";
			case cgltf_attribute_type_tangent: return "cgltf_attribute_type_tangent";
			case cgltf_attribute_type_texcoord: return "cgltf_attribute_type_texcoord";
			case cgltf_attribute_type_color: return "cgltf_attribute_type_color";
			case cgltf_attribute_type_joints: return "cgltf_attribute_type_joints";
			case cgltf_attribute_type_weights: return "cgltf_attribute_type_weights";
			case cgltf_attribute_type_custom: return "cgltf_attribute_type_custom";
			case cgltf_attribute_type_max_enum: return "cgltf_attribute_type_max_enum";
		}
	}

	std::unique_ptr<renderer::Mesh>
	importMesh(std::filesystem::path const& source, MeshImportDescriptor const& descriptor, graphics::IDevice* pDevice)
	{
		cgltf_options options = {
			.type = cgltf_file_type_gltf,
			.file = cgltf_file_options{

			},
		};
		cgltf_data* data = nullptr;
		cgltf_result parseFileResult = cgltf_parse_file(&options, source.c_str(), &data);
		if (parseFileResult != cgltf_result_success)
		{
			cgltf_free(data);
			return nullptr;
		}

		cgltf_result loadBuffersResult = cgltf_load_buffers(&options, data, source.c_str());
		if (loadBuffersResult != cgltf_result_success)
		{
			cgltf_free(data);
			return nullptr;
		}

		for (cgltf_size i = 0; i < data->meshes_count; i++)
		{
			cgltf_mesh mesh = data->meshes[i];
			std::cout << "mesh: " << mesh.name << std::endl;

			for (cgltf_size j = 0; j < mesh.primitives_count; j++)
			{
				cgltf_primitive primitive = mesh.primitives[j];
				std::cout << "primitive: " << toString(primitive.type) << std::endl;

				cgltf_accessor* indicesAccessor = primitive.indices;

				// we need to go over each vertex attribute
				for (cgltf_size k = 0; k < primitive.attributes_count; k++)
				{
					cgltf_attribute attribute = primitive.attributes[k];
					std::cout << "attribute: " << toString(attribute.type) << std::endl;
				}
			}
		}

		cgltf_free(data);

		return std::make_unique<renderer::Mesh>(pDevice);
	}
}