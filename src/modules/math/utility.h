//
// Created by Arjo Nagelhout on 17/12/2023.
//

#ifndef SHAPEREALITY_UTILITY_H
#define SHAPEREALITY_UTILITY_H

namespace math
{
    constexpr float PI = 3.14159265359f;

    constexpr float degreesToRadians(float degrees)
    {
        return degrees * (PI / 180.0f);
    }

    constexpr float radiansToDegrees(float radians)
    {
        return radians * 180.0f / PI;
    }

    template<typename Type>
    constexpr Type lerpUnclamped(Type a, Type b, Type t)
    {
        return (a * (1.f - t)) + (b * t);
    }

    template<typename Type>
    constexpr Type lerp(Type a, Type b, Type t)
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
}

#endif //SHAPEREALITY_UTILITY_H
