#ifndef SHAPEREALITY_VECTOR_INL
#define SHAPEREALITY_VECTOR_INL

#include "vector.h"

#include <sstream>
#include <algorithm>

// inline definitions for vector.h

namespace math
{
#if (defined(VECTOR_TEMPLATE) or (defined(VECTOR_TYPE)))
    static_assert(false);
#endif
#define VECTOR_TEMPLATE template<SizeType Size, typename Type>
#define VECTOR_TYPE Vector<Size, Type>

    //--------------------------------
    // Construct, copy, move, destruct
    //--------------------------------

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE::Vector() = default;

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE::Vector(std::initializer_list<Type> data_)
    {
        std::copy_n(data_.begin(), data_.size(), data.begin());
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE::Vector(Vector&& other) noexcept
        : data(std::move(other.data))
    {
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE& VECTOR_TYPE::operator=(Vector&& other) noexcept
    {
        data = std::move(other.data);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE::Vector(Vector const& other)
        : data(other.data)
    {
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE& VECTOR_TYPE::operator=(Vector const& other)
    {
        data = other.data;
        return *this;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE::~Vector() = default;

    //-----------
    // Conversion
    //-----------

    VECTOR_TEMPLATE
    template<SizeType ResultSize>
    constexpr VECTOR_TYPE::operator Vector<ResultSize, Type>()
    {
        Vector<ResultSize, Type> result;
        std::copy_n(data.begin(), std::min(Size, ResultSize), result.data.begin());
        return result;
    }

    //-----------
    // Properties
    //-----------

    VECTOR_TEMPLATE
    constexpr SizeType VECTOR_TYPE::size()
    {
        return Size;
    }

    //-------
    // Access
    //-------

    VECTOR_TEMPLATE
    constexpr Type& VECTOR_TYPE::operator[](SizeType index)
    {
        return data[index];
    }

    VECTOR_TEMPLATE
    constexpr Type const& VECTOR_TYPE::operator[](SizeType index) const
    {
        return data[index];
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::get(SizeType index) const
    {
        return data[index];
    }

    VECTOR_TEMPLATE
    constexpr void VECTOR_TYPE::set(SizeType index, Type value)
    {
        data[index] = value;
    }

    //---------
    // Equality
    //---------

    VECTOR_TEMPLATE
    constexpr bool VECTOR_TYPE::operator==(Vector const& other) const
    {
        return data == other.data;
    }

    VECTOR_TEMPLATE
    constexpr bool VECTOR_TYPE::operator!=(Vector const& other) const
    {
        return data != other.data;
    }

    VECTOR_TEMPLATE
    constexpr bool VECTOR_TYPE::approximatelyEquals(Vector const& other)
    requires (std::is_same_v<Type, float> || std::is_same_v<Type, double>)
    {
        Type epsilon;
        if constexpr (std::is_same_v<Type, float>)
        {
            epsilon = vectorEpsilonFloat;
        }
        else if constexpr (std::is_same_v<Type, double>)
        {
            epsilon = vectorEpsilonDouble;
        }

        for (SizeType i = 0; i < size(); i++)
        {
            if (std::abs(data[i] - other.data_[i]) > epsilon)
            {
                return false;
            }
        }
        return true;
    }

    //----------
    // Operators
    //----------

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::operator+(Vector const& other) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = data[i] + other[i]; });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::operator-(Vector const& other) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = data[i] - other[i]; });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::operator*(Type scalar) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = data[i] * scalar; });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::operator/(Type scalar) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = data[i] / scalar; });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr void VECTOR_TYPE::operator+=(Vector const& other)
    {
        forEach([&](SizeType i) { this->operator[](i) += other[i]; });
    }

    VECTOR_TEMPLATE
    constexpr void VECTOR_TYPE::operator-=(Vector const& other)
    {
        forEach([&](SizeType i) { this->operator[](i) -= other[i]; });
    }

    VECTOR_TEMPLATE
    constexpr void VECTOR_TYPE::operator*=(Type scalar)
    {
        forEach([&](SizeType i) { this->operator[](i) *= scalar; });
    }

    VECTOR_TEMPLATE
    constexpr void VECTOR_TYPE::operator/=(Type scalar)
    {
        return operator*(1.0 / scalar);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::operator-() const
    {
        forEach([&](SizeType i) { this->operator[](i) = -this->operator[](i); });
    }

    //-----------------
    // Member functions
    //-----------------

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::magnitude() const
    {
        return std::sqrt(magnitudeSquared());
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::magnitudeSquared() const
    {
        Type sum = 0;
        forEach([&](SizeType i) { sum += data[i] * data[i]; });
        return sum;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::normalized() const
    {
        return *this / magnitude();
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::dot(Vector const& other) const
    {
        Type result = 0;
        forEach([&](SizeType i) { result += this->operator[](i) * other[i]; });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::cross(Vector const& other) const
    requires (Size == 3)
    {
        return {
            get(1) * other[2] - get(2) * other[1],
            -(get(0) * other[2] - get(2) * other[0]),
            get(0) * other[1] - get(1) * other[0]
        };
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::angle(Vector const& other) const
    {
        // todo
        return {};
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::distance(Vector const& other) const
    {
        return (*this - other).magnitude();
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::min(Vector const& other) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = std::min(get(i), other[i]); });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::max(Vector const& other) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = std::max(get(i), other[i]); });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::scale(Vector const& other) const
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = get(i) * other[i]; });
        return result;
    }

    //-------
    // Static
    //-------

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::dot(Vector const& lhs, Vector const& rhs)
    {
        return lhs.dot(rhs);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::cross(Vector const& lhs, Vector const& rhs)
    requires (Size == 3)
    {
        return lhs.cross(rhs);
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::angle(Vector const& lhs, Vector const& rhs)
    {
        return lhs.angle(rhs);
    }

    VECTOR_TEMPLATE
    constexpr Type VECTOR_TYPE::distance(Vector const& lhs, Vector const& rhs)
    {
        return lhs.distance(rhs);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::min(Vector const& lhs, Vector const& rhs)
    {
        return lhs.min(rhs);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::max(Vector const& lhs, Vector const& rhs)
    {
        return lhs.max(rhs);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::scale(Vector const& lhs, Vector const& rhs)
    {
        return lhs.scale(rhs);
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::project(Vector const& vector, Vector const& normal)
    {
        // todo
        return {};
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::projectOnPlane(Vector const& vector, Vector const& planeNormal)
    {
        // todo
        return {};
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::lerp(Vector const& a, Vector const& b, Type t)
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

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::lerpUnclamped(Vector const& a, Vector const& b, Type t)
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = (a.get(i) * (1.f - t)) + (b.get(i) * t); });
        return result;
    }

    VECTOR_TEMPLATE
    constexpr VECTOR_TYPE VECTOR_TYPE::clamp(Vector const& vector, Vector const& min, Vector const& max)
    {
        Vector result;
        forEach([&](SizeType i) { result[i] = std::clamp(vector.get(i), min.get(i), max.get(i)); });
        return result;
    }

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::zero = Vector{};

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::up = Vector{0, 1, 0};

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::down = Vector{0, -1, 0};

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::left = Vector{-1, 0, 0};

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::right = Vector{1, 0, 0};

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::forward = Vector{0, 0, 1};

    VECTOR_TEMPLATE
    VECTOR_TYPE const VECTOR_TYPE::back = Vector{0, 0, -1};

    //--------
    // Private
    //--------

    VECTOR_TEMPLATE
    template<typename Function>
    void VECTOR_TYPE::forEach(Function&& function)
    {
        for (SizeType i = 0; i < Size; i++)
        {
            function(i);
        }
    }

    template<SizeType Size, typename Type>
    constexpr Vector<Size, Type> operator*(Type lhs, Vector<Size, Type> const& rhs)
    {
        return rhs * lhs;
    }

#undef VECTOR_TEMPLATE
#undef VECTOR_TYPE
}

#endif //SHAPEREALITY_VECTOR_INL