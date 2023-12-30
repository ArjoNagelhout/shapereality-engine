//
// Created by Arjo Nagelhout on 16/11/2023.
//

#ifndef SHAPEREALITY_QUATERNION_H
#define SHAPEREALITY_QUATERNION_H

#include "vector.h"

namespace math
{
    // can be used to represent rotations
    struct Quaternion final
    {
        constexpr explicit Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
        {}

        float x;
        float y;
        float z;
        float w;

        constexpr ~Quaternion() = default;

        //
        const static Quaternion identity;

        //
        [[nodiscard]] constexpr static Vector<3> toEuler();

        //
        [[nodiscard]] constexpr static Quaternion fromEulerInRadians(float x, float y, float z);

    private:

    };
}

#endif //SHAPEREALITY_QUATERNION_H
