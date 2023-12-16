#ifndef BORED_ENGINE_VECTOR_INL
#define BORED_ENGINE_VECTOR_INL

#include "vector.h"

#include <sstream>

// inline definitions for vector.h

namespace math
{
	template<vector_size_type Size>
	template<vector_size_type ResultSize>
	constexpr Vector<Size>::operator Vector<ResultSize>()
	{
		std::array<float, ResultSize> resultData{};
		std::copy(data.data(), data.data() + std::min(Size, ResultSize), resultData.begin());
		return Vector<ResultSize>(resultData);
	}

	template<vector_size_type Size>
	constexpr vector_size_type Vector<Size>::size() const
	{
		return Size;
	}

	template<vector_size_type Size>
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

	template<vector_size_type Size>
	constexpr float& Vector<Size>::operator[](vector_size_type index)
	{
		return data[index];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::get(vector_size_type index) const
	{
		return data[index];
	}

	template<vector_size_type Size>
	constexpr void Vector<Size>::set(vector_size_type index, float value)
	{
		data[index] = value;
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::x() const requires (Size >= 1)
	{
		return data[0];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::y() const requires (Size >= 2)
	{
		return data[1];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::z() const requires (Size >= 3)
	{
		return data[2];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::w() const requires (Size >= 4)
	{
		return data[3];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::r() const requires (Size >= 1)
	{
		return data[0];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::g() const requires (Size >= 2)
	{
		return data[1];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::b() const requires (Size >= 3)
	{
		return data[2];
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::a() const requires (Size >= 4)
	{
		return data[3];
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::operator*(float rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = data[i] * rhs;
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::operator+(Vector<Size> const& rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = data[i] + rhs.get(i);
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::operator-(Vector<Size> const& rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = data[i] - rhs.get(i);
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::operator/(float rhs) const
	{
		return operator*(1.f / rhs);
	}

	template<vector_size_type Size>
	constexpr void Vector<Size>::operator+=(Vector const& rhs)
	{
		for (int i = 0; i < Size; i++)
		{
			data[i]+=rhs.get(i);
		}
	}

	template<vector_size_type Size>
	constexpr void Vector<Size>::operator-=(Vector const& rhs)
	{
		for (int i = 0; i < Size; i++)
		{
			data[i]-=rhs.get(i);
		}
	}

	template<vector_size_type Size>
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

	template<vector_size_type Size>
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

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::operator-() const
	{
		return *this * -1.f;
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::magnitude() const
	{
		return std::sqrt(magnitudeSquared());
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::magnitudeSquared() const
	{
		float sum = 0.f;
		for (int i = 0; i < Size; i++)
		{
			sum += data[i] * data[i];
		}
		return sum;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::normalized() const
	{
		float _magnitude = magnitude();
		return *this / _magnitude;
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::dot(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		float result = 0.f;
		for (int i = 0; i < Size; i++)
		{
			result += lhs.get(i) * rhs.get(i);
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::cross(Vector<Size> const& lhs, Vector<Size> const& rhs)
	requires (Size == 3)
	{
		return Vector{{
						  lhs.get(1) * rhs.get(2) - lhs.get(2) * rhs.get(1),
						  -(lhs.get(0) * rhs.get(2) - lhs.get(2) * rhs.get(0)),
						  lhs.get(0) * rhs.get(1) - lhs.get(1) * rhs.get(0)
					  }};
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::angle(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		return 0.f;
	}

	template<vector_size_type Size>
	constexpr float Vector<Size>::distance(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		return (lhs - rhs).magnitude();
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::min(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = std::min(lhs.get(i), rhs.get(i));
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::max(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = std::max(lhs.get(i), rhs.get(i));
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::scale(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = lhs.get(i) * rhs.get(i);
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::project(Vector<Size> const& vector, Vector<Size> const& normal)
	{
		return vector;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::projectOnPlane(Vector<Size> const& vector, Vector<Size> const& planeNormal)
	{
		return vector;
	}

	template<vector_size_type Size>
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

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::lerpUnclamped(Vector<Size> const& a, Vector<Size> const& b, float t)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = (a.get(i) * (1.f - t)) + (b.get(i) * t);
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::clamp(Vector<Size> const& vector, Vector<Size> const& min, Vector<Size> const& max)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result[i] = std::clamp(vector.get(i), min.get(i), max.get(i));
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::create(float value)
	{
		Vector result{};
		for (vector_size_type i = 0; i < Size; i++)
		{
			result[i] = value;
		}
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::createUnitVector(vector_size_type index)
	{
		//static_assert(index < Size); // you can't create a unit vector if the index exceeds the size
		Vector result{};
		result[index] = 1.0f;
		return result;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::zero = Vector<Size>();

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::one = Vector<Size>::create(1.f);

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::up = Vector<Size>::createUnitVector(1);

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::down = -Vector<Size>::createUnitVector(1);

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::left = -Vector<Size>::createUnitVector(0);

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::right = Vector<Size>::createUnitVector(0);

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::forward = Vector<Size>::createUnitVector(2);

	template<vector_size_type Size>
	constexpr Vector<Size> Vector<Size>::back = -Vector<Size>::createUnitVector(2);

	template<vector_size_type Size>
	constexpr std::ostream& operator<<(std::ostream& ostream, Vector<Size> const& vector)
	{
		ostream << vector.toString();
		return ostream;
	}

	template<vector_size_type Size>
	constexpr Vector<Size> operator*(float lhs, Vector<Size> const& rhs)
	{
		return rhs * lhs;
	}
}

#endif //BORED_ENGINE_VECTOR_INL