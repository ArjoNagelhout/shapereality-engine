//
// Created by Arjo Nagelhout on 09/12/2023.
//

#ifndef BORED_ENGINE_TYPES_H
#define BORED_ENGINE_TYPES_H

namespace graphics
{
	// a simple color
	struct Color final
	{
		explicit Color(float r, float g, float b, float a);
		~Color();

		float r;
		float g;
		float b;
		float a;

		static Color clear;

		// conversion
	};

	enum class Winding
	{
		Clockwise,
		CounterClockwise
	};
}

#endif //BORED_ENGINE_TYPES_H
