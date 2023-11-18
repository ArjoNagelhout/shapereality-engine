#ifndef BORED_ENGINE_MTL_RENDERER_H
#define BORED_ENGINE_MTL_RENDERER_H

#include "../../renderer.h"

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

namespace renderer
{
	class MetalRendererBackend : public RendererBackend
	{
	public:
		explicit MetalRendererBackend(Renderer* renderer);
		~MetalRendererBackend() override;

		MTL::Device* getDevice();

	private:
		NS::AutoreleasePool* pAutoreleasePool; // ????
		MTL::Device* pDevice{};
	};
}

#endif //BORED_ENGINE_MTL_RENDERER_H