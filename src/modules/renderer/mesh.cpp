#include "mesh.h"

#include "math/vector.inl"

using namespace math;

namespace renderer
{
    Mesh::Mesh(graphics::IDevice* device,
               std::vector<VertexData> const& verticesData,
               std::vector<IndexType> const& indices)
        : indexCount(indices.size())
    {
        graphics::BufferDescriptor vertexBufferDescriptor{
            .usage = graphics::BufferUsage_GPURead,
            .size = static_cast<unsigned int>(verticesData.size() * sizeof(VertexData)),
            .stride = sizeof(VertexData)
        };
        vertexBuffer = device->createBuffer(vertexBufferDescriptor, (void*)verticesData.data(), false);

        graphics::BufferDescriptor indexBufferDescriptor{
            .usage = graphics::BufferUsage_GPURead,
            .size = static_cast<unsigned int>(indices.size() * sizeof(IndexType)),
            .stride = sizeof(IndexType)
        };
        indexBuffer = device->createBuffer(indexBufferDescriptor, (void*)indices.data(), false);
    }

    graphics::Buffer* Mesh::getVertexBuffer() const
    {
        return vertexBuffer.get();
    }

    graphics::Buffer* Mesh::getIndexBuffer() const
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

    [[nodiscard]] size_t elementSize(VertexAttributeDescriptor const& descriptor)
    {
        return componentCount(descriptor.elementType) * stride(descriptor.componentType);
    }

    Mesh::~Mesh() = default;

    bool MeshDescriptor::valid() const
    {
        if (hasIndexBuffer)
        {
            if (indexCount == 0)
            {
                return false;
            }

            if (!(indexType == ComponentType::UnsignedInt || indexType == ComponentType::UnsignedShort))
            {
                return false;
            }
        }

        return true;
    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor descriptor)
        : device(device_), descriptor_(std::move(descriptor))
    {
        validate();
        createVertexBuffer();
        createIndexBuffer();
    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor descriptor, void* vertexData, void* indexData)
        : Mesh_(device_, std::move(descriptor))
    {
        if (vertexData)
        {
            setVertexData(vertexData);
        }

        if (indexData)
        {
            setIndexData(indexData);
        }
    }

    Mesh_::Mesh_(graphics::IDevice* device_, MeshDescriptor descriptor, std::vector<void*> const& attributesData, void* indexData)
        : Mesh_(device_, std::move(descriptor))
    {
        setAttributesData(attributesData);
        if (indexData)
        {
            setIndexData(indexData);
        }
    }

    Mesh_::~Mesh_() = default;

    void Mesh_::clear()
    {
        // todo
        assert(false && "todo: not implemented");
    }

    void Mesh_::setAttributeData(renderer::VertexAttribute_ attribute, void* data, size_t index)
    {
        // todo
        assert(false && "todo: not implemented");
    }

    void Mesh_::setAttributesData(std::vector<void*> const& attributesData)
    {
        assert(descriptor_.attributes.size() == attributesData.size());

        size_t offset = 0;
        for (size_t i = 0; i < attributesData.size(); i++)
        {
            void* attributeData = attributesData[i];
            VertexAttributeDescriptor const& attribute = descriptor_.attributes[i];
            size_t size = elementSize(attribute) * descriptor_.vertexCount;
            vertexBuffer->set(attributeData, size, offset, false);

            // update offset
            offset += size;
        }
        vertexBuffer->synchronize();
    }

    void Mesh_::setVertexData(void* vertexData)
    {
        assert(vertexData && "provided vertex data should not be nullptr");

        vertexBuffer->set(vertexData, true);
    }

    void Mesh_::setIndexData(void* indexData)
    {
        assert(indexData && "provided index data should not be nullptr");
        assert(descriptor_.hasIndexBuffer && "hasIndexBuffer should be set to true when setting the index data");

        indexBuffer->set(indexData, true);
    }

    size_t Mesh_::desiredVertexBufferSize()
    {
        size_t sum = 0;
        for (auto& attribute: descriptor_.attributes)
        {
            sum += elementSize(attribute) * descriptor_.vertexCount;
        }
        return sum;
    }

    void Mesh_::createVertexBuffer()
    {
        size_t desiredSize = desiredVertexBufferSize();
        assert(desiredSize != 0 && "can't create empty mesh");

        // check if size is desired size
        if (vertexBuffer)
        {
            if (vertexBuffer->descriptor().size == desiredSize)
            {
                return; // nothing to do, already at the right size
            }
        }

        graphics::BufferDescriptor vertexBufferDescriptor{
            .usage = bufferUsage(),
            .size = desiredSize,
        };
        vertexBuffer = device->createBuffer(vertexBufferDescriptor);
    }

    void Mesh_::createIndexBuffer()
    {
        assert(descriptor_.hasIndexBuffer);

        graphics::BufferDescriptor indexBufferDescriptor{
            .usage = bufferUsage(),
            .size = descriptor_.indexCount * stride(descriptor_.indexType),
            .stride = stride(descriptor_.indexType)
        };
        indexBuffer = device->createBuffer(indexBufferDescriptor);
    }

    graphics::BufferUsage_ Mesh_::bufferUsage() const
    {
        static auto writableUsage = static_cast<graphics::BufferUsage_>(graphics::BufferUsage_CPUWrite | graphics::BufferUsage_GPURead);
        static graphics::BufferUsage_ nonWritableUsage = graphics::BufferUsage_GPURead;
        return descriptor_.writable ? writableUsage : nonWritableUsage;
    }

    void Mesh_::validate() const
    {
        assert(device && "graphics device was not set");
        assert(descriptor_.valid() && "descriptor should be valid");
    }
}