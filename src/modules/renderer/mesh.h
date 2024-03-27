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
        VertexAttributeType_All = (1 << 7) - 1
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
     * Data type used for components or the index buffer
     */
    enum class StorageType
    {
        SignedByte = 0, // 8 bits, 1 byte
        UnsignedByte, // 8 bits, 1 byte
        SignedShort, // 16 bits, 2 bytes
        UnsignedShort, // 16 bits, 2 bytes
        UnsignedInt, // 32 bits, 4 bytes
        Float // 32 bits, 4 bytes
    };

    // get the stride for a given component type (in bytes)
    [[nodiscard]] size_t stride(StorageType componentType);

    struct VertexAttributeDescriptor_
    {
        VertexAttributeType_ type = VertexAttributeType_Position;
        size_t index = 0;
        StorageType componentType = StorageType::Float;
    };

    struct MeshDescriptor_
    {
        // mesh type
        graphics::PrimitiveType primitiveType;

        // vertices
        std::vector<VertexAttributeDescriptor_> vertexAttributes;
        size_t vertexCount = 0;

        // indices
        bool hasIndexBuffer = false; // if this is set to true, indexData should not be nullptr and indexCount should be set.
        size_t indexCount = 0;
        StorageType indexType = StorageType::UnsignedInt; // UnsignedInt = 32 bits, 2^16 only supports 65.536 indices

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
        explicit Mesh_(graphics::IDevice* device);

        // construct mesh without vertex or index data provided yet, can be supplied later using the setXXX() methods
        explicit Mesh_(graphics::IDevice* device, MeshDescriptor_ descriptor);

        // construct mesh from memory that already contains the different attributes sequentially
        explicit Mesh_(graphics::IDevice* device, MeshDescriptor_ descriptor, void* vertexData, void* indexData = nullptr);

        // construct mesh from individual pieces of memory that contain the different attributes separately
        explicit Mesh_(graphics::IDevice* device, MeshDescriptor_ descriptor, std::vector<void*> attributesData, void* indexData = nullptr);

        ~Mesh_();

        // clears the buffers and resets the descriptor to represent an empty mesh
        // does not reset any of the other MeshDescriptor parameters like its PrimitiveType or whether it's writable
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
        [[nodiscard]] bool setAttributeData(VertexAttributeType_ attribute, void* data, size_t index = 0);

        // set the different vertex attributes from different memory locations individually
        [[nodiscard]] bool setAttributesData(std::vector<void*> attributesData);

        // set the entire vertex buffer at once
        void setVertexData(void* vertexData);

        // set the index buffer data
        void setIndexData(void* indexData);

        // upload to GPU. either gets done lazily on retrieving the vertex and index buffers (done by the MeshRenderer)
        // or can be done manually. If writable
        void uploadToGPU();

    private:
        graphics::IDevice* device;

        // description of what is inside the buffers, e.g. vertex count and the primitive type
        MeshDescriptor_ descriptor;

        // buffers
        std::unique_ptr<graphics::IBuffer> vertexBuffer;
        std::unique_ptr<graphics::IBuffer> indexBuffer;
    };
}

#endif //SHAPEREALITY_MESH_H
