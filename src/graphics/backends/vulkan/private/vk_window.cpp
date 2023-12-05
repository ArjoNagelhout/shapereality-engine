//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../vk_window.h"

namespace graphics
{
	VulkanWindow::VulkanWindow(graphics::Window* window) : WindowRendererImplementation(window)
	{
	}

	VulkanWindow::~VulkanWindow() = default;
}