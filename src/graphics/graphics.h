#ifndef BORED_ENGINE_GRAPHICS_H
#define BORED_ENGINE_GRAPHICS_H

#include "window.h"
#include "device.h"

#include <string>
#include <unordered_map>

namespace graphics
{
	enum class GraphicsBackend
	{
		None = 0,
		Metal,
		Vulkan,
		OpenGL,
		WebGPU
	};

	struct GraphicsBackendMetadata
	{
		std::string const name;
	};

	static const std::unordered_map<GraphicsBackend, GraphicsBackendMetadata> graphicsBackendMetadata{
		{GraphicsBackend::None, {"None"}},
		{GraphicsBackend::None, {"Metal"}},
		{GraphicsBackend::None, {"Vulkan"}},
		{GraphicsBackend::None, {"OpenGL"}},
		{GraphicsBackend::None, {"WebGPU"}}
	};

	std::string toString(GraphicsBackend const& backend);

	// main entrypoint for creating a renderer
	[[nodiscard]] std::unique_ptr<IDevice> createDevice(GraphicsBackend const& backend);
}

#endif //BORED_ENGINE_GRAPHICS_H