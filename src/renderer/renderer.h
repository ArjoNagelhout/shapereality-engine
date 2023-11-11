#ifndef BORED_ENGINE_RENDERER_H
#define BORED_ENGINE_RENDERER_H

#include <string>

namespace renderer
{
	enum class RendererBackend
	{
		None = 0,
		Metal,
		Vulkan,
		OpenGL,
		WebGPU
	};

	std::string ToString(RendererBackend const& rendererBackend);

	class Renderer
	{
	public:
		virtual ~Renderer();

		virtual void render();

	private:

	};
}

#endif //BORED_ENGINE_RENDERER_H