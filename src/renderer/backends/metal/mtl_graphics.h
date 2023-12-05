#ifndef BORED_ENGINE_MTL_GRAPHICS_H
#define BORED_ENGINE_MTL_GRAPHICS_H

#include "../../graphics.h"

namespace renderer
{
	class MetalGraphicsBackend final : public GraphicsBackend
	{
	public:
		explicit MetalGraphicsBackend(Graphics* renderer);
		~MetalGraphicsBackend() override;

		static MetalGraphicsBackend* pInstance;

		class Implementation;

		Implementation* getImplementation();

	private:
		std::unique_ptr<Implementation> pImplementation;
	};
}

#endif //BORED_ENGINE_MTL_GRAPHICS_H