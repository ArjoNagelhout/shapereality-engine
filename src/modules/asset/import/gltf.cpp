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

    [[nodiscard]] renderer::VertexAttribute_ convert(cgltf_attribute_type type)
    {
        switch (type)
        {
            case cgltf_attribute_type_position: return renderer::VertexAttribute_Position;
            case cgltf_attribute_type_normal: return renderer::VertexAttribute_Normal;
            case cgltf_attribute_type_tangent: return renderer::VertexAttribute_Tangent;
            case cgltf_attribute_type_texcoord: return renderer::VertexAttribute_TextureCoordinate;
            case cgltf_attribute_type_color: return renderer::VertexAttribute_Color;
            case cgltf_attribute_type_joints: return renderer::VertexAttribute_Joints;
            case cgltf_attribute_type_weights: return renderer::VertexAttribute_Weights;
            default: assert(false && "invalid vertex attribute");
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
            default: assert(false && "invalid component type");
        }
    }

    [[nodiscard]] renderer::ElementType convert(cgltf_type type)
    {
        switch (type)
        {
            case cgltf_type_scalar: return renderer::ElementType::Scalar;
            case cgltf_type_vec2: return renderer::ElementType::Vector2;
            case cgltf_type_vec3: return renderer::ElementType::Vector3;
            case cgltf_type_vec4: return renderer::ElementType::Vector4;
            case cgltf_type_mat2: return renderer::ElementType::Matrix2x2;
            case cgltf_type_mat3: return renderer::ElementType::Matrix3x3;
            case cgltf_type_mat4: return renderer::ElementType::Matrix4x4;
            default: assert(false && "invalid attribute type");
        }
    }

    ImportResult importGltfNew(AssetDatabase& assetDatabase, std::filesystem::path const& inputFile)
    {
        std::filesystem::path const path = assetDatabase.absolutePath(inputFile);
        ImportResultData result;
        graphics::IDevice* device = assetDatabase.context().device;

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

            result.dependencies.emplace_back(imagePath);
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

                renderer::MeshDescriptor outMeshDescriptor;
                outMeshDescriptor.vertexAttributes.reserve(primitive.attributes_count);
                std::vector<void*> outBuffers;

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

                    if (attribute.type == cgltf_attribute_type_invalid)
                    {

                        continue;
                    }

                    renderer::VertexAttribute_ type = convert(attribute.type);

                    if ((type & importParameters.vertexAttributesToImport) == 0)
                    {
                        // attribute not supported as dictated by the import parameters
                        continue;
                    }

                    // see https://github.com/KhronosGroup/glTF-Tutorials/blob/main/gltfTutorial/gltfTutorial_005_BuffersBufferViewsAccessors.md

                    cgltf_accessor* a = attribute.data;
                    cgltf_buffer_view* v = a->buffer_view;
                    cgltf_buffer* b = v->buffer;

                    renderer::VertexAttributeDescriptor outAttribute{
                        .index = static_cast<size_t>(attribute.index),
                        .attribute = type,
                        .elementType = convert(a->type),
                        .componentType = convert(a->component_type)
                    };

                    // the data may be stored interleaved, that is, within one buffer view, multiple accessors exist.
                    // that define different offsets
                    // the buffer view defines the stride
                    // this needs to be converted to the way we internally store our mesh: in de-interleaved format

                    // let's first do the naive way, simply allocating a buffer and copying to it using cgltf's built in method

                    assert(outAttribute.componentType == renderer::ComponentType::Float && "only float is supported now, otherwise implement own version of cgltf_accessor_unpack_floats");
                    size_t vertexCount = a->count; // validate whether this count is the same for each attribute, if not assert
                    size_t floatCount = vertexCount * renderer::componentCount(outAttribute.elementType);

                    auto* outBuffer = static_cast<cgltf_float*>(malloc(
                        floatCount *
                        renderer::stride(outAttribute.componentType)));

                    cgltf_size amountOfFloatsCopied = cgltf_accessor_unpack_floats(attribute.data, outBuffer, floatCount);
                    assert(amountOfFloatsCopied != 0 && "unable to read gltf data into buffer");

                    // handling sparse data is already handled by cgltf_accessor_unpack_floats


                    common::log::infoDebug("accessor: name: {}, count: {}, offset: {}, stride: {}, is_sparse: {}",
                                           a->name ? a->name : "null", a->count, a->offset, a->stride,
                                           a->is_sparse ? "true" : "false");
                    common::log::infoDebug("buffer view: name: {}, stride: {}, offset: {}, size: {}",
                                           v->name ? v->name : "null", v->stride, v->offset, v->size);
                    common::log::infoDebug("buffer: name: {}, uri: {}, size: {}",
                                           b->name ? b->name : "null", b->uri, b->size);

                    common::log::infoDebug("index: {}, type: {}, elementType: {}, componentType: {}",
                                           outAttribute.index,
                                           reflection::enumToString(outAttribute.attribute),
                                           reflection::enumToString(outAttribute.elementType),
                                           reflection::enumToString(outAttribute.componentType));

                    outMeshDescriptor.vertexAttributes.emplace_back(outAttribute);
                    outBuffers.emplace_back(outBuffer);
                }

                Asset<renderer::Mesh_> outMesh = makeAsset<renderer::Mesh_>(
                    AssetId{inputFile, fmt::format("{}.{}", mesh.name, kAssetFileExtensionMesh)},
                    device,
                    outMeshDescriptor,
                    outBuffers);

                // deallocate buffers. make this more intelligent by using std::unique_ptr to a CPU buffer,
                // that gets passed around and destroyed when destructed / out of scope. This way we also
                // wouldn't have to create unnecessary buffers when the data is already laid out properly.
                for (auto b: outBuffers)
                {
                    free(b);
                }

                result.artifacts.emplace_back(std::move(outMesh));
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

        result.artifacts.emplace_back(makeAsset<DummyAsset>(AssetId{inputFile, "first_asset.dummy"}, "a first asset"));
        result.artifacts.emplace_back(makeAsset<DummyAsset>(AssetId{inputFile, "second_asset.dummy"}, "a second asset"));
        result.artifacts.emplace_back(makeAsset<DummyAsset>(AssetId{inputFile, "third_asset.dummy"}, "a third asset"));
        return ImportResult::makeSuccess(std::move(result));
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