//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../mtl_window.h"

#include "../../../window_cocoa.h"
#include "../mtl_renderer.h"
#include "mtl_renderer_implementation.h"

#import <MetalKit/MetalKit.h>

@interface MTKViewDelegate : NSObject<MTKViewDelegate>
	@property (unsafe_unretained, nonatomic) renderer::Window* pWindow;

@end

@implementation MTKViewDelegate
id<MTLCommandQueue> pCommandQueue;

-(id)init
{
	if (self = [super init])
	{
		id<MTLDevice> pDevice = renderer::MetalRendererBackend::pInstance->getImplementation()->pDevice;
		pCommandQueue = [pDevice newCommandQueue];
	}
	return self;
}

- (void)dealloc
{
	[pCommandQueue release];
	[super dealloc];
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
	NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
	id<MTLCommandBuffer> pCmd = [pCommandQueue commandBuffer];
	MTLRenderPassDescriptor* pRenderPassDescriptor = [view currentRenderPassDescriptor];
	id<MTLRenderCommandEncoder> pEncoder = [pCmd renderCommandEncoderWithDescriptor:pRenderPassDescriptor];

	[pEncoder endEncoding];
	[pCmd presentDrawable:view.currentDrawable];
	[pCmd commit];

//	[pEncoder release];
//	[pRenderPassDescriptor release];
//	[pCmd release];

	_pWindow->getDelegate()->render(_pWindow);

	[pPool release];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{

}

@end

namespace renderer
{
	struct MetalWindowImplementation::Implementation
	{
		MTKView* pMtkView;
		MTKViewDelegate* pDelegate;
	};

	MetalWindowImplementation::MetalWindowImplementation(Window* window) : WindowRendererImplementation(window)
	{
		pImplementation = std::make_unique<Implementation>();

		// create delegate
		pImplementation->pDelegate = [[MTKViewDelegate alloc] init];
		pImplementation->pDelegate.pWindow = pWindow;

		// initialize mtk view
		NSWindow* nsWindow = pWindow->getPlatformImplementation()->pWindow;
		pImplementation->pMtkView = [[MTKView alloc] initWithFrame:nsWindow.frame
													  device:MetalRendererBackend::pInstance->getImplementation()->pDevice];
		[pImplementation->pMtkView setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
		[pImplementation->pMtkView setClearColor:MTLClearColorMake(1.0, 0.5, 1.0, 1.0)];
		[pImplementation->pMtkView setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
		[pImplementation->pMtkView setClearDepth:1.0f];
		[pImplementation->pMtkView setDelegate:pImplementation->pDelegate];

		[nsWindow setContentView:pImplementation->pMtkView];
	}

	MetalWindowImplementation::~MetalWindowImplementation()
	{
		[pImplementation->pMtkView release];
	}
}