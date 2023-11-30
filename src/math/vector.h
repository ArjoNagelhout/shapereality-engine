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
	private:
		std::array<float, Size> data{};

	public:
		explicit Vector() = default;

		explicit Vector(std::array<float, Size> _data) : data(_data)
		{}

		~Vector() = default;

		constexpr unsigned int size()
		{
			return Size;
		}

		[[nodiscard]] std::string toString()
		{
			std::stringstream result{};
			result << "{";
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

		// static_cast vector conversions
		template<unsigned int resultSize>
		explicit operator Vector<resultSize>()
		{
			std::array<float, resultSize> resultData{};
			std::copy(data.data(), data.data() + std::min(Size, resultSize), resultData.begin());
			return Vector<resultSize>(resultData);
		}

		constexpr float operator[](int index) const
		{
			return data[index];
		}

		constexpr Vector operator*(float rhs) const
		{
			Vector result{};
			for (int i = 0; i < Size; i++)
			{
				result.data[i] = data[i] * rhs;
			}
			return result;
		}

		constexpr Vector operator/(float rhs)
		{
			return operator* (1 / rhs);
		}

		[[nodiscard]] static float dot(Vector const& lhs, Vector const& rhs)
		{
			float result = 0.f;
			for (int i = 0; i < Size; i++)
			{
				result += lhs[i] * rhs[i];
			}
			return result;
		}

		[[nodiscard]]
		static std::enable_if_t<(Size >= 3), Vector> cross(Vector const& lhs, Vector const& rhs)
		{
			return Vector{{
				lhs[1] * rhs[2] - lhs[2] * rhs[1],
				-(lhs[0] * rhs[2] - lhs[2] * rhs[0]),
				lhs[0] * rhs[1] - lhs[1] * rhs[0]
			}};
		}
	};

	template<unsigned int Size>
	constexpr Vector<Size> operator*(float lhs, Vector<Size> const& rhs)
	{
		return rhs * lhs;
	}

	using Vector4 = Vector<4>;
	using vec4 = Vector4;

	using Vector3 = Vector<3>;
	using vec3 = Vector3;

	using Vector2 = Vector<2>;
	using vec2 = Vector2;
}

#endif //BORED_ENGINE_VECTOR_H
