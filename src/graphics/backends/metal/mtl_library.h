//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_MTL_LIBRARY_H
#define BORED_ENGINE_MTL_LIBRARY_H

#include "../../library.h"

namespace graphics
{
	class MetalLibrary final : public ILibrary
	{
	public:
		explicit MetalLibrary();
		~MetalLibrary() override;

	private:

	};
}

#endif //BORED_ENGINE_MTL_LIBRARY_H
