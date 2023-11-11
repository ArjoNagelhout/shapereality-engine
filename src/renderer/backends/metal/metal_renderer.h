#ifndef BORED_ENGINE_METAL_RENDERER_H
#define BORED_ENGINE_METAL_RENDERER_H

#include "../../renderer.h"

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

namespace renderer
{
	class MetalRenderer : public Renderer
	{
	public:
		explicit MetalRenderer();

		~MetalRenderer() override;

		void render() override;

	private:
		NS::AutoreleasePool* pAutoreleasePool;
	};
}

#endif //BORED_ENGINE_METAL_RENDERER_H