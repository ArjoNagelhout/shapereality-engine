//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_WINDOW_H
#define BORED_ENGINE_MTL_WINDOW_H

#include "../../window.h"

namespace graphics
{
	class MetalWindow final : public WindowRendererImplementation
	{
	public:
		explicit MetalWindow(Window* window);
		~MetalWindow() override;

	private:
		// wild that we have a implementation inside an implementation
		// but, otherwise we'd poison the rest of the codebase (such as windows.cpp) with
		// Objective-C++ as a requirement for the compiler.
		class Implementation;

		std::unique_ptr<Implementation> pImplementation;
	};
}

#endif //BORED_ENGINE_MTL_WINDOW_H
