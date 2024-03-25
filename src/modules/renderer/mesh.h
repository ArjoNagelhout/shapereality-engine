//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_MESH_H
#define SHAPEREALITY_MESH_H

#include "graphics/device.h"
#include "graphics/buffer.h"

#include "math/vector.h"

#include <vector>

namespace renderer
{
    using index_type = uint32_t;

    // warning: MSL float3 has size 4*4 bytes, instead of 4*3 bytes,
    // so we need to use packed_float3 inside the shader
    struct VertexData
    {
        math::Vector3 position;
        math::Vector3 normal;
        math::Vector2 uv0;
    };

    enum class VertexAttribute
    {
        Position = 0,
        Normal,
        Tangent,
        TextureCoordinate, // also known as UV
        Color,
        Joints,
        Weights,
        Custom
    };

    // currently, mesh is an immutable piece of data.
    class Mesh
    {
    public:
        explicit Mesh(graphics::IDevice* device, std::vector<VertexData> const& verticesData,
                      std::vector<index_type> const& indices);

        ~Mesh();

        [[nodiscard]] graphics::IBuffer* getVertexBuffer() const;

        [[nodiscard]] graphics::IBuffer* getIndexBuffer() const;

        [[nodiscard]] size_t getIndexCount() const;

    private:
        std::unique_ptr<graphics::IBuffer> vertexBuffer;
        std::unique_ptr<graphics::IBuffer> indexBuffer;
        unsigned int indexCount;
    };

    struct VertexAttributeDescriptor_
    {
        VertexAttribute type = VertexAttribute::Position;
        size_t index = 0;
        size_t stride = 0;
        void* data = nullptr;
    };

    struct MeshDescriptor_
    {
        // mesh type
        graphics::PrimitiveType primitiveType;

        // vertices
        std::vector<VertexAttributeDescriptor_> vertexAttributes;
        size_t vertexCount = 0;
        size_t alignment = 0; // alignment to use for vertex attributes, e.g. if set to 64, and the sum of strides of each attribute is 72, the resulting total stride of one vertex is 128 (it rounds up)
        void* vertexData = nullptr; // if this is set, we copy the vertex data directly from here, instead of individually per vertex from the VertexAttributeDescriptor

        // indices
        bool hasIndexBuffer = false; // if this is set to true, indexData should not be nullptr and indexCount should be set.
        size_t indexCount = 0;
        void* indexData = nullptr;

        bool writable = false; // if this is set to true, we keep a copy of the mesh on the CPU that can be written to.
    };

    /**
     * Data inside a mesh is stored as a array of structures (AoS), because
     * that is how the GPU expects the memory to be laid out. However,
     * because the mesh needs to support an arbitrary amount of vertex attributes,
     * we need to manually interweave the data of these different attributes.
     *
     * Coming from GLTF for example, each vertex attribute is stored in a separate buffer (array),
     * which is why this is the API.
     *
     * When loading mesh data from disk that has already been imported, we can simply set vertexData and just copy the whole
     * buffer to the GPU.
     */
    class Mesh_
    {
    public:
        // constructs empty mesh
        explicit Mesh_();

        explicit Mesh_(graphics::IDevice* device, MeshDescriptor_ const& descriptor);

        ~Mesh_();

        //
        void clear();

        // resizes the vertex buffer to fit the vertex count
        void setVertexCount(size_t count);

        // resizes the index buffer to fit the index count
        void setIndexCount();

        /**
         * set data of the mesh for a specific vertex attribute
         * no validation is done for whether data is large enough
         *
         * @param attribute which attribute type to use
         * @param data
         * @param index e.g. whether to use UV_0 or UV_1
         * @returns whether setting the data was successful
         */
        [[nodiscard]] bool set(VertexAttribute attribute, void* data, size_t index);

        // applies changes made to the mesh to the GPU (i.e. copies the data on the CPU to the GPU)
        void apply();

    private:
        graphics::IDevice* device;

        // description of what is inside the buffers, this is used for creating the mesh
        // but also for keeping track of the mesh data representation throughout its lifecycle.
        MeshDescriptor_ descriptor;

        // buffers
        std::unique_ptr<graphics::IBuffer> vertexBuffer;
        std::unique_ptr<graphics::IBuffer> indexBuffer;

        // whether the data needs to be re-uploaded to the GPU
        bool changed = false;
    };
}

#endif //SHAPEREALITY_MESH_H
