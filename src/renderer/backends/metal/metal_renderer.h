#ifndef BORED_ENGINE_METAL_RENDERER_H
#define BORED_ENGINE_METAL_RENDERER_H

#include "../../renderer.h"

namespace renderer
{
	class MetalRenderer : public Renderer
	{
	public:
		explicit MetalRenderer();

		~MetalRenderer() override;

		void render() override;
	};
}

#endif //BORED_ENGINE_METAL_RENDERER_H