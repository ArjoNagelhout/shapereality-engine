//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_VECTOR_H
#define BORED_ENGINE_VECTOR_H

#include <array>
#include <sstream>

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

		template<unsigned int resultSize>
		explicit operator Vector<resultSize>()
		{
			std::array<float, resultSize> resultData{};
			std::copy(data.data(), data.data() + std::min(Size, resultSize), resultData.begin());
			return Vector<resultSize>(resultData);
		}

		[[nodiscard]] std::string toString()
		{
			std::stringstream result{"{"};
			for (int i = 0; i < Size; i++)
			{
				result << data[i];
				if (i < Size - 1)
				{
					result << ", ";
				}
			}
			result << "}";
			return result.str();
		}

//		template<
//			unsigned int rhsSize,
//			unsigned int resultSize = std::max(rhsSize, Size)
//		>
//		Vector<resultSize> dot(Vector<rhsSize> const& rhs)
//		{
//			return Vector<resultSize>{};
//		}

		//Vector<Size> cross(Vector<rhsSize>)
	};

	using Vector4 = Vector<4>;
	using vec4 = Vector4;

	using Vector3 = Vector<3>;
	using vec3 = Vector3;

	using Vector2 = Vector<2>;
	using vec2 = Vector2;
}

#endif //BORED_ENGINE_VECTOR_H
