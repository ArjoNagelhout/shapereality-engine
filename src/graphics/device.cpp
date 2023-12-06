//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "device.h"

namespace graphics
{
	std::unique_ptr<IWindow> IDevice::createWindow(WindowDescription description) const
	{
		return {};
	}
}