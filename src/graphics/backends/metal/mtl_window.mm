//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_window.h"

#include "graphics/platform/cocoa/cocoa_window.h"
#include "../../graphics.h"

#include "mtl_render_pass.h"
#include "mtl_texture.h"

#include <iostream>

@implementation MetalView

- (void)drawRect:(NSRect)dirtyRect
{
	_pWindow->getRenderDelegate()->render(_pWindow);
}

@end

namespace graphics
{
	MetalWindow::MetalWindow(WindowDescriptor descriptor, id <MTLDevice> _Nonnull pDevice) : Window(descriptor)
	{
		// initialize metal view
		NSWindow* nsWindow = pImplementation->pWindowAdapter;
		pMetalView = [[MetalView alloc] initWithFrame:nsWindow.frame
										   device:pDevice];
		[pMetalView retain];
		pMetalView.pWindow = this;
		[pMetalView setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
		Color c{0.f, 0.5f, 1.f, 1.f};
		[pMetalView setClearColor:MTLClearColorMake(c.r, c.g, c.b, c.a)];
		[pMetalView setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
		[pMetalView setClearDepth:1.0f];
		[nsWindow setContentView:pMetalView];
	}

	MetalWindow::~MetalWindow()
	{
		[pMetalView release];
	}

	std::unique_ptr<IRenderPass> MetalWindow::getRenderPass() const
	{
		return std::make_unique<MetalRenderPass>(pMetalView.currentRenderPassDescriptor);
	}

	std::unique_ptr<ITexture> MetalWindow::getDrawable() const
	{
		id<MTLDrawable> drawable = [pMetalView currentDrawable];
		return std::make_unique<MetalTexture>(drawable);
	}
}