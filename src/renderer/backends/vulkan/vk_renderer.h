#ifndef BORED_ENGINE_VK_RENDERER_H
#define BORED_ENGINE_VK_RENDERER_H

#include "../../renderer.h"

namespace renderer
{
	class VulkanRenderer : Renderer
	{
		explicit VulkanRenderer();

		~VulkanRenderer() override;

		void Render() override;
	};
}

#endif //BORED_ENGINE_VK_RENDERER_H
