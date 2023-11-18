//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../mtl_window.h"

namespace renderer
{
	MetalWindowImplementation::MetalWindowImplementation(Window* window) : WindowRendererImplementation(window)
	{
	}

	MetalWindowImplementation::~MetalWindowImplementation() = default;
}