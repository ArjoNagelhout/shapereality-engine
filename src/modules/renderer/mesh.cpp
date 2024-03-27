#include "mesh.h"

#include "math/vector.inl"

using namespace math;

namespace renderer
{
    Mesh::Mesh(graphics::IDevice* device,
               std::vector<VertexData> const& verticesData,
               std::vector<index_type> const& indices)
        : indexCount(indices.size())
    {
        graphics::BufferDescriptor vertexBufferDescriptor{
            .storageMode = graphics::BufferDescriptor::StorageMode::Managed,
            .data = verticesData.data(),
            .length = static_cast<unsigned int>(verticesData.size() * sizeof(VertexData)),
            .stride = sizeof(VertexData)
        };
        vertexBuffer = device->createBuffer(vertexBufferDescriptor);

        graphics::BufferDescriptor indexBufferDescriptor{
            .storageMode = graphics::BufferDescriptor::StorageMode::Managed,
            .data = indices.data(),
            .length = static_cast<unsigned int>(indices.size() * sizeof(index_type)),
            .stride = sizeof(index_type)
        };
        indexBuffer = device->createBuffer(indexBufferDescriptor);
    }

    graphics::IBuffer* Mesh::getVertexBuffer() const
    {
        return vertexBuffer.get();
    }

    graphics::IBuffer* Mesh::getIndexBuffer() const
    {
        return indexBuffer.get();
    }

    size_t Mesh::getIndexCount() const
    {
        return indexCount;
    }

    size_t stride(StorageType componentType)
    {
        switch (componentType)
        {
            case StorageType::SignedByte:
            case StorageType::UnsignedByte:
            {
                return 1;
            }
            case StorageType::SignedShort:
            case StorageType::UnsignedShort:
            {
                return 2;
            }
            case StorageType::UnsignedInt:
            case StorageType::Float:
            {
                return 4;
            }
        }
    }

    Mesh::~Mesh() = default;

    Mesh_::Mesh_(graphics::IDevice* device_) : device(device_) {}

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor_ descriptor_)
        : device(device_), descriptor(std::move(descriptor_)) {}

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor_ descriptor_,
                 void* vertexData, void* indexData)
        : device(device_), descriptor(std::move(descriptor_))
    {

    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor_ descriptor_,
                 std::vector<void*> attributesData, void* indexData)
        : device(device_), descriptor(std::move(descriptor_))
    {

    }

    Mesh_::~Mesh_() = default;

    void Mesh_::clear()
    {

    }

    bool Mesh_::setAttributeData(renderer::VertexAttributeType_ attribute, void* data, size_t index)
    {
        return false;
    }

    bool Mesh_::setAttributesData(std::vector<void*> attributesData)
    {
        return false;
    }

    void Mesh_::setVertexData(void* vertexData)
    {

    }

    void Mesh_::setIndexData(void* indexData)
    {

    }

    void Mesh_::uploadToGPU()
    {

    }
}