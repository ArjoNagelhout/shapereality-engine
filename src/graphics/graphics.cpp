#include <cassert>
#include "graphics.h"
#include "device.h"
#include "window.h"
#include "backends/metal/mtl_device.h"

namespace graphics
{
	std::unique_ptr<IWindow> createWindow(GraphicsBackend const& graphicsBackend)
	{
		switch (graphicsBackend)
		{
//			case GraphicsBackend::Metal:
//				return nullptr;
			default:
				return nullptr;
		}
	}

	std::unique_ptr<IDevice> createDevice(GraphicsBackend const& graphicsBackend)
	{
		switch (graphicsBackend)
		{
			case GraphicsBackend::Metal:
				return std::make_unique<MetalDevice>();
			default:
				return nullptr;
		}
	}
}