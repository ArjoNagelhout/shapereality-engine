//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_MESH_H
#define SHAPEREALITY_MESH_H

#include "graphics/device.h"
#include "graphics/buffer.h"

#include "math/vector.h"

namespace renderer
{
	using index_type = uint32_t;

	// warning: MSL float3 has size 4*4 bytes, instead of 4*3 bytes,
	// so we need to use packed_float3 inside the shader
	struct VertexData
	{
		math::vec3 position;
		math::vec3 normal;
		math::vec2 uv0;
	};

	class Mesh
	{
	public:
		explicit Mesh(graphics::IDevice* pDevice, std::vector<VertexData> const& vertexData, std::vector<index_type> const& indices);
		~Mesh();

		[[nodiscard]] graphics::IBuffer* getVertexBuffer() const;
		[[nodiscard]] graphics::IBuffer* getIndexBuffer() const;

	private:
		std::unique_ptr<graphics::IBuffer> pVertexBuffer;
		std::unique_ptr<graphics::IBuffer> pIndexBuffer;
	};
}

#endif //SHAPEREALITY_MESH_H
