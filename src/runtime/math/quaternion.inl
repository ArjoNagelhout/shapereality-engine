#include "quaternion.h"

#ifndef SHAPEREALITY_QUATERNION_INL
#define SHAPEREALITY_QUATERNION_INL

namespace math
{
    constexpr Quaternion Quaternion::identity = Quaternion{0, 0, 0, 1};

    constexpr Quaternion Quaternion::fromEulerInRadians(float x, float y, float z)
    {
        Vector3 c = Vector3{{cos(x * 0.5f), cos(y * 0.5f), cos(z * 0.5f)}};
        Vector3 s = Vector3{{sin(x * 0.5f), sin(y * 0.5f), sin(z * 0.5f)}};

        Quaternion result{0.0f, 0.0f, 0.0f, 1.0f};
        result.w = c.x() * c.y() * c.z() + s.x() * s.y() * s.z();
        result.x = s.x() * c.y() * c.z() - c.x() * s.y() * s.z();
        result.y = c.x() * s.y() * c.z() + s.x() * c.y() * s.z();
        result.z = c.x() * c.y() * s.z() - s.x() * s.y() * c.z();
        return result;
    }


}

#endif //SHAPEREALITY_QUATERNION_INL