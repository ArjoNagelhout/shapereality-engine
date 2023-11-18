#ifndef BORED_ENGINE_VK_RENDERER_H
#define BORED_ENGINE_VK_RENDERER_H

#include "../../renderer.h"

namespace renderer
{
	class VulkanRendererBackend : public RendererBackend
	{
	public:
		explicit VulkanRendererBackend(Renderer* renderer);
		~VulkanRendererBackend() override;

	private:
	};
}

#endif //BORED_ENGINE_VK_RENDERER_H
