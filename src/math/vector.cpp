#include "vector.h"

#include <sstream>

namespace math
{
	template<unsigned int Size>
	constexpr unsigned int Vector<Size>::size()
	{
		return Size;
	}

	template<unsigned int Size>
	std::string Vector<Size>::toString()
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
	constexpr float Vector<Size>::operator[](int index) const
	{
		return data[index];
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator*(float rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result.data[i] = data[i] * rhs;
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator+(Vector<Size> const& rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result.data[i] = data[i] + rhs.data[i];
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator-(Vector<Size> const& rhs) const
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result.data[i] = data[i] - rhs[i];
		}
		return result;
	}

	template<unsigned int Size>
	constexpr Vector<Size> Vector<Size>::operator/(float rhs) const
	{
		return operator* (1.f / rhs);
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
	float Vector<Size>::dot(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		float result = 0.f;
		for (int i = 0; i < Size; i++)
		{
			result += lhs[i] * rhs[i];
		}
		return result;
	}

	template<unsigned int Size>
	Vector<Size> Vector<Size>::cross(Vector const& lhs, Vector const& rhs) requires (Size == 3)
	{
		return Vector{{
			lhs[1] * rhs[2] - lhs[2] * rhs[1],
			-(lhs[0] * rhs[2] - lhs[2] * rhs[0]),
			lhs[0] * rhs[1] - lhs[1] * rhs[0]
		}};
	}

	template<unsigned int Size>
	float Vector<Size>::angle(Vector const& lhs, Vector const& rhs)
	{
		return 0.f;
	}

	template<unsigned int Size>
	float Vector<Size>::distance(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		return (lhs - rhs).magnitude();
	}

	template<unsigned int Size>
	Vector<Size> Vector<Size>::min(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result.data[i] = std::min(lhs[i], rhs[i]);
		}
		return result;
	}

	template<unsigned int Size>
	Vector<Size> Vector<Size>::max(Vector const& lhs, Vector const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result.data[i] = std::max(lhs[i], rhs[i]);
		}
		return result;
	}

	template<unsigned int Size>
	Vector<Size> Vector<Size>::scale(Vector<Size> const& lhs, Vector<Size> const& rhs)
	{
		Vector<Size> result{};
		for (int i = 0; i < Size; i++)
		{
			result.data[i] = lhs[i] * rhs[i];
		}
		return result;
	}

	template<unsigned int Size>
	Vector<Size> Vector<Size>::project(Vector<Size> const& vector, Vector<Size> const& normal)
	{
		return vector;
	}

	template<unsigned int Size>
	constexpr Vector<Size> operator*(float lhs, Vector<Size> const& rhs)
	{
		return rhs * lhs;
	}

	// we know we only need the following vector types.
	// this allows the implementations to not be included
	// for each file that includes the vector.h template
	template class Vector<2>;
	template class Vector<3>;
	template class Vector<4>;
}