//
// Created by Arjo Nagelhout on 16/11/2023.
//

#ifndef BORED_ENGINE_MTL_TYPES_H
#define BORED_ENGINE_TYPES_H

// simple types

namespace engine
{
	enum class Space
	{
		World = 0, // Applies transformation relative to the world coordinate system.
		Self // Applies transformation relative to the local coordinate system.
	};
}

#endif //BORED_ENGINE_MTL_TYPES_H
