//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_VECTOR_H
#define BORED_ENGINE_VECTOR_H

#include <array>

namespace math
{
	// vector type, including math operations
	template<unsigned int Size>
	class Vector final
	{
	private:
		std::array<float, Size> data{};

	public:
		explicit Vector() = default;

		explicit Vector(std::array<float, Size> _data) : data(_data)
		{}

		~Vector() = default;

		constexpr unsigned int size();

		[[nodiscard]] std::string toString();

		// static_cast vector conversions
		template<unsigned int ResultSize>
		explicit operator Vector<ResultSize>();

		constexpr float operator[](int index) const;
		constexpr Vector operator*(float rhs) const;
		constexpr Vector operator+(Vector const& rhs) const;
		constexpr Vector operator-(Vector const& rhs) const;
		constexpr Vector operator/(float rhs) const;

		[[nodiscard]] constexpr float magnitude() const;
		[[nodiscard]] constexpr float magnitudeSquared() const;

		// get the dot product of two vectors
		[[nodiscard]] static float dot(Vector const& lhs, Vector const& rhs);

		// get the cross product of two vectors
		// note: only valid for two three-dimensional vectors.
		template<typename Vector = Vector>
		[[nodiscard]] static std::enable_if_t<(Size == 3), Vector> cross(Vector const& lhs, Vector const& rhs)
		{
			return Vector{{
				lhs[1] * rhs[2] - lhs[2] * rhs[1],
				-(lhs[0] * rhs[2] - lhs[2] * rhs[0]),
				lhs[0] * rhs[1] - lhs[1] * rhs[0]
			}};
		}

		// get angle between two vectors
		[[nodiscard]] static float angle(Vector const& lhs, Vector const& rhs);

		// get distance between two vectors
		[[nodiscard]] static float distance(Vector const& lhs, Vector const& rhs);

		// get vector containing the smallest components of two vectors
		[[nodiscard]] static Vector min(Vector const& lhs, Vector const& rhs);

		// get vector containing the largest components of two vectors
		[[nodiscard]] static Vector max(Vector const& lhs, Vector const& rhs);

		// component-wise scale two vectors
		[[nodiscard]] static Vector scale(Vector const& lhs, Vector const& rhs);

		// project vector onto a normal vector
		[[nodiscard]] static Vector project(Vector const& vector, Vector const& normal);
	};

	// multiply float by a vector
	template<unsigned int Size>
	constexpr Vector<Size> operator*(float lhs, Vector<Size> const& rhs);

	// shorthand forms of writing common types
	using Vector4 = Vector<4>;
	using vec4 = Vector4;

	using Vector3 = Vector<3>;
	using vec3 = Vector3;

	using Vector2 = Vector<2>;
	using vec2 = Vector2;
}

#endif //BORED_ENGINE_VECTOR_H
