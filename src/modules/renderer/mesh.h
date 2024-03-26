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

    enum VertexAttributeType_
    {
        VertexAttributeType_None = 0,
        VertexAttributeType_Position = 1 << 0,
        VertexAttributeType_Normal = 1 << 1,
        VertexAttributeType_Tangent = 1 << 2,
        VertexAttributeType_TextureCoordinate = 1 << 3, // also known as UV
        VertexAttributeType_Color = 1 << 4,
        VertexAttributeType_Joints = 1 << 5,
        VertexAttributeType_Weights = 1 << 6,
        VertexAttributeType_Custom = 1 << 7,
        VertexAttributeType_All = (1 << 8) - 1
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

    /**
     * Data type used
     */
    enum class ComponentType
    {
        SignedByte = 0, // 8 bits, 1 byte
        UnsignedByte, // 8 bits, 1 byte
        SignedShort, // 16 bits, 2 bytes
        UnsignedShort, // 16 bits, 2 bytes
        UnsignedInt, // 32 bits, 4 bytes
        Float // 32 bits, 4 bytes
    };

    // get the stride for a given component type (in bytes)
    [[nodiscard]] size_t stride(ComponentType componentType);

    struct VertexAttributeDescriptor_
    {
        VertexAttributeType_ type = VertexAttributeType_Position;
        size_t index = 0;
        ComponentType componentType = ComponentType::Float;
        void* data = nullptr;
    };

    struct MeshDescriptor_
    {
        // mesh type
        graphics::PrimitiveType primitiveType;

        // vertices
        VertexAttributeType_ supportedVertexAttributes = VertexAttributeType_All; // a mask for which attributes are accepted, note that setting the vertex attributes in the descriptor does not get validated, so validation is the responsibility of the user of the mesh descriptor (e.g. Mesh_)
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
     * We use non-interleaved vertex attributes.
     * All vertex attributes are stored in the same buffer, at defined offsets.
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

        /**
         * set data of the mesh for a specific vertex attribute
         * no validation is done for whether data is large enough
         *
         * @param attribute which attribute type to use
         * @param data
         * @param index e.g. whether to use UV_0 or UV_1
         * @returns whether setting the data was successful
         */
        [[nodiscard]] bool set(VertexAttributeType_ attribute, void* data, size_t index);

        // upload to GPU
        void uploadToGPU();

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
