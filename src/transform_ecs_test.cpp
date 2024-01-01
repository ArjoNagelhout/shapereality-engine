//
// Created by Arjo Nagelhout on 28/12/2023.
//

#include <iostream>
#include <thread>
#include <array>
#include <string>
#include <limits>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/quaternion.h"
#include "math/quaternion.inl"

#include "entity/sparse_set.h"
#include "entity/registry.h"

using namespace math;
using namespace entity;

// https://github.com/skypjack/entt/commits/master/?after=f2c417435c021b6bd2e426ffd9d1fc116b804146+5000
// https://github.com/skypjack/entt/blob/ccf8f712ccbb32b2ac19e1b0f75ac0630db71caf/src/registry.hpp

// only contains data, no logic
struct TransformComponent
{
    vec3 localPosition{vec3::zero};
    Quaternion localRotation{Quaternion::identity};
    vec3 localScale{vec3::one};

    bool dirty{true};

    mat4 localToParentTransform{mat4::identity}; // from local space to parent space
    mat4 localToWorldTransform{
        mat4::identity}; // from local space to world space (with parent's transformations applied)

    size_t parentIndex{0};
};

// entt uses paging because this makes allocating more efficient.

void update(Registry& registry)
{
//    for (auto& t: registry.transformComponents)
//    {
//        if (t.dirty)
//        {
//            // update the localToParentTransform
//            t.localToParentTransform = math::createTranslationRotationScaleMatrix(t.localPosition, t.localRotation,
//                                                                                  t.localScale);
//            t.dirty = false;
//        }
//
//        // calculate the localToWorldTransform
//        if (t.parentIndex > 0)
//        {
//            TransformComponent const& parent = registry.transformComponents[t.parentIndex];
//            t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
//        }
//    }

    std::cout << "updated" << std::endl;
}

struct Beast
{
    int five = 5;
    bool yes = true;
    float wee = 0.3235f;
    float waa = 1234.425f;
};

int main(int argc, char* argv[])
{
    Registry registry;
    registry.createEntity(0);
    registry.addComponent<Beast>(0, Beast{.five = 6, .yes = false, .wee = 0.1f, .waa = 10.0f});
    registry.removeComponent<Beast>(0);

    bool hasBeast = registry.hasComponent<Beast>(0);
    std::cout << "hasBeast: " << hasBeast << std::endl;
}
