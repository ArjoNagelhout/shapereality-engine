#include "mtl_view.h"

#include <iostream>

#include "mtl_renderer.h"

namespace renderer
{
	MetalView::MetalView(MetalRenderer* renderer, engine::Window* window) : pRenderer(renderer)
	{
		std::cout << "new metal view" << std::endl;

		engine::Rect r = window->getRect();
		CGRect frame = CGRectMake(r.x, r.y, r.width, r.height);

		pMtkView = MTK::View::alloc()->init(frame, renderer->getDevice());
		pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
		pMtkView->setClearColor(MTL::ClearColor::Make(0.3, 0.3, 0.6, 1.0));
		pMtkView->setDepthStencilPixelFormat(MTL::PixelFormat::PixelFormatDepth16Unorm);
		pMtkView->setClearDepth(1.0f);
		pMtkView->setDelegate(this);
	}

	MetalView::~MetalView()
	{
		std::cout << "destroy metal view" << std::endl;
		pMtkView->release();
	}
}