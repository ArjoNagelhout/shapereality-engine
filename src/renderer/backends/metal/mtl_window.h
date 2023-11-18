//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_WINDOW_H
#define BORED_ENGINE_MTL_WINDOW_H

#include "../../window.h"

namespace renderer
{
	class MetalWindowImplementation final : public WindowRendererImplementation
	{
	public:
		explicit MetalWindowImplementation(Window* window);
		~MetalWindowImplementation() override;
	};
}

#endif //BORED_ENGINE_MTL_WINDOW_H
