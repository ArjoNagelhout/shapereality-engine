//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef SHAPEREALITY_RAY_H
#define SHAPEREALITY_RAY_H

#include "vector.h"

#include <string>

namespace math
{
    struct Ray final
    {
        // construct a ray
        // direction is always normalized
        constexpr explicit Ray(Vector3 const& origin, Vector3 const& direction);

        constexpr ~Ray();

        //
        [[nodiscard]] std::string string() const;

        // get the point at the given distance from the origin of this ray along this ray
        [[nodiscard]] constexpr Vector3 getPoint(float distance) const;

        // get origin, non-mutable
        [[nodiscard]] constexpr Vector3 origin() const;

        // get direction, non-mutable
        [[nodiscard]] constexpr Vector3 direction() const;

        //
        constexpr void setOrigin(Vector3 const& origin);

        // note: normalizes the given direction
        constexpr void setDirection(Vector3 const& direction);

    private:
        Vector3 _origin;
        Vector3 _direction;
    };

    constexpr std::ostream& operator<<(std::ostream& ostream, Ray const& ray);
}

#endif //SHAPEREALITY_RAY_H
