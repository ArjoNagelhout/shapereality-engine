//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_VK_WINDOW_H
#define BORED_ENGINE_VK_WINDOW_H

#include "../../window.h"

namespace renderer
{
	class VulkanWindowImplementation final : public WindowRendererImplementation
	{
	public:
		explicit VulkanWindowImplementation(Window* window);
		~VulkanWindowImplementation() override;
	};
}

#endif //BORED_ENGINE_VK_WINDOW_H
