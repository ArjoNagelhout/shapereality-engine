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
		template<unsigned int ResultSize>
		explicit operator Vector<ResultSize>()
		{
			std::array<float, ResultSize> resultData{};
			std::copy(data.data(), data.data() + std::min(Size, ResultSize), resultData.begin());
			return Vector<ResultSize>(resultData);
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

		constexpr Vector operator+(Vector const& rhs) const
		{
			Vector result{};
			for (int i = 0; i < Size; i++)
			{
				result.data[i] = data[i] + rhs.data[i];
			}
			return result;
		}

		constexpr Vector operator-(Vector const& rhs) const
		{
			Vector result{};
			for (int i = 0; i < Size; i++)
			{
				result.data[i] = data[i] - rhs[i];
			}
			return result;
		}

		constexpr Vector operator/(float rhs) const
		{
			return operator* (1 / rhs);
		}

		[[nodiscard]] constexpr float magnitude() const
		{
			float sum = 0.f;
			for (int i = 0; i < Size; i++)
			{
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		[[nodiscard]] constexpr float magnitudeSquared() const
		{
			float sum = 0.f;
			for (int i = 0; i < Size; i++)
			{
				sum += data[i] * data[i];
			}
			return sum;
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

		// cross product is only valid for two three-dimensional vectors.
		template<typename Vector = Vector>
		[[nodiscard]] static std::enable_if_t<(Size == 3), Vector> cross(Vector const& lhs, Vector const& rhs)
		{
			return Vector{{
				lhs[1] * rhs[2] - lhs[2] * rhs[1],
				-(lhs[0] * rhs[2] - lhs[2] * rhs[0]),
				lhs[0] * rhs[1] - lhs[1] * rhs[0]
			}};
		}

		[[nodiscard]] static float angle(Vector const& lhs, Vector const& rhs)
		{
			return 0.f;
		}

		[[nodiscard]] static float distance(Vector const& lhs, Vector const& rhs)
		{
			return (lhs - rhs).magnitude();
		}

		// compute vector containing the smallest components of two vectors
		[[nodiscard]] static Vector min(Vector const& lhs, Vector const& rhs)
		{
			Vector result{};
			for (int i = 0; i < Size; i++)
			{
				result.data[i] = std::min(lhs[i], rhs[i]);
			}
			return result;
		}

		// compute vector containing the largest components of two vectors
		[[nodiscard]] static Vector max(Vector const& lhs, Vector const& rhs)
		{
			Vector result{};
			for (int i = 0; i < Size; i++)
			{
				result.data[i] = std::max(lhs[i], rhs[i]);
			}
			return result;
		}

		// component-wise scale two vectors
		[[nodiscard]] static Vector scale(Vector const& lhs, Vector const& rhs)
		{
			Vector result{};
			for (int i = 0; i < Size; i++)
			{
				result.data[i] = lhs[i] * rhs[i];
			}
			return result;
		}

		// project vector onto a normal vector
		[[nodiscard]] static Vector project(Vector const& vector, Vector const& normal)
		{
			return vector;
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
