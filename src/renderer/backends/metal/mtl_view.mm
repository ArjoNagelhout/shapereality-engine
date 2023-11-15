#include "mtl_view.h"

#include <iostream>

#include "mtl_renderer.h"

#include "../../../view_cocoa.h"

namespace renderer
{
	MetalView::MetalView(MetalRenderer* renderer, engine::Window* window) :
		pRenderer(renderer),
		pWindow(window),
		delegate(std::make_unique<Delegate>()),
		engine::View()
	{
		std::cout << "new metal view" << std::endl;

		engine::Rect r = pWindow->getRect();
		CGRect frame = CGRectMake(r.x, r.y, r.width, r.height);

		pMtkView = MTK::View::alloc()->init(frame, renderer->getDevice());
		pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
		pMtkView->setClearColor(MTL::ClearColor::Make(0.3, 0.3, 0.6, 1.0));
		pMtkView->setDepthStencilPixelFormat(MTL::PixelFormat::PixelFormatDepth16Unorm);
		pMtkView->setClearDepth(1.0f);
		pMtkView->setDelegate(delegate.get());

		pImpl->pView = (__bridge NSView*)pMtkView;

		// add the MtkView to the content view of the window
		pWindow->setContentView(this);
	}

	void MetalView::Delegate::drawInMTKView(class MTK::View* pView)
	{
		std::cout << "draw in mtk view" << std::endl;
	}

	void MetalView::Delegate::drawableSizeWillChange(class MTK::View* pView, CGSize size)
	{
		std::cout << "drawable size will change" << std::endl;
	}

	MetalView::~MetalView()
	{
		std::cout << "destroy metal view" << std::endl;
		pWindow->setContentView(nullptr);
		pMtkView->release();
	}
}