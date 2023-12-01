#ifndef BORED_ENGINE_VECTOR_INL
#define BORED_ENGINE_VECTOR_INL

#include "vector.h"

#include <sstream>

// inline definitions for vector.h

namespace math
{
	template<unsigned int Size>
	constexpr unsigned int Vector<Size>::size()
	{
		return Size;
	}

	template<unsigned int Size>
	std::string Vector<Size>::toString() const
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

	template<unsigned int Size>
	constexpr float& Vector<Size>::operator[](int index)
	{
		return data[index];
	}

	template<unsigned int Size>
	constexpr float Vector<Size>::get(int index) const
	{
		return data[index];
	}

	template<unsigned int Size>
	constexpr void Vector<Size>::set(int index, float value)
	{
		data[index] = value;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator*(float rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = data[i] * rhs;
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator+(Vector<Size> const& rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = data[i] + rhs.get(i);
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator-(Vector<Size> const& rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = data[i] - rhs.get(i);
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator/(float rhs) const
	{
		return operator*(1.f / rhs);
	}

	template<unsigned int Size>
	constexpr bool Vector<Size>::operator==(Vector<Size> const& rhs) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (std::abs(data[i] - rhs.get(i)) > epsilon)
			{
				return false;
			}
		}
		return true;
	}

	template<unsigned int Size>
	constexpr bool Vector<Size>::operator!=(Vector<Size> const& rhs) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (std::abs(data[i] - rhs.get(i)) > epsilon)
			{
				return true;
			}
		}
		return false;
	}

	template<unsigned int Size>
	constexpr float Vector<Size>::magnitude() const
	{
		return std::sqrt(magnitudeSquared());
	}

	template<unsigned int Size>
	constexpr float Vector<Size>::magnitudeSquared() const
	{
		float sum = 0.f;
		for (int i = 0; i < Size; i++)
		{
			sum += data[i] * data[i];
		}
		return sum;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::normalized() const
	{
		float _magnitude = magnitude();
		return *this / _magnitude;
	}

	template<unsigned int Size>
	constexpr float Vector<Size>::dot(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		float result = 0.f;
		for (int i = 0; i < Size; i++)
		{
			result += lhs.get(i) * rhs.get(i);
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::cross(Vector<Size> const& lhs, Vector<Size> const& rhs)
	requires (Size == 3)
	{
		return Vector{{
						  lhs.get(1) * rhs.get(2) - lhs.get(2) * rhs.get(1),
						  -(lhs.get(0) * rhs.get(2) - lhs.get(2) * rhs.get(0)),
						  lhs.get(0) * rhs.get(1) - lhs.get(1) * rhs.get(0)
					  }};
	}

	template<unsigned int Size>
	constexpr float Vector<Size>::angle(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		return 0.f;
	}

	template<unsigned int Size>
	constexpr float Vector<Size>::distance(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		return (lhs - rhs).magnitude();
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::min(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = std::min(lhs.get(i), rhs.get(i));
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::max(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = std::max(lhs.get(i), rhs.get(i));
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::scale(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = lhs.get(i) * rhs.get(i);
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::project(Vector<Size> const& vector, Vector<Size> const& normal)
	{
		return vector;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::projectOnPlane(Vector<Size> const& vector, Vector<Size> const& planeNormal)
	{
		return vector;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::lerp(Vector<Size> const& a, Vector<Size> const& b, float t)
	{
		if (t <= 0.f)
		{
			return a;
		}
		else if (t >= 1.f)
		{
			return b;
		}

		return lerpUnclamped(a, b, t);
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::lerpUnclamped(Vector<Size> const& a, Vector<Size> const& b, float t)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = (a.get(i) * (1.f - t)) + (b.get(i) * t);
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::clamp(Vector<Size> const& vector, Vector<Size> const& min, Vector<Size> const& max)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = std::clamp(vector.get(i), min.get(i), max.get(i));
		}
		return result;
	}
}

#endif //BORED_ENGINE_VECTOR_INL