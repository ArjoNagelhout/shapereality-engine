#ifndef BORED_ENGINE_MTL_VIEW_H
#define BORED_ENGINE_MTL_VIEW_H

#include "../../../window.h"

#include <MetalKit/MetalKit.hpp>

namespace renderer
{
	class MetalRenderer;

	// surface that can be drawn to, tied to a specific window
	// should be destroyed when the window it is attached to is closed
	class MetalView : public engine::View
	{
	public:
		explicit MetalView(MetalRenderer* renderer, engine::Window* window);
		~MetalView() override;

	private:
		class Delegate : public MTK::ViewDelegate
		{
		};

		std::unique_ptr<Delegate> delegate;
		MTK::View* pMtkView;

		MetalRenderer* pRenderer;
		engine::Window* pWindow;
	};
}

#endif //BORED_ENGINE_MTL_VIEW_H
