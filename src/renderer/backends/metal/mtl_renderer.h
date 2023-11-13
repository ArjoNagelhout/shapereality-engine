#ifndef BORED_ENGINE_MTL_RENDERER_H
#define BORED_ENGINE_MTL_RENDERER_H

#include "../../renderer.h"
#include "mtl_view.h"

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

		void addWindow(engine::Window *window) override;
		void removeWindow(engine::Window *window) override;

	private:
		NS::AutoreleasePool* pAutoreleasePool;
		std::unordered_map<engine::Window*, std::unique_ptr<MetalView>> metalViews;
	};
}

#endif //BORED_ENGINE_MTL_RENDERER_H