#ifndef BORED_ENGINE_METAL_RENDERER_H
#define BORED_ENGINE_METAL_RENDERER_H

#include "../../renderer.h"

namespace renderer
{
	class MetalRenderer : Renderer
	{
		explicit MetalRenderer();

		~MetalRenderer() override;

		void Render() override;
	};
}

#endif //BORED_ENGINE_METAL_RENDERER_H