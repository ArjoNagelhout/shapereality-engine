#ifndef BORED_ENGINE_RENDERER_H
#define BORED_ENGINE_RENDERER_H

#include <string>
#include <vector>
#include <unordered_set>

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

		static Renderer* pInstance;

		void registerObject(RendererObject* object);
		void unregisterObject(RendererObject* object);

		// backend
		[[nodiscard]] RendererBackendType getRendererBackendType();
		void setRendererBackendType(const renderer::RendererBackendType& _rendererBackendType);
		RendererBackend* getRendererBackend();

	private:
		// backend
		RendererBackendType rendererBackendType{RendererBackendType::None};
		std::unique_ptr<RendererBackend> rendererBackend;

		// objects that need to be communicated with when the backend is changed
		std::unordered_set<RendererObject*> pObjects{};
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

	protected:
		Renderer* pRenderer;
	};
}

#endif //BORED_ENGINE_RENDERER_H