#ifndef BORED_ENGINE_MTL_RENDERER_H
#define BORED_ENGINE_MTL_RENDERER_H

#include "../../renderer.h"
#include "mtl_view.h"

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

		void registerWindow(engine::Window *window) override;
		void unregisterWindow(engine::Window *window) override;

		MTL::Device* getDevice();

	private:
		NS::AutoreleasePool* pAutoreleasePool; // ????
		MTL::Device* pDevice{};
		std::unordered_map<engine::Window*, std::unique_ptr<MetalView>> metalViews;
	};
}

#endif //BORED_ENGINE_MTL_RENDERER_H