#ifndef BORED_ENGINE_RENDERER_H
#define BORED_ENGINE_RENDERER_H

#include <string>
#include <vector>
#include "texture.h"
#include "mesh.h"
#include "material.h"
#include "shader.h"

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

	// as on macOS the view calls a delegate that will then render a new frame
	// instead of the other way around, we adopt this concept for our renderer
	// as well.
	//
	// the delegate is responsible keeping a reference of which window needs to have which content rendered.
	class RendererDelegate
	{
	public:
		virtual ~RendererDelegate();

		/**
		 * todo: add additional parameters that pass renderer information relevant for
		 * 		 the user to render to this specific window
		 *
		 * @param window the window that needs to be rendered
		 */
		virtual void render(engine::Window* window);
	};

	class Renderer
	{
	public:
		virtual ~Renderer();

		// delegate
		RendererDelegate* getDelegate(); // change to shared pointer?
		void setDelegate(RendererDelegate* delegate);

		// when adding a window to the renderer, it will initialize a renderer
		// view for that window
		virtual void addWindow(engine::Window* window);
		virtual void removeWindow(engine::Window* window);

		// factory methods
		virtual std::unique_ptr<Texture> createTexture();
		virtual std::unique_ptr<Mesh> createMesh();
		virtual std::unique_ptr<Material> createMaterial();
		virtual std::unique_ptr<Shader> createShader();

	protected:
		RendererDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_RENDERER_H