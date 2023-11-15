#include "view_cocoa.h"

namespace engine
{
	View::View()
	{
		pImpl = std::make_unique<Implementation>();
	}

	View::~View() = default;
}