#ifndef BORED_ENGINE_GRAPHICS_H
#define BORED_ENGINE_GRAPHICS_H

#include <string>
#include <vector>
#include <unordered_set>

namespace renderer
{
	enum class GraphicsBackendType
	{
		None = 0,
		Metal,
		Vulkan,
		OpenGL,
		WebGPU
	};

	std::string ToString(GraphicsBackendType const& graphicsBackendType);

	//------------------------------------------------
	//  GraphicsObject
	//------------------------------------------------

	// object that gets managed by the Renderer, so that when the backend changes
	// its implementation automatically gets updated
	class GraphicsObject
	{
	public:
		virtual ~GraphicsObject();
		virtual void onGraphicsBackendChanged(const GraphicsBackendType& rendererBackendType);

		// note: should always be called after creating a graphics object
		// otherwise its backend implementation won't be loaded.
		// you can also use the convenience function `create<T>()`
		void registerObject();
	};

	// function for creating and automatically registering a renderer object
	template <class T, typename... Args>
	std::enable_if<std::is_base_of<GraphicsObject, T>::value, std::unique_ptr<T>>::type create(Args&&... args)
	{
		std::unique_ptr<T> object = std::make_unique<T>(std::forward<Args>(args)...);
		object->registerObject();
		return object;
	}

	class GraphicsBackend;

	//------------------------------------------------
	//  Graphics
	//------------------------------------------------

	// overarching class that manages the lifetime of graphics backends
	class Graphics
	{
	public:
		explicit Graphics();
		~Graphics();

		static Graphics* pInstance;

		void registerObject(GraphicsObject* object);
		void unregisterObject(GraphicsObject* object);

		// backend
		[[nodiscard]] GraphicsBackendType getGraphicsBackendType() const;
		void setGraphicsBackendType(renderer::GraphicsBackendType const& type);
		GraphicsBackend* getGraphicsBackend();

	private:
		// backend
		GraphicsBackendType graphicsBackendType{GraphicsBackendType::None};
		std::unique_ptr<GraphicsBackend> graphicsBackend;

		// objects that need to be communicated with when the graphics backend is changed
		std::unordered_set<GraphicsObject*> pObjects{};
	};

	//------------------------------------------------
	//  GraphicsBackend
	//------------------------------------------------

	// cross-platform implementation that gets swapped out
	class GraphicsBackend
	{
	public:
		explicit GraphicsBackend(Graphics* renderer);
		virtual ~GraphicsBackend();

	protected:
		Graphics* pGraphics;
	};
}

#endif //BORED_ENGINE_GRAPHICS_H