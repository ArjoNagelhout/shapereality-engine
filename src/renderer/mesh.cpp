#include "mesh.h"

#include "math/vector.inl"

using namespace math;

namespace renderer
{

	Mesh::Mesh(graphics::IDevice* pDevice)
	{
		using index_type = uint32_t;

		const float s = 0.5f;

		std::array<VertexData, 24> vertices{{
												VertexData{vec3{{-s, -s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, -s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, +s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, +s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, -s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, -s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, +s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, +s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, -s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, -s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, +s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, +s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, -s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, -s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, +s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, +s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, +s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, +s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, +s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, +s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, -s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, -s, -s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{+s, -s, +s}}, vec3::zero, vec3::zero, vec2::one},
												VertexData{vec3{{-s, -s, +s}}, vec3::zero, vec3::zero, vec2::one}
											}};

		graphics::BufferDescriptor vertexBufferDescriptor{
			.type = graphics::BufferDescriptor::Type::Vertex,
			.storageMode = graphics::BufferDescriptor::StorageMode::Managed,
			.data = &vertices,
			.length = sizeof(vertices),
			.stride = sizeof(math::vec3)
		};
		pVertexBuffer = pDevice->createBuffer(vertexBufferDescriptor);

		std::array<index_type, 36> indices{{
			   0, 1, 2, 2, 3, 0, /* front */
			   4, 5, 6, 6, 7, 4, /* right */
			   8, 9, 10, 10, 11, 8, /* back */
			   12, 13, 14, 14, 15, 12, /* left */
			   16, 17, 18, 18, 19, 16, /* top */
			   20, 21, 22, 22, 23, 20, /* bottom */
		}};

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