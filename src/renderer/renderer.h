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
	enum class RendererBackendType
	{
		None = 0,
		Metal,
		Vulkan,
		OpenGL,
		WebGPU
	};

	std::string ToString(RendererBackendType const& rendererBackendType);

	//------------------------------------------------
	//  RendererDelegate
	//------------------------------------------------

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
		 * @param window the window that needs to be rendered
		 */
		virtual void render(engine::Window* window);
	};

	//------------------------------------------------
	//  RendererObject
	//------------------------------------------------

	// object that gets managed by the Renderer, so that when the backend changes
	// its implementation automatically gets updated
	class RendererObject
	{
	public:
		explicit RendererObject();
		virtual ~RendererObject();
		virtual void onRendererBackendChanged(RendererBackendType const& rendererBackendType);
	};

	class RendererBackend;

	//------------------------------------------------
	//  Renderer
	//------------------------------------------------

	// overarching class that manages the lifetime of rendering backends
	class Renderer
	{
	public:
		explicit Renderer();
		~Renderer();

		static Renderer* pRenderer;

		// delegate
		RendererDelegate* getDelegate(); // change to shared pointer?
		void setDelegate(RendererDelegate* delegate);

		// when adding a window to the renderer, it will initialize a renderer
		// view for that window
		void registerWindow(engine::Window* window);
		void unregisterWindow(engine::Window* window);

		void registerObject(RendererObject* object);
		void unregisterObject(RendererObject* object);

		// backend
		[[nodiscard]] RendererBackendType getRendererBackendType();
		void setRendererBackendType(const renderer::RendererBackendType& _rendererBackendType);
		RendererBackend* getRendererBackend();

	private:
		// delegate
		RendererDelegate* pDelegate{nullptr};

		// backend
		RendererBackendType rendererBackendType{RendererBackendType::None};
		std::unique_ptr<RendererBackend> rendererBackend;

		// objects that need to be communicated with when the backend is changed
		std::vector<engine::Window*> pWindows{};
		std::vector<RendererObject*> pObjects{};
	};

	//------------------------------------------------
	//  RendererBackend
	//------------------------------------------------

	// cross-platform implementation that gets swapped out
	class RendererBackend
	{
	public:
		explicit RendererBackend(Renderer* renderer);
		virtual ~RendererBackend();

		virtual void registerWindow(engine::Window* window);
		virtual void unregisterWindow(engine::Window* window);

	protected:
		Renderer* pRenderer;
	};
}

#endif //BORED_ENGINE_RENDERER_H