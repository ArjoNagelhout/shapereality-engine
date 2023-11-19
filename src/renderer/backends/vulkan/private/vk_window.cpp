//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../vk_window.h"

namespace renderer
{
	VulkanWindow::VulkanWindow(renderer::Window* window) : WindowRendererImplementation(window)
	{
	}

	VulkanWindow::~VulkanWindow() = default;
}