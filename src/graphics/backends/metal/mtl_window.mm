//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_window.h"

#include "../../window_cocoa.h"
#include "../../types.h"

#include "mtl_render_pass.h"
#include "mtl_texture.h"

#include <iostream>

@implementation MTKViewDelegate

- (void)drawInMTKView:(nonnull MTKView *)view
{
	_pWindow->getDelegate()->render(_pWindow);
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{

}

@end

namespace graphics
{
	MetalWindow::MetalWindow(WindowDescriptor descriptor, id <MTLDevice> _Nonnull pDevice) : Window(descriptor)
	{
		// create delegate
		pMTKViewDelegate = [[MTKViewDelegate alloc] init];
		pMTKViewDelegate.pWindow = this;

		// initialize mtk view
		NSWindow* nsWindow = pImplementation->pWindow;
		pMTKView = [[MTKView alloc] initWithFrame:nsWindow.frame
										   device:pDevice];
		[pMTKView setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
		Color c{0.f, 1.f, 1.f, 1.f};
		[pMTKView setClearColor:MTLClearColorMake(c.r, c.g, c.b, c.a)];
		[pMTKView setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
		[pMTKView setClearDepth:1.0f];
		[pMTKView setDelegate:pMTKViewDelegate];
		[nsWindow setContentView:pMTKView];
	}

	MetalWindow::~MetalWindow()
	{
		[pMTKView release];
	}

	std::unique_ptr<IRenderPass> MetalWindow::getRenderPass() const
	{
		return std::make_unique<MetalRenderPass>(pMTKView.currentRenderPassDescriptor);
	}

	std::unique_ptr<ITexture> MetalWindow::getDrawable() const
	{
		id<MTLDrawable> drawable = [pMTKView currentDrawable];

		return std::make_unique<MetalTexture>(drawable);
	}
}