#ifndef BORED_ENGINE_RECT_H
#define BORED_ENGINE_RECT_H

namespace math
{
	struct Rect final
	{
		explicit Rect(float x, float y, float width, float height);

		~Rect();

		float x;
		float y;
		float width;
		float height;
	};
}

#endif //BORED_ENGINE_RECT_H
