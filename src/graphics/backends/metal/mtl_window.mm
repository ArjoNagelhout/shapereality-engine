//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_window.h"

#include "../../window_cocoa.h"

#include "../../../math/vector.inl"

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
	MetalWindow::MetalWindow(WindowDescription description, id <MTLDevice> _Nonnull pDevice) : IWindow(description)
	{
		// create delegate
		pMTKViewDelegate = [[MTKViewDelegate alloc] init];
		pMTKViewDelegate.pWindow = this;

		// initialize mtk view
		NSWindow* nsWindow = pImplementation->pWindow;
		pMTKView = [[MTKView alloc] initWithFrame:nsWindow.frame
										   device:pDevice];
		[pMTKView setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
		math::vec4 c = description.clearColor;
		[pMTKView setClearColor:MTLClearColorMake(c.r(), c.g(), c.b(), c.a())];
		[pMTKView setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
		[pMTKView setClearDepth:1.0f];
		[pMTKView setDelegate:pMTKViewDelegate];
		[nsWindow setContentView:pMTKView];
	}

	MetalWindow::~MetalWindow()
	{
		[pMTKView release];
	}
}