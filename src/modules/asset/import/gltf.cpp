//
// Created by Arjo Nagelhout on 22/12/2023.
//

#include "gltf.h"

#define CGLTF_IMPLEMENTATION

#include <cgltf.h>

#include <iostream>
#include <asset/asset_database.h>
#include <common/logger.h>
#include <scene/scene.h>

namespace asset
{
    [[nodiscard]] std::string toString(cgltf_primitive_type value)
    {
        switch (value)
        {
            case cgltf_primitive_type_invalid: return "cgltf_primitive_type_invalid";
            case cgltf_primitive_type_points: return "cgltf_primitive_type_points";
            case cgltf_primitive_type_lines: return "cgltf_primitive_type_lines";
            case cgltf_primitive_type_line_loop: return "cgltf_primitive_type_line_loop";
            case cgltf_primitive_type_line_strip: return "cgltf_primitive_type_line_strip";
            case cgltf_primitive_type_triangles: return "cgltf_primitive_type_triangles";
            case cgltf_primitive_type_triangle_strip: return "cgltf_primitive_type_triangle_strip";
            case cgltf_primitive_type_triangle_fan: return "cgltf_primitive_type_triangle_fan";
            case cgltf_primitive_type_max_enum: return "cgltf_primitive_type_max_enum";
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

    GltfImportResult importGltf(graphics::IDevice* device,
                                std::filesystem::path const& source,
                                GltfImportDescriptor const& descriptor,
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
            return GltfImportResult{.success = false};
        }

        // load buffers
        cgltf_result loadBuffersResult = cgltf_load_buffers(&options, data, source.c_str());
        if (loadBuffersResult != cgltf_result_success)
        {
            cgltf_free(data);
            return GltfImportResult{.success = false};
        }

        std::cout << data->scenes_count << std::endl;

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
                            case cgltf_attribute_type_invalid:break;
                            case cgltf_attribute_type_position:
                                vertices[index].position = math::Vector3{{out[0], out[1], out[2]}};
                                break;
                            case cgltf_attribute_type_normal:
                                vertices[index].normal = math::Vector3{{out[0], out[1], out[2]}};
                                break;
                            case cgltf_attribute_type_tangent:break;
                            case cgltf_attribute_type_texcoord:vertices[index].uv0 = math::Vector2{{out[0], out[1]}};
                                break;
                            case cgltf_attribute_type_color:
                            case cgltf_attribute_type_joints:
                            case cgltf_attribute_type_weights:
                            case cgltf_attribute_type_custom:
                            case cgltf_attribute_type_max_enum:break;
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
                outMeshes.emplace_back(std::make_unique<renderer::Mesh>(device, vertices, indices));

                // we should implement a way for meshes to have a variable amount of attributes, depending on
                // whether they are active. For now, we just implement position, normal and uv

                // for now, we have to convert struct of arrays to array of structs, which can be done by
                // iterating over the entire length

                // let's just do a crude implementation first
            }
        }

        cgltf_free(data);

        return GltfImportResult{.success = true};
    }

    [[nodiscard]] renderer::VertexAttributeType_ convert(cgltf_attribute_type type)
    {
        switch (type)
        {
            case cgltf_attribute_type_position: return renderer::VertexAttributeType_Position;
            case cgltf_attribute_type_normal: return renderer::VertexAttributeType_Normal;
            case cgltf_attribute_type_tangent: return renderer::VertexAttributeType_Tangent;
            case cgltf_attribute_type_texcoord: return renderer::VertexAttributeType_TextureCoordinate;
            case cgltf_attribute_type_color: return renderer::VertexAttributeType_Color;
            case cgltf_attribute_type_joints: return renderer::VertexAttributeType_Joints;
            case cgltf_attribute_type_weights: return renderer::VertexAttributeType_Weights;
            default: return renderer::VertexAttributeType_None;
        }
    }

    [[nodiscard]] renderer::ComponentType convert(cgltf_component_type type)
    {
        switch (type)
        {
            case cgltf_component_type_r_8: return renderer::ComponentType::SignedByte;
            case cgltf_component_type_r_8u: return renderer::ComponentType::UnsignedByte;
            case cgltf_component_type_r_16: return renderer::ComponentType::SignedShort;
            case cgltf_component_type_r_16u: return renderer::ComponentType::UnsignedShort;
            case cgltf_component_type_r_32u: return renderer::ComponentType::UnsignedInt;
            case cgltf_component_type_r_32f: return renderer::ComponentType::Float;
            default: return renderer::ComponentType::Float;
        }
    }

    ImportResult importGltfNew(AssetDatabase& assets, std::filesystem::path const& inputFile)
    {
        std::filesystem::path const path = assets.absolutePath(inputFile);
        std::vector<AssetBase> results;

        GltfImportParameters importParameters;

        // parse file
        cgltf_options options = {
            .type = cgltf_file_type_gltf,
            .file = cgltf_file_options{}
        };
        cgltf_data* data = nullptr;
        cgltf_result parseFileResult = cgltf_parse_file(&options, path.c_str(), &data);
        if (parseFileResult != cgltf_result_success)
        {
            cgltf_free(data);
            return ImportResult::makeError(common::ResultCode::Cancelled, "Failed to parse gltf file");
        }

        // load buffers
        cgltf_result loadBuffersResult = cgltf_load_buffers(&options, data, path.c_str());
        if (loadBuffersResult != cgltf_result_success)
        {
            cgltf_free(data);
            return ImportResult::makeError(common::ResultCode::Cancelled,
                                           "Failed to load buffers, this can be due to .bin files not being located next to the file");
        }

        common::log::infoDebug("gltf data:\n"
                               "\tscenes_count = {}\n"
                               "\tmeshes_count = {}\n"
                               "\tmaterials_count = {}\n"
                               "\ttextures_count = {}\n" // holds references to images and samplers
                               "\timages_count = {}", // refers to actual image file paths
                               data->scenes_count,
                               data->meshes_count,
                               data->materials_count,
                               data->textures_count,
                               data->images_count);

        for (size_t i = 0; i < data->images_count; i++)
        {
            cgltf_image& image = data->images[i];
            common::log::infoDebug("image {}: \n"
                                   "\tname = {}\n"
                                   "\tmime_type = {}\n"
                                   "\turi = {}",
                                   i,
                                   image.name ? image.name : "",
                                   image.mime_type ? image.mime_type : "",
                                   image.uri);

            std::filesystem::path imagePath = image.uri;
            if (imagePath.is_relative()) // i.e. does not contain root directory
            {
                // resolve file path
                imagePath = inputFile.parent_path() / imagePath;
            }
            common::log::infoDebug("resolved image path: {}", imagePath.string());

            AssetId id = {
                .inputFilePath = imagePath,
                .artifactPath = {} // empty artifact path means we just take the first / main generated artifact
            };
        }

        // meshes
        for (size_t i = 0; i < data->meshes_count; i++)
        {
            cgltf_mesh& mesh = data->meshes[i];
            common::log::infoDebug("mesh {}: \n"
                                   "\tname = {}\n"
                                   "\tprimitives_count = {}\n"
                                   "\textensions_count = {}\n"
                                   "\tweights_count = {}\n"
                                   "\ttarget_names_count = {}",
                                   i,
                                   mesh.name,
                                   mesh.primitives_count,
                                   mesh.extensions_count,
                                   mesh.weights_count,
                                   mesh.target_names_count);

            for (size_t j = 0; j < mesh.primitives_count; j++)
            {
                cgltf_primitive& primitive = mesh.primitives[j];
                common::log::infoDebug("mesh {} primitive {}: \n"
                                       "\ttype = {}\n"
                                       "\tattributes_count = {}\n"
                                       "\ttargets_count = {}",
                                       i,
                                       j,
                                       toString(primitive.type),
                                       primitive.attributes_count,
                                       primitive.targets_count
                );

                renderer::MeshDescriptor_ outMeshDescriptor;
                outMeshDescriptor.vertexAttributes.reserve(primitive.attributes_count);

                for (size_t k = 0; k < primitive.attributes_count; k++)
                {
                    cgltf_attribute& attribute = primitive.attributes[k];
                    common::log::infoDebug("mesh {} primitive {} attribute {}: \n"
                                           "\tname = {}\n"
                                           "\ttype = {}\n"
                                           "\tindex = {}",
                                           i,
                                           j,
                                           k,
                                           attribute.name,
                                           toString(attribute.type),
                                           attribute.index);

                    renderer::VertexAttributeType_ type = convert(attribute.type);
                    if (type == renderer::VertexAttributeType_None)
                    {
                        // invalid, don't use this attribute
                        continue;
                    }

                    if ((type & importParameters.vertexAttributesToImport) == 0)
                    {
                        // attribute not supported as dictated by the import parameters
                        continue;
                    }

                    // determine whether we have to convert the data to our own engine's format or whether it is already in the
                    // desired format
                    cgltf_accessor* a = attribute.data;

                    // we need to convert the data to our own representation.
                    // however, we need to determine first whether we want to support

                    renderer::VertexAttributeDescriptor_ outAttribute{
                        .type = convert(attribute.type),
                        .index = static_cast<size_t>(attribute.index),
                        .componentType = convert(a->component_type)
                    };

                    common::log::infoDebug("type: {}, index: {}, componentType: {}",
                                           reflection::enumToString(outAttribute.type),
                                           outAttribute.index,
                                           reflection::enumToString(outAttribute.componentType));

                    outMeshDescriptor.vertexAttributes.emplace_back(outAttribute);
                }

                Asset<renderer::Mesh_> outMesh = makeAsset<renderer::Mesh_>(
                    AssetId{inputFile, fmt::format("{}.{}", mesh.name, kAssetFileExtensionMesh)},
                    nullptr,
                    outMeshDescriptor);
                results.emplace_back(std::move(outMesh));
            }
        }

        for (size_t i = 0; i < data->materials_count; i++)
        {

        }

        for (size_t i = 0; i < data->scenes_count; i++)
        {
            // create scenes
            cgltf_scene& scene = data->scenes[i];
            common::log::infoDebug("scene {}: \n"
                                   "\tname = {}\n"
                                   "\tnodes_count = {}\n"
                                   "\textensions_count = {}",
                                   i,
                                   scene.name,
                                   scene.nodes_count,
                                   scene.extensions_count);

            for (size_t j = 0; j < scene.extensions_count; j++)
            {
                cgltf_extension& extension = scene.extensions[j];
                common::log::infoDebug("scene {} extension {}: \n"
                                       "\tname = {}\n"
                                       "\tdata = {}",
                                       i,
                                       j,
                                       extension.name,
                                       extension.data);
            }

            //scene::Scene s;
            //s.name = scene.name;
        }

        results.emplace_back(makeAsset<DummyAsset>(AssetId{inputFile, "first_asset.dummy"}, "a first asset"));
        results.emplace_back(makeAsset<DummyAsset>(AssetId{inputFile, "second_asset.dummy"}, "a second asset"));
        results.emplace_back(makeAsset<DummyAsset>(AssetId{inputFile, "third_asset.dummy"}, "a third asset"));
        return ImportResult::makeSuccess(std::move(results));
    }

    DummyAsset::DummyAsset(std::string name) : name_(std::move(name))
    {

    }

    DummyAsset::~DummyAsset() = default;

    std::string_view DummyAsset::name() const
    {
        return name_;
    }
}