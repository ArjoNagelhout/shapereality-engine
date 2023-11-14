#ifndef BORED_ENGINE_MTL_VIEW_H
#define BORED_ENGINE_MTL_VIEW_H

#include "../../../window.h"

#include <MetalKit/MetalKit.hpp>

namespace renderer
{
	class MetalRenderer;

	// surface that can be drawn to, tied to a specific window
	// should be destroyed when the window it is attached to is closed
	class MetalView : public MTK::ViewDelegate
	{
	public:
		explicit MetalView(MetalRenderer* renderer, engine::Window* window);
		~MetalView() override;

	private:
		MetalRenderer* pRenderer;
		MTK::View* pMtkView{};
	};
}

#endif //BORED_ENGINE_MTL_VIEW_H
