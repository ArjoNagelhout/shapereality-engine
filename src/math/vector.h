//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_VECTOR_H
#define BORED_ENGINE_VECTOR_H

#include <array>

namespace math
{
	template<unsigned int Size>
	class Vector
	{
	public:
		explicit Vector() = default;
		explicit Vector(std::array<float, Size> _data) : data(_data)
		{}

		~Vector() = default;

		std::array<float, Size> data{};

		constexpr unsigned int size()
		{
			return Size;
		}
	};

	using Vector4 = Vector<4>;
	using vec4 = Vector4;

	using Vector3 = Vector<3>;
	using vec3 = Vector3;

	using Vector2 = Vector<2>;
	using vec2 = Vector2;
}

#endif //BORED_ENGINE_VECTOR_H
