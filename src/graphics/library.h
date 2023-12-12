//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_LIBRARY_H
#define BORED_ENGINE_LIBRARY_H

namespace graphics
{
	// contains shader source code, either loaded from a file
	class ILibrary
	{
	public:
		virtual ~ILibrary() = default;
	};
}

#endif //BORED_ENGINE_LIBRARY_H
