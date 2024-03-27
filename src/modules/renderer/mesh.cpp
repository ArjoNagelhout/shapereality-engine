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

    size_t stride(ComponentType componentType)
    {
        switch (componentType)
        {
            case ComponentType::SignedByte:
            case ComponentType::UnsignedByte:
            {
                return 1;
            }
            case ComponentType::SignedShort:
            case ComponentType::UnsignedShort:
            {
                return 2;
            }
            case ComponentType::UnsignedInt:
            case ComponentType::Float:
            {
                return 4;
            }
        }
    }

    size_t componentCount(ElementType elementType)
    {
        switch (elementType)
        {
            case ElementType::Scalar: return 1;
            case ElementType::Vector2: return 2;
            case ElementType::Vector3: return 3;
            case ElementType::Vector4: return 4;
            case ElementType::Matrix2x2: return 2 * 2;
            case ElementType::Matrix3x3: return 3 * 3;
            case ElementType::Matrix4x4: return 4 * 4;
        }
    }

    Mesh::~Mesh() = default;

    Mesh_::Mesh_(graphics::IDevice* device_) : device(device_)
    {
        assert(device && "graphics device was not set");
    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor descriptor_)
        : device(device_), descriptor(std::move(descriptor_))
    {
        assert(device && "graphics device was not set");
    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor descriptor_,
                 void* vertexData, void* indexData)
        : device(device_), descriptor(std::move(descriptor_))
    {
        assert(device && "graphics device was not set");
    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor descriptor_,
                 std::vector<void*> const& attributesData, void* indexData)
        : device(device_), descriptor(std::move(descriptor_))
    {
        assert(device && "graphics device was not set");
        assert(setAttributesData(attributesData));
        setIndexData(indexData);
    }

    Mesh_::~Mesh_() = default;

    void Mesh_::clear()
    {

    }

    bool Mesh_::setAttributeData(renderer::VertexAttribute_ attribute, void* data, size_t index)
    {
        return false;
    }

    bool Mesh_::setAttributesData(std::vector<void*> const& attributesData)
    {
        return true;
    }

    void Mesh_::setVertexData(void* vertexData)
    {
        if (vertexData == nullptr)
        {
            return;
        }

        assert(false && "not implemented");
    }

    void Mesh_::setIndexData(void* indexData)
    {
        if (indexData == nullptr)
        {
            return;
        }
        assert(false && "not implemented");
    }

    void Mesh_::uploadToGPU()
    {

    }

    void Mesh_::reallocateVertexBuffer()
    {
        // first destroy
        if (vertexBuffer)
        {
            vertexBuffer.reset();
        }

        //vertexBuffer =
    }
}