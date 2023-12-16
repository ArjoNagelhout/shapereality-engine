#include "mesh.h"

#include "math/vector.inl"

namespace renderer
{
	Mesh::Mesh(graphics::IDevice* pDevice)
	{
		using index_type = uint32_t;

		std::array<VertexData, 9> vertices{
			VertexData{math::vec3::up, math::vec3::zero, math::vec3{{1.0, 0.0, 0.0}}, math::vec2::zero},
			VertexData{math::vec3::left, math::vec3::zero, math::vec3{{0.0, 1.0, 1.0}}, math::vec2::zero},
			VertexData{math::vec3::right, math::vec3::zero, math::vec3{{1.0, 0.0, 1.0}}, math::vec2::zero},
			VertexData{math::vec3::down, math::vec3::zero, math::vec3{{1.0, 1.0, 0.0}}, math::vec2::zero},
			VertexData{math::vec3::left, math::vec3::zero, math::vec3{{0.0, 0.0, 1.0}}, math::vec2::zero},
			VertexData{math::vec3::right, math::vec3::zero, math::vec3{{1.0, 0.0, 1.0}}, math::vec2::zero},
			VertexData{math::vec3{{0.5, -0.5, 0}}, math::vec3::zero, math::vec3{{0.0, 0.0, 0.0}}, math::vec2::zero},
			VertexData{math::vec3{{-0.5, -0.5, 0}}, math::vec3::zero, math::vec3{{0.0, 1.0, 1.0}}, math::vec2::zero},
			VertexData{math::vec3{{0, 0.5, 0}}, math::vec3::zero, math::vec3{{1.0, 0.0, 1.0}}, math::vec2::zero},
		};

		graphics::BufferDescriptor vertexBufferDescriptor{
			.type = graphics::BufferDescriptor::Type::Vertex,
			.storageMode = graphics::BufferDescriptor::StorageMode::Managed,
			.data = &vertices,
			.length = sizeof(vertices),
			.stride = sizeof(math::vec3)
		};
		pVertexBuffer = pDevice->createBuffer(vertexBufferDescriptor);

		std::array<index_type, 9> indices{{0, 1, 2, 3, 4, 5, 6, 7, 8}};

		graphics::BufferDescriptor indexBufferDescriptor{
			.type = graphics::BufferDescriptor::Type::Index,
			.storageMode = graphics::BufferDescriptor::StorageMode::Managed,
			.data = &indices,
			.length = sizeof(indices),
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