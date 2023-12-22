#ifndef SHAPEREALITY_RECT_H
#define SHAPEREALITY_RECT_H

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

#endif //SHAPEREALITY_RECT_H
