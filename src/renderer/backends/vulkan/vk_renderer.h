#ifndef BORED_ENGINE_VK_RENDERER_H
#define BORED_ENGINE_VK_RENDERER_H

#include "../../renderer.h"

namespace renderer
{
	class VulkanRenderer : public Renderer
	{
	public:
		explicit VulkanRenderer();
		~VulkanRenderer() override;
		void render() override;

	private:
	};
}

#endif //BORED_ENGINE_VK_RENDERER_H
