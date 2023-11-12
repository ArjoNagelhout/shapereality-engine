#ifndef BORED_ENGINE_MTL_VIEW_H
#define BORED_ENGINE_MTL_VIEW_H

namespace renderer
{
	// surface that can be drawn to, tied to a specific window
	// should be destroyed when the window it is attached to is closed
	class MetalView
	{
		explicit MetalView();
		~MetalView();
	};
}

#endif //BORED_ENGINE_MTL_VIEW_H
