#include <cassert>
#include "graphics.h"
#include "device.h"
#include "backends/metal/mtl_device.h"

namespace graphics
{
	std::unique_ptr<IDevice> createDevice(GraphicsBackend const& backend)
	{
		switch (backend)
		{
			case GraphicsBackend::Metal:
				return std::make_unique<MetalDevice>();
			default:
				return nullptr;
		}
	}
}