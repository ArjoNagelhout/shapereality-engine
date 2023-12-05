#ifndef BORED_ENGINE_VK_GRAPHICS_H
#define BORED_ENGINE_VK_GRAPHICS_H

#include "../../graphics.h"

namespace renderer
{
	class VulkanGraphicsBackend final : public GraphicsBackend
	{
	public:
		explicit VulkanGraphicsBackend(Graphics* renderer);
		~VulkanGraphicsBackend() override;

	private:
	};
}

#endif //BORED_ENGINE_VK_GRAPHICS_H
