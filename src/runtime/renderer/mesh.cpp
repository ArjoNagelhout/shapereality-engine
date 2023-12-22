#include "mesh.h"

#include "math/vector.inl"

using namespace math;

namespace renderer
{

	Mesh::Mesh(graphics::IDevice* pDevice,
			   std::vector<VertexData> const& vertexData,
			   std::vector<index_type> const& indices)
	{
		graphics::BufferDescriptor vertexBufferDescriptor{
			.type = graphics::BufferDescriptor::Type::Vertex,
			.storageMode = graphics::BufferDescriptor::StorageMode::Managed,
			.data = vertexData.data(),
			.length = static_cast<unsigned int>(vertexData.size() * sizeof(VertexData)),
			.stride = sizeof(VertexData)
		};
		pVertexBuffer = pDevice->createBuffer(vertexBufferDescriptor);

		graphics::BufferDescriptor indexBufferDescriptor{
			.type = graphics::BufferDescriptor::Type::Index,
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

	Mesh::~Mesh() = default;
}