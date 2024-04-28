//
// Created by Arjo Nagelhout on 16/11/2023.
//

#ifndef SHAPEREALITY_QUATERNION_H
#define SHAPEREALITY_QUATERNION_H

#include "vector.h"

namespace math
{
    // can be used to represent rotations
    template<typename Type>
    struct Quaternion final
    {
        constexpr explicit Quaternion() : x(0), y(0), z(0), w(1) {}

        constexpr explicit Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        float x;
        float y;
        float z;
        float w;

        constexpr ~Quaternion() = default;

        [[nodiscard]] constexpr Quaternion operator*(Quaternion const& other) const;

        const static Quaternion identity;

        [[nodiscard]] constexpr Vector<3, Type> toEulerInRadians() const;

        [[nodiscard]] constexpr Vector<3, Type> toEulerInDegrees() const;

        [[nodiscard]] constexpr static Quaternion createFromEulerInRadians(Vector3 eulerAngles);

        [[nodiscard]] constexpr static Quaternion angleAxis(Type angle, Vector3 const& axis);

        [[nodiscard]] constexpr static Quaternion createFromEulerInDegrees(Vector3 eulerAngles);

        [[nodiscard]] constexpr bool operator==(Quaternion const& other) const;

        [[nodiscard]] constexpr bool operator!=(Quaternion const& other) const;
    };

    template<typename Type>
    [[nodiscard]] constexpr Vector<3, Type> operator*(Quaternion<Type> const& lhs, Vector<3, Type> const& rhs);
}

#endif //SHAPEREALITY_QUATERNION_H
