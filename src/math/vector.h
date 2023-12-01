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
		constexpr explicit Vector() = default;

		constexpr explicit Vector(std::array<float, Size> _data) : data(_data)
		{}

		constexpr ~Vector() = default;

		// epsilon for equality operators
		constexpr static const float epsilon = 1e-5;

		// convert this vector to a given size
		template<unsigned int ResultSize>
		constexpr explicit operator Vector<ResultSize>()
		{
			std::array<float, ResultSize> resultData{};
			std::copy(data.data(), data.data() + std::min(Size, ResultSize), resultData.begin());
			return Vector<ResultSize>(resultData);
		}

		// get the amount of components this vector has
		constexpr unsigned int size();

		// get formatted string of this vector
		[[nodiscard]] std::string toString() const;

		// access a component of this vector at a given index
		// note: as this returns a reference, the value can be
		// altered. If this is undesirable (e.g. to keep a function const, use `get()`)
		constexpr float& operator[](int index);

		// get a component of this vector at a given index.
		// note: does not allow changing the value in place.
		// use the subscript operator [] to directly change the value
		// in place
		constexpr float get(int index) const;

		// set a component of this vector at a given index to the given value
		constexpr void set(int index, float value);

		// multiply this vector by a float and return the result as a copy
		constexpr Vector operator*(float rhs) const;

		// add a vector to this vector and return the result as a copy
		constexpr Vector operator+(Vector const& rhs) const;

		// subtract a vector from this vector and return the result as a copy
		constexpr Vector operator-(Vector const& rhs) const;

		// divide this vector by a float and return the result as a copy
		constexpr Vector operator/(float rhs) const;

		// get whether this vector is roughly equal to a given vector
		// uses `epsilon` to mitigate floating point imprecision
		constexpr bool operator==(Vector const& rhs) const;

		// get whether this vector is not roughly equal to a given vector
		constexpr bool operator!=(Vector const& rhs) const;

		// get the magnitude of this vector (slow, as it uses a sqrt operation)
		[[nodiscard]] constexpr float magnitude() const;

		// get the squared magnitude of this vector (faster, as it does not use a sqrt operation)
		[[nodiscard]] constexpr float magnitudeSquared() const;

		// get normalized vector of this vector
		// returns a vector with magnitude 1 with the same direction as this vector
		[[nodiscard]] constexpr Vector normalized() const;

		// get the dot product of two vectors
		[[nodiscard]] constexpr static float dot(Vector const& lhs, Vector const& rhs);

		// get the cross product of two vectors
		// note: only valid for three-dimensional vectors.
		[[nodiscard]] constexpr static Vector cross(Vector const& lhs, Vector const& rhs)
		requires (Size == 3);

		// get angle between two vectors
		[[nodiscard]] constexpr static float angle(Vector const& lhs, Vector const& rhs);

		// get distance between two vectors
		[[nodiscard]] constexpr static float distance(Vector const& lhs, Vector const& rhs);

		// get vector containing the smallest components of two vectors
		[[nodiscard]] constexpr static Vector min(Vector const& lhs, Vector const& rhs);

		// get vector containing the largest components of two vectors
		[[nodiscard]] constexpr static Vector max(Vector const& lhs, Vector const& rhs);

		// component-wise scale two vectors
		[[nodiscard]] constexpr static Vector scale(Vector const& lhs, Vector const& rhs);

		// project vector onto a normal vector
		[[nodiscard]] constexpr static Vector project(Vector const& vector, Vector const& normal);

		// project a vector onto a plane that goes through the origin (0, 0, 0) with a given normal
		[[nodiscard]] constexpr static Vector projectOnPlane(Vector const& vector, Vector const& planeNormal);

		// linearly interpolate between two vectors
		// returns `a` if t <= 0.f and returns `b` if t >= 1.f
		[[nodiscard]] constexpr static Vector lerp(Vector const& a, Vector const& b, float t);

		// linearly interpolate between two vectors
		// interpolates beyond t<= 0.f and t >= 1.f
		[[nodiscard]] constexpr static Vector lerpUnclamped(Vector const& a, Vector const& b, float t);

		// clamp a vector component-wise between two vectors min and max
		[[nodiscard]] constexpr static Vector clamp(Vector const& vector, Vector const& a, Vector const& b);
	};

	template<unsigned int Size>
	constexpr std::ostream& operator<<(std::ostream& ostream, Vector<Size> const& vector)
	{
		ostream << vector.toString();
		return ostream;
	}

	// multiply float by a vector (instead of the other way around)
	template<unsigned int Size>
	constexpr Vector<Size> operator*(float lhs, Vector<Size> const& rhs)
	{
		return rhs * lhs;
	}

	// shorthand forms
	using vec2 = Vector<2>;
	using vec3 = Vector<3>;
	using vec4 = Vector<4>;
}

#endif //BORED_ENGINE_VECTOR_H
