#include "quaternion.h"

#include "utility.h"
#include "vector.inl"

#ifndef SHAPEREALITY_QUATERNION_INL
#define SHAPEREALITY_QUATERNION_INL

namespace math
{
    template<typename Type>
    constexpr Quaternion<Type> Quaternion<Type>::operator*(Quaternion const& other) const
    {
        Quaternion<Type> const p{*this};
        Quaternion<Type> const q{other};

        return Quaternion<Type>{
            p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
            p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z,
            p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x,
            p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z,
        };
    }

    template<typename Type>
    constexpr Quaternion<Type> Quaternion<Type>::identity = Quaternion{0, 0, 0, 1};

    template<typename Type>
    constexpr Vector<3, Type> Quaternion<Type>::toEulerInRadians() const
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
        constexpr float delta = 0.499f;

        float const test = x * y + z * w;

        // singularity at North Pole
        if (test > delta)
        {
            float const heading = 2.f * atan2(x, w);
            float const attitude = PI / 2.f;
            float const bank = 0.f;
            return Vector<3, Type>{heading, attitude, bank};
        }

        // singularity at South Pole
        if (test < -delta)
        {
            float const heading = -2.f * atan2(x, w);
            float const attitude = -PI / 2.f;
            float const bank = 0;
            return Vector<3, Type>{heading, attitude, bank};
        }
        float const sqx = x * x;
        float const sqy = y * y;
        float const sqz = z * z;

        float const heading = atan2(2.f * y * w - 2.f * x * z, 1.f - 2.f * sqy - 2.f * sqz);
        float const attitude = asin(2.f * test);
        float const bank = atan2(2 * x * w - 2 * y * z, 1.f - 2.f * sqx - 2.f * sqz);

        return Vector<3, Type>{heading, attitude, bank};
    }

    template<typename Type>
    constexpr Vector<3, Type> Quaternion<Type>::toEulerInDegrees() const
    {
        Vector3 eulerInRadians = toEulerInRadians();
        return eulerInRadians * 180.0f / PI;
    }

    template<typename Type>
    constexpr Quaternion<Type> Quaternion<Type>::angleAxis(Type angle, Vector3 const& axis)
    {
        // from glm
        // negates x and z to be consistent with NASA aircraft rotations

        Type const a{angle};
        Type const s = sin(a * static_cast<Type>(0.5));

        return Quaternion(-axis.x() * s, axis.y() * s, -axis.z() * s, cos(a * static_cast<Type>(0.5)));
    }

    template<typename Type>
    constexpr Quaternion<Type> Quaternion<Type>::createFromEulerInRadians(Vector3 eulerAngles)
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
        // https://gamedev.stackexchange.com/questions/143212/set-rotation-on-single-axis-with-provided-angle-using-quaternion

        // y should be heading
        // x should be attitude
        // z should be bank

        float const heading = eulerAngles[1]; // y
        float const attitude = eulerAngles[0]; // x
        float const bank = eulerAngles[2]; // z

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

    template<typename Type>
    constexpr Quaternion<Type> Quaternion<Type>::createFromEulerInDegrees(Vector3 eulerAngles)
    {
        Vector3 eulerAnglesInRadians = eulerAngles * (PI / 180.0f);
        return createFromEulerInRadians(eulerAnglesInRadians);
    }

    template<typename Type>
    [[nodiscard]] constexpr bool Quaternion<Type>::operator==(Quaternion const& other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    template<typename Type>
    [[nodiscard]] constexpr bool Quaternion<Type>::operator!=(Quaternion const& other) const
    {
        return (!this->operator==(other));
    }

    template<typename Type>
    constexpr Vector<3, Type> operator*(Quaternion<Type> const& lhs, Vector<3, Type> const& rhs)
    {
        // implementation taken from glm, https://github.com/g-truc/glm/blob/master/glm/detail/type_quat.inl

        Vector<3, Type> const q{lhs.x, lhs.y, lhs.z};
        Vector<3, Type> const uv{q.cross(rhs)};
        Vector<3, Type> const uuv{q.cross(uv)};
        return rhs + ((uv * lhs.w) + uuv) * static_cast<Type>(2);
    }
}

#endif //SHAPEREALITY_QUATERNION_INL