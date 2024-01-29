#include "quaternion.h"

#include "utils.h"
#include "vector.inl"

#ifndef SHAPEREALITY_QUATERNION_INL
#define SHAPEREALITY_QUATERNION_INL

namespace math
{


    constexpr Quaternion Quaternion::identity = Quaternion{0, 0, 0, 1};

    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
    constexpr Vector3 Quaternion::toEulerInRadians() const
    {
        constexpr float delta = 0.499f;

        float const test = x * y + z * w;

        // singularity at north pole
        if (test > delta)
        {
            float const heading = 2.f * atan2(x, w);
            float const attitude = PI / 2.f;
            float const bank = 0.f;
            return Vector3{{heading, attitude, bank}};
        }

        // singularity at south pole
        if (test < -delta)
        {
            float const heading = -2.f * atan2(x, w);
            float const attitude = -PI / 2.f;
            float const bank = 0;
            return Vector3{{heading, attitude, bank}};
        }
        float const sqx = x * x;
        float const sqy = y * y;
        float const sqz = z * z;

        float const heading = atan2(2.f * y * w - 2.f * x * z, 1.f - 2.f * sqy - 2.f * sqz);
        float const attitude = asin(2.f * test);
        float const bank = atan2(2 * x * w - 2 * y * z, 1.f - 2.f * sqx - 2.f * sqz);

        return Vector3{{heading, attitude, bank}};
    }

    constexpr Vector3 Quaternion::toEulerInDegrees() const
    {
        Vector3 eulerInRadians = toEulerInRadians();
        return eulerInRadians * 180.0f / PI;
    }

    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
    constexpr Quaternion Quaternion::createFromEulerInRadians(Vector3 eulerAngles)
    {
        float const heading = eulerAngles[0];
        float const attitude = eulerAngles[1];
        float const bank = eulerAngles[2];

        float const c1 = cos(heading / 2.f);
        float const s1 = sin(heading / 2.f);
        float const c2 = cos(attitude / 2.f);
        float const s2 = sin(attitude / 2.f);
        float const c3 = cos(bank / 2.f);
        float const s3 = sin(bank / 2.f);
        float const c1c2 = c1 * c2;
        float const s1s2 = s1 * s2;
        float const w = c1c2 * c3 - s1s2 * s3;
        float const x = c1c2 * s3 + s1s2 * c3;
        float const y = s1 * c2 * c3 + c1 * s2 * s3;
        float const z = c1 * s2 * c3 - s1 * c2 * s3;
        return Quaternion(x, y, z, w);
    }

    constexpr Quaternion Quaternion::createFromEulerInDegrees(Vector3 eulerAngles)
    {
        Vector3 eulerAnglesInRadians = eulerAngles * (PI / 180.0f);
        return createFromEulerInRadians(eulerAnglesInRadians);
    }
}

#endif //SHAPEREALITY_QUATERNION_INL