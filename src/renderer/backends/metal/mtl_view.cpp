#include "mtl_view.h"

#include <iostream>

namespace renderer
{
	MetalView::MetalView()
	{
		std::cout << "new metal view" << std::endl;
	}

	MetalView::~MetalView()
	{
		std::cout << "destroy metal view" << std::endl;
	}
}