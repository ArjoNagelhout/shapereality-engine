#ifndef BORED_ENGINE_RENDERER_H
#define BORED_ENGINE_RENDERER_H

#include <string>
#include <vector>

namespace engine
{
	class Window;
}

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

		// when adding a window to the renderer, it will initialize a renderer
		// view for that window
		virtual void addWindow(engine::Window* window);
		virtual void removeWindow(engine::Window* window);

	protected:

	};
}

#endif //BORED_ENGINE_RENDERER_H