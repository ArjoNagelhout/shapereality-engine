#ifndef BORED_ENGINE_MTL_RENDERER_H
#define BORED_ENGINE_MTL_RENDERER_H

#include "../../renderer.h"

namespace renderer
{
	class MetalRendererBackend final : public RendererBackend
	{
	public:
		explicit MetalRendererBackend(Renderer* renderer);
		~MetalRendererBackend() override;

		static MetalRendererBackend* pInstance;

		class Implementation;

		Implementation* getImplementation();

	private:
		std::unique_ptr<Implementation> pImplementation;
	};
}

#endif //BORED_ENGINE_MTL_RENDERER_H