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

		// initialize mtk view
		NSWindow* nsWindow = pWindow->getPlatformImplementation()->pWindow;
		pImplementation->pMtkView = [[MTKView alloc] initWithFrame:nsWindow.frame
													  device:MetalRendererBackend::pInstance->getImplementation()->pDevice];
		[pImplementation->pMtkView setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
		[pImplementation->pMtkView setClearColor:MTLClearColorMake(0.3, 0.3, 0.1, 1.0)];
		[pImplementation->pMtkView setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
		[pImplementation->pMtkView setClearDepth:1.0f];

		[nsWindow setContentView:pImplementation->pMtkView];
	}

	MetalWindowImplementation::~MetalWindowImplementation()
	{
		[pImplementation->pMtkView release];
	}
}