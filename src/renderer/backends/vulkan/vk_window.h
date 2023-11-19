//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_VK_WINDOW_H
#define BORED_ENGINE_VK_WINDOW_H

#include "../../window.h"

namespace renderer
{
	class VulkanWindow final : public WindowRendererImplementation
	{
	public:
		explicit VulkanWindow(Window* window);
		~VulkanWindow() override;
	};
}

#endif //BORED_ENGINE_VK_WINDOW_H
