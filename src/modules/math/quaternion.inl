#include "quaternion.h"

#include "utils.h"

#ifndef SHAPEREALITY_QUATERNION_INL
#define SHAPEREALITY_QUATERNION_INL

namespace math
{
    constexpr Quaternion Quaternion::identity = Quaternion{0, 0, 0, 1};

    // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    constexpr Vector3 Quaternion::toEulerInRadians() const
    {
        Vector3 result{};
        // roll (x-axis rotation)
        float sinr_cosp = 2.f * (w * x + y * z);
        float cosr_cosp = 1.f - 2.f * (x * x + y * y);
        result[0] = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        float sinp = std::sqrt(1.f + 2.f * (w * y - x * z));
        float cosp = std::sqrt(1.f - 2.f * (w * y - x * z));
        result[1] = 2.f * std::atan2(sinp, cosp) - PI / 2.f;

        // yaw (z-axis rotation)
        float siny_cosp = 2.f * (w * z + x * y);
        float cosy_cosp = 1.f - 2.f * (y * y + z * z);
        result[2] = std::atan2(siny_cosp, cosy_cosp);

        return result;
    }

    constexpr Vector3 Quaternion::toEulerInDegrees() const
    {
        Vector3 eulerInRadians = toEulerInRadians();
        return eulerInRadians * 180.0f / PI;
    }

    constexpr Quaternion Quaternion::createFromEulerInRadians(Vector3 eulerAngles)
    {
        Vector3 c = Vector3{{cos(eulerAngles.x() * 0.5f), cos(eulerAngles.y() * 0.5f), cos(eulerAngles.z() * 0.5f)}};
        Vector3 s = Vector3{{sin(eulerAngles.x() * 0.5f), sin(eulerAngles.y() * 0.5f), sin(eulerAngles.z() * 0.5f)}};

        Quaternion result{0.0f, 0.0f, 0.0f, 1.0f};
        result.w = c.x() * c.y() * c.z() + s.x() * s.y() * s.z();
        result.x = s.x() * c.y() * c.z() - c.x() * s.y() * s.z();
        result.y = c.x() * s.y() * c.z() + s.x() * c.y() * s.z();
        result.z = c.x() * c.y() * s.z() - s.x() * s.y() * c.z();
        return result;
    }

    constexpr Quaternion Quaternion::createFromEulerInDegrees(Vector3 eulerAngles)
    {
        Vector3 eulerAnglesInRadians = eulerAngles * (PI / 180.0f);
        return createFromEulerInRadians(eulerAnglesInRadians);
    }
}

#endif //SHAPEREALITY_QUATERNION_INL