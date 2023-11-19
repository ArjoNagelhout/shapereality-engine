//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../mtl_window.h"

#include "../../../window_cocoa.h"
#include "../mtl_renderer.h"
#include "mtl_renderer_implementation.h"

#import <MetalKit/MetalKit.h>

namespace renderer
{
	struct MetalWindowImplementation::Implementation
	{
		MTKView* pMtkView;
	};

	MetalWindowImplementation::MetalWindowImplementation(Window* window) : WindowRendererImplementation(window)
	{
		pImplementation = std::make_unique<Implementation>();

		// get renderer
		RendererBackend* backend = renderer::Renderer::pRenderer->getRendererBackend();
		auto* metalBackend = dynamic_cast<renderer::MetalRendererBackend*>(backend);
		MetalRendererBackend::Implementation* implementation = metalBackend->getImplementation();

		// initialize mtk view
		NSWindow* nsWindow = pWindow->getPlatformImplementation()->pWindow;
		pImplementation->pMtkView = [[MTKView alloc] initWithFrame:nsWindow.frame
													  device:implementation->pDevice];
		[nsWindow setContentView:pImplementation->pMtkView];
	}

	MetalWindowImplementation::~MetalWindowImplementation()
	{
		[pImplementation->pMtkView release];
	}
}