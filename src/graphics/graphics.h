#ifndef BORED_ENGINE_GRAPHICS_H
#define BORED_ENGINE_GRAPHICS_H

#include <string>
#include <unordered_map>

namespace graphics
{
	class IDevice;

	enum class GraphicsBackend : int
	{
		None = 0,
		Metal,
		Vulkan,
		OpenGL,
		WebGPU
	};
	constexpr static char const* graphicsBackendStrings[]{
		"None",
		"Metal",
		"Vulkan",
		"OpenGL",
		"WebGPU"
	};
	constexpr char const* toString(GraphicsBackend value)
	{
		return graphicsBackendStrings[static_cast<int>(value)];
	}

	// main entrypoint for creating a renderer
	[[nodiscard]] std::unique_ptr<IDevice> createDevice(GraphicsBackend backend);
}

#endif //BORED_ENGINE_GRAPHICS_H