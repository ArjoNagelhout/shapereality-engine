//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_MESH_H
#define BORED_ENGINE_MESH_H

#include "graphics/device.h"
#include "graphics/buffer.h"

#include "math/vector.h"

namespace renderer
{
	// warning: MSL float3 has size 4*4 bytes, instead of 4*3 bytes,
	// so we need to use packed_float3 inside the shader
	struct VertexData
	{
		math::vec3 position;
		math::vec3 normal;
		math::vec3 color;
		math::vec2 uv0;
	};

	class Mesh
	{
	public:
		explicit Mesh(graphics::IDevice* pDevice);
		~Mesh();

		[[nodiscard]] graphics::IBuffer* getVertexBuffer() const;
		[[nodiscard]] graphics::IBuffer* getIndexBuffer() const;

	private:
		std::unique_ptr<graphics::IBuffer> pVertexBuffer;
		std::unique_ptr<graphics::IBuffer> pIndexBuffer;
	};
}

#endif //BORED_ENGINE_MESH_H
