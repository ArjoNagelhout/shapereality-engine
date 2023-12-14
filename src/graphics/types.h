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

	enum class WindingOrder
	{
		Clockwise,
		CounterClockwise
	};

	// The geometric primitive type for drawing commands.
	enum class PrimitiveType
	{
		// Rasterize a point at each vertex. The vertex shader must provide [[point_size]],
		// or the point size is undefined.
		Point,

		// Rasterize a line between each separate pair of vertices, resulting in a series of
		// unconnected lines. If there are an odd number of vertices, the last vertex is ignored.
		Line,

		// Rasterize a line between each pair of adjacent vertices, resulting in a series of
		// connected lines (also called a polyline).
		LineStrip,

		// For every separate set of three vertices, rasterize a triangle. If the number of
		// vertices is not a multiple of three, either one or two vertices is ignored.
		Triangle,

		// For every three adjacent vertices, rasterize a triangle.
		TriangleStrip
	};
}

#endif //BORED_ENGINE_TYPES_H
