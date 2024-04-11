#include "mesh.h"

using namespace math;

namespace renderer
{
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

    // MeshDescriptor

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

    // VertexAttributesIterator

    VertexAttributesIterator::VertexAttributesIterator(renderer::Mesh const& mesh_, size_t index_) : mesh(mesh_)
    {
        current.index = index_;
        updateCurrent();
    }

    VertexAttributesIterator::const_reference VertexAttributesIterator::operator*() const
    {
        return current;
    }

    VertexAttributesIterator& VertexAttributesIterator::operator++()
    {
        current.index++;
        updateCurrent();
        return *this;
    }

    bool operator==(VertexAttributesIterator const& lhs, VertexAttributesIterator const& rhs)
    {
        return lhs.current.index == rhs.current.index && std::addressof(lhs.mesh) == std::addressof(rhs.mesh);
    }

    bool operator!=(VertexAttributesIterator const& lhs, VertexAttributesIterator const& rhs)
    {
        return !(lhs == rhs);
    }

    void VertexAttributesIterator::updateCurrent()
    {
        if (current.index > mesh.descriptor_.attributes.size())
        {
            return;
        }

        current.descriptor = &mesh.descriptor_.attributes[current.index];
        current.offset = mesh.offsets[current.index];
    }

    // Mesh

    Mesh::Mesh(graphics::IDevice* device_, MeshDescriptor descriptor)
        : device(device_), descriptor_(std::move(descriptor))
    {
        assert(device && "graphics device was not set");
        assert(descriptor_.valid() && "descriptor should be valid");

        createVertexBuffer();
        if (descriptor_.hasIndexBuffer)
        {
            createIndexBuffer();
        }
        updateOffsets();
    }

    Mesh::Mesh(graphics::IDevice* device_, MeshDescriptor descriptor, void* vertexData, void* indexData)
        : Mesh(device_, std::move(descriptor))
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

    Mesh::Mesh(graphics::IDevice* device_, MeshDescriptor descriptor, std::vector<void*> const& attributesData, void* indexData)
        : Mesh(device_, std::move(descriptor))
    {
        setAttributesData(attributesData);
        if (indexData)
        {
            setIndexData(indexData);
        }
    }

    Mesh::~Mesh() = default;

    void Mesh::clear()
    {
        // todo
        assert(false && "todo: not implemented");
    }

    void Mesh::setAttributeData(renderer::VertexAttribute_ attribute, void* data, size_t index)
    {
        size_t attributeIndex = getAttributeIndex(attribute, index);
        VertexAttributeDescriptor const& attributeDescriptor = descriptor_.attributes[attributeIndex];
        size_t size = elementSize(attributeDescriptor) * descriptor_.vertexCount;
        size_t offset = offsets[attributeIndex];
        vertexBuffer_->set(data, size, offset, true);
    }

    void Mesh::setAttributesData(std::vector<void*> const& attributesData)
    {
        assert(descriptor_.attributes.size() == attributesData.size());

        size_t offset = 0;
        for (size_t i = 0; i < attributesData.size(); i++)
        {
            void* attributeData = attributesData[i];
            VertexAttributeDescriptor const& attribute = descriptor_.attributes[i];
            size_t size = elementSize(attribute) * descriptor_.vertexCount;
            vertexBuffer_->set(attributeData, size, offset, false);

            // update offset
            offset += size;
        }
        vertexBuffer_->synchronize();
    }

    void Mesh::setVertexData(void* vertexData)
    {
        assert(vertexData && "provided vertex data should not be nullptr");

        vertexBuffer_->set(vertexData, true);
    }

    void Mesh::setIndexData(void* indexData)
    {
        assert(indexData && "provided index data should not be nullptr");
        assert(descriptor_.hasIndexBuffer && "hasIndexBuffer should be set to true when setting the index data");

        indexBuffer_->set(indexData, true);
    }

    MeshDescriptor const& Mesh::descriptor() const
    {
        return descriptor_;
    }

    graphics::Buffer* Mesh::vertexBuffer()
    {
        assert(vertexBuffer_ && "vertex buffer should always exist");
        return vertexBuffer_.get();
    }

    graphics::Buffer* Mesh::indexBuffer()
    {
        assert(descriptor_.hasIndexBuffer && "descriptor should have hasIndexBuffer set to true");
        return indexBuffer_.get();
    }

    VertexAttributesIterator Mesh::begin() const
    {
        return VertexAttributesIterator{*this, 0};
    }

    VertexAttributesIterator Mesh::end() const
    {
        return VertexAttributesIterator{*this, descriptor_.attributes.size()};
    }

    size_t Mesh::desiredVertexBufferSize()
    {
        size_t sum = 0;
        for (auto& attribute: descriptor_.attributes)
        {
            sum += elementSize(attribute) * descriptor_.vertexCount;
        }
        return sum;
    }

    void Mesh::createVertexBuffer()
    {
        size_t desiredSize = desiredVertexBufferSize();
        assert(desiredSize != 0 && "can't create empty mesh");

        // check if size is desired size
        if (vertexBuffer_)
        {
            if (vertexBuffer_->descriptor().size == desiredSize)
            {
                return; // nothing to do, already at the right size
            }
        }

        graphics::BufferDescriptor vertexBufferDescriptor{
            .usage = bufferUsage(),
            .size = desiredSize,
        };
        vertexBuffer_ = device->createBuffer(vertexBufferDescriptor);
    }

    void Mesh::createIndexBuffer()
    {
        assert(descriptor_.hasIndexBuffer);

        graphics::BufferDescriptor indexBufferDescriptor{
            .usage = bufferUsage(),
            .size = descriptor_.indexCount * stride(descriptor_.indexType),
            .stride = stride(descriptor_.indexType)
        };
        indexBuffer_ = device->createBuffer(indexBufferDescriptor);
    }

    graphics::BufferUsage_ Mesh::bufferUsage() const
    {
        static auto writableUsage = static_cast<graphics::BufferUsage_>(graphics::BufferUsage_CPUWrite | graphics::BufferUsage_GPURead);
        static graphics::BufferUsage_ nonWritableUsage = graphics::BufferUsage_GPURead;
        //return descriptor_.writable ? writableUsage : nonWritableUsage;
        return graphics::BufferUsage_All;
    }

    void Mesh::updateOffsets()
    {
        offsets.resize(descriptor_.attributes.size());
        size_t offset = 0;
        for (size_t i = 0; i < descriptor_.attributes.size(); i++)
        {
            offsets[i] = offset;

            VertexAttributeDescriptor const& attribute = descriptor_.attributes[i];
            size_t size = elementSize(attribute) * descriptor_.vertexCount;
            offset += size;
        }
    }

    size_t Mesh::getAttributeIndex(VertexAttribute_ attribute, size_t index) const
    {
        for (size_t i = 0; i < descriptor_.attributes.size(); i++)
        {
            VertexAttributeDescriptor const& entry = descriptor_.attributes[i];
            if (entry.type == attribute && entry.index == index)
            {
                return i;
            }
        }
        assert(false && "Mesh does not contain attribute type with provided index");
    }
}