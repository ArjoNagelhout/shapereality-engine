//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_VECTOR_H
#define BORED_ENGINE_VECTOR_H

#include <array>

namespace math
{
	// an n-dimensional vector with common math operations
	// for use in a real-time graphics context
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

		// convert this vector to a given size
		template<unsigned int ResultSize>
		explicit operator Vector<ResultSize>()
		{
			std::array<float, ResultSize> resultData{};
			std::copy(data.data(), data.data() + std::min(Size, ResultSize), resultData.begin());
			return Vector<ResultSize>(resultData);
		}

		// get the amount of components this vector has
		constexpr unsigned int size();

		// get formatted string of this vector
		[[nodiscard]] std::string toString();

		// access a component of this vector at a given index
		constexpr float operator[](int index) const;

		// multiply this vector by a float and return the result as a copy
		constexpr Vector operator*(float rhs) const;

		// add a vector to this vector and return the result as a copy
		constexpr Vector operator+(Vector const& rhs) const;

		// subtract a vector from this vector and return the result as a copy
		constexpr Vector operator-(Vector const& rhs) const;

		// divide this vector by a float and return the result as a copy
		constexpr Vector operator/(float rhs) const;

		// get the magnitude of this vector (slow, as it uses a sqrt operation)
		[[nodiscard]] constexpr float magnitude() const;

		// get the squared magnitude of this vector (faster, as it does not use a sqrt operation)
		[[nodiscard]] constexpr float magnitudeSquared() const;

		// get the dot product of two vectors
		[[nodiscard]] static float dot(Vector const& lhs, Vector const& rhs);

		// get the cross product of two vectors
		// note: only valid for three-dimensional vectors.
		[[nodiscard]] static Vector cross(Vector const& lhs, Vector const& rhs) requires (Size == 3);

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

		[[nodiscard]] static Vector projectOnPlane(Vector const& vector, Vector const& )

		// linearly interpolate between two vectors
		// returns `a` if t <= 0.f and returns `b` if t >= 1.f
		[[nodiscard]] static Vector lerp(Vector const& a, Vector const& b, float t);

		// linearly interpolate between two vectors
		[[nodiscard]] static Vector lerpUnclamped(Vector const& a, Vector const& b, float t);
	};

	// multiply float by a vector (instead of the other way around)
	template<unsigned int Size>
	constexpr Vector<Size> operator*(float lhs, Vector<Size> const& rhs);

	// shorthand forms
	using Vector4 = Vector<4>;
	using vec4 = Vector4;

	using Vector3 = Vector<3>;
	using vec3 = Vector3;

	using Vector2 = Vector<2>;
	using vec2 = Vector2;
}

#endif //BORED_ENGINE_VECTOR_H
