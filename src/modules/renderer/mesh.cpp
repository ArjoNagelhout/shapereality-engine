#include "mesh.h"

#include "math/vector.inl"

using namespace math;

namespace renderer
{
    Mesh::Mesh(graphics::IDevice* pDevice,
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
        pVertexBuffer = pDevice->createBuffer(vertexBufferDescriptor);

        graphics::BufferDescriptor indexBufferDescriptor{
            .storageMode = graphics::BufferDescriptor::StorageMode::Managed,
            .data = indices.data(),
            .length = static_cast<unsigned int>(indices.size() * sizeof(index_type)),
            .stride = sizeof(index_type)
        };
        pIndexBuffer = pDevice->createBuffer(indexBufferDescriptor);
    }

    graphics::IBuffer* Mesh::getVertexBuffer() const
    {
        return pVertexBuffer.get();
    }

    graphics::IBuffer* Mesh::getIndexBuffer() const
    {
        return pIndexBuffer.get();
    }

    size_t Mesh::getIndexCount() const
    {
        return indexCount;
    }

    Mesh::~Mesh() = default;
}