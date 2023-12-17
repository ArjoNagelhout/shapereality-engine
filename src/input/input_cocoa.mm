//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "input.h"

namespace input
{
	struct Input::Implementation
	{

	};

	Input::Input()
	{
		pImplementation = std::make_unique<Implementation>();
	}

	Input::~Input()
	{

	}
}