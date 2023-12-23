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
			case cgltf_attribute_type_invalid:
				return "cgltf_attribute_type_invalid";
			case cgltf_attribute_type_position:
				return "cgltf_attribute_type_position";
			case cgltf_attribute_type_normal:
				return "cgltf_attribute_type_normal";
			case cgltf_attribute_type_tangent:
				return "cgltf_attribute_type_tangent";
			case cgltf_attribute_type_texcoord:
				return "cgltf_attribute_type_texcoord";
			case cgltf_attribute_type_color:
				return "cgltf_attribute_type_color";
			case cgltf_attribute_type_joints:
				return "cgltf_attribute_type_joints";
			case cgltf_attribute_type_weights:
				return "cgltf_attribute_type_weights";
			case cgltf_attribute_type_custom:
				return "cgltf_attribute_type_custom";
			case cgltf_attribute_type_max_enum:
				return "cgltf_attribute_type_max_enum";
		}
	}

	MeshImportResult importMesh(graphics::IDevice* pDevice,
								std::filesystem::path const& source,
								MeshImportDescriptor const& descriptor,
								std::vector<std::unique_ptr<renderer::Mesh>>& outMeshes)
	{
		// parse file
		cgltf_options options = {
			.type = cgltf_file_type_gltf,
			.file = cgltf_file_options{}
		};
		cgltf_data* data = nullptr;
		cgltf_result parseFileResult = cgltf_parse_file(&options, source.c_str(), &data);
		if (parseFileResult != cgltf_result_success)
		{
			cgltf_free(data);
			return MeshImportResult{.success = false};
		}

		// load buffers
		cgltf_result loadBuffersResult = cgltf_load_buffers(&options, data, source.c_str());
		if (loadBuffersResult != cgltf_result_success)
		{
			cgltf_free(data);
			return MeshImportResult{.success = false};
		}

		// go over all meshes
		for (cgltf_size meshIndex = 0; meshIndex < data->meshes_count; meshIndex++)
		{
			cgltf_mesh mesh = data->meshes[meshIndex];
			std::cout << "mesh: " << mesh.name << std::endl;

			// each mesh has multiple primitives.
			// a primitive exists because one primitive can only be assigned one material.
			// when exporting from blender for example, a mesh with multiple materials applied
			// to different parts of that mesh, will create a primitive for each material.

			for (cgltf_size primitiveIndex = 0; primitiveIndex < mesh.primitives_count; primitiveIndex++)
			{
				cgltf_primitive primitive = mesh.primitives[primitiveIndex];
				std::cout << "primitive: " << toString(primitive.type) << std::endl;
				std::cout << "count:" << primitive.attributes[0].data->count << std::endl;
				cgltf_size verticesCount = primitive.attributes[0].data->count; // we assume each attribute has the same length, this might not always be the case.

				// create the output vertex data
				std::vector<renderer::VertexData> vertices(verticesCount);

				// we need to go over each vertex attribute
				for (cgltf_size attributeIndex = 0; attributeIndex < primitive.attributes_count; attributeIndex++)
				{
					cgltf_attribute attribute = primitive.attributes[attributeIndex];
					cgltf_size componentsCount = cgltf_num_components(attribute.data->type);

					std::cout << "attribute: " << toString(attribute.type) << std::endl;
					std::cout << "num components: " << componentsCount << std::endl;

					// get the buffer of this vertex attribute
					for (cgltf_size index = 0; index < verticesCount; index++)
					{
						cgltf_float out[componentsCount];
						cgltf_accessor_read_float(attribute.data, index, out, componentsCount);
						switch (attribute.type)
						{
							case cgltf_attribute_type_invalid:
								break;
							case cgltf_attribute_type_position:
								vertices[index].position = math::vec3{{out[0], out[1], out[2]}};
								break;
							case cgltf_attribute_type_normal:
								vertices[index].normal = math::vec3{{out[0], out[1], out[2]}};
								break;
							case cgltf_attribute_type_tangent:
								break;
							case cgltf_attribute_type_texcoord:
								vertices[index].uv0 = math::vec2{{out[0], out[1]}};
								break;
							case cgltf_attribute_type_color:
							case cgltf_attribute_type_joints:
							case cgltf_attribute_type_weights:
							case cgltf_attribute_type_custom:
							case cgltf_attribute_type_max_enum:
								break;
						}
					}
				}

				// create output indices
				cgltf_size indicesCount = primitive.indices->count;
				std::vector<renderer::index_type> indices(indicesCount);

				for (cgltf_size index = 0; index < indicesCount; index++)
				{
					cgltf_size value = cgltf_accessor_read_index(primitive.indices, index);
					indices[index] = value;
				}

				// for now create a mesh for each primitive
				outMeshes.emplace_back(std::make_unique<renderer::Mesh>(pDevice, vertices, indices));

				// we should implement a way for meshes to have a variable amount of attributes, depending on
				// whether they are active. For now, we just implement position, normal and uv

				// for now, we have to convert struct of arrays to array of structs, which can be done by
				// iterating over the entire length

				// let's just do a crude implementation first
			}
		}

		cgltf_free(data);

		return MeshImportResult{.success = true};
	}
}