//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_VECTOR_H
#define SHAPEREALITY_VECTOR_H

#include <array>
#include <algorithm>

#include "config.h"

namespace math
{
    // an n-dimensional vector with common math operations
    // for use in a real-time graphics context
    template<SizeType Size, typename Type>
    struct Vector final
    {
        template<SizeType OtherSize, typename Type_>
        friend class Vector;

        //--------------------------------
        // Construct, copy, move, destruct
        //--------------------------------

        // construct with all zeros
        constexpr explicit Vector();

        // construct from initializer list
        constexpr Vector(std::initializer_list<Type> data_);

        // move constructor
        constexpr Vector(Vector&& other) noexcept;

        // move assignment operator
        constexpr Vector& operator=(Vector&& other) noexcept;

        // copy constructor
        constexpr Vector(Vector const& other);

        // copy assignment operator
        constexpr Vector& operator=(Vector const& other);

        constexpr ~Vector();

        //-----------
        // Conversion
        //-----------

        // explicit conversion to a different vector size
        template<SizeType ResultSize>
        constexpr explicit operator Vector<ResultSize, Type>();

        //-----------
        // Properties
        //-----------

        // get amount of components of this vector
        [[nodiscard]] constexpr static SizeType size();

        //-------
        // Access
        //-------

        // get reference to component at index
        [[nodiscard]] constexpr Type& operator[](SizeType index);

        // get const reference to component at index
        [[nodiscard]] constexpr Type const& operator[](SizeType index) const;

        // get value of component at index
        [[nodiscard]] constexpr Type get(SizeType index) const;

        // set value of component at index
        constexpr void set(SizeType index, Type value);

        //---------
        // Equality
        //---------

        // get whether this vector is exactly equal to the given vector
        [[nodiscard]] constexpr bool operator==(Vector const& other) const;

        // get whether this vector is not exactly equal to the given vector
        [[nodiscard]] constexpr bool operator!=(Vector const& other) const;

        // approximate equality using epsilon
        [[nodiscard]] constexpr bool approximatelyEquals(Vector const& other)
        requires (std::is_same_v<Type, float> || std::is_same_v<Type, double>);

        //----------
        // Operators
        //----------

        // component-wise add a vector to this vector and return the result as a copy
        [[nodiscard]] constexpr Vector operator+(Vector const& other) const;

        // component-wise subtract a vector from this vector and return the result as a copy
        [[nodiscard]] constexpr Vector operator-(Vector const& other) const;

        // multiply this vector by a scalar and return the result as a copy
        [[nodiscard]] constexpr Vector operator*(Type scalar) const;

        // divide this vector by a scalar and return the result as a copy
        [[nodiscard]] constexpr Vector operator/(Type scalar) const;

        // add a vector to this vector in place
        constexpr void operator+=(Vector const& other);

        // subtract a vector from this vector in place
        constexpr void operator-=(Vector const& other);

        // multiply this vector by a scalar in place
        constexpr void operator*=(Type scalar);

        // divide this vector by a scalar in place
        constexpr void operator/=(Type scalar);

        // get a negative copy of this vector
        constexpr Vector operator-() const;

        //-----------------
        // Member functions
        //-----------------

        // get the magnitude of this vector (slow, as it uses a sqrt operation)
        [[nodiscard]] constexpr Type magnitude() const;

        // get the squared magnitude of this vector (faster, as it does not use a sqrt operation)
        [[nodiscard]] constexpr Type magnitudeSquared() const;

        // get normalized vector of this vector
        // returns a vector with magnitude 1 with the same direction as this vector
        [[nodiscard]] constexpr Vector normalized() const;

        // get the dot product of two vectors
        [[nodiscard]] constexpr Type dot(Vector const& other) const;

        // get the cross product of two vectors
        // note: only valid for three-dimensional vectors.
        [[nodiscard]] constexpr Vector cross(Vector const& other) const
        requires (Size == 3);

        // get angle between two vectors
        [[nodiscard]] constexpr Type angle(Vector const& other) const;

        // get distance between two vectors
        [[nodiscard]] constexpr Type distance(Vector const& other) const;

        // get vector containing the smallest components of two vectors
        [[nodiscard]] constexpr Vector min(Vector const& other) const;

        // get vector containing the largest components of two vectors
        [[nodiscard]] constexpr Vector max(Vector const& other) const;

        // component-wise scale two vectors
        [[nodiscard]] constexpr Vector scale(Vector const& other) const;

        //-------
        // Static
        //-------

        // get the dot product of two vectors
        [[nodiscard]] constexpr static Type dot(Vector const& lhs, Vector const& rhs);

        // get the cross product of two vectors
        // note: only valid for three-dimensional vectors.
        [[nodiscard]] constexpr static Vector cross(Vector const& lhs, Vector const& rhs)
        requires (Size == 3);

        // get angle between two vectors
        [[nodiscard]] constexpr static Type angle(Vector const& lhs, Vector const& rhs);

        // get distance between two vectors
        [[nodiscard]] constexpr static Type distance(Vector const& lhs, Vector const& rhs);

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
        [[nodiscard]] constexpr static Vector lerp(Vector const& a, Vector const& b, Type t);

        // linearly interpolate between two vectors
        // interpolates beyond t<= 0.f and t >= 1.f
        [[nodiscard]] constexpr static Vector lerpUnclamped(Vector const& a, Vector const& b, Type t);

        // clamp a vector component-wise between two vectors min and max
        [[nodiscard]] constexpr static Vector clamp(Vector const& vector, Vector const& min, Vector const& max);

        static Vector const zero;

        // only valid for Vector<3, Type>
        static Vector const up;
        static Vector const down;
        static Vector const left;
        static Vector const right;
        static Vector const forward;
        static Vector const back;

    private:
        std::array<Type, Size> data;

        template<typename Function>
        static void forEach(Function&& function);
    };

    template<SizeType Size, typename Type>
    [[nodiscard]] constexpr Vector<Size, Type> operator*(Type lhs, Vector<Size, Type> const& rhs);
}

#endif //SHAPEREALITY_VECTOR_H
