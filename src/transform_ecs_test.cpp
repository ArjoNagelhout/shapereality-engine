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

struct MyBeautifulComponent
{
    int five = 5;
    bool yes = true;
    float wee = 0.3235f;
    float waa = 1234.425f;
};

struct AnotherComponent
{
    bool woez = false;
    bool waz = true;
    bool woe = false;
    bool waa = true;
};

int main(int argc, char* argv[])
{
    Registry registry;
    registry.createEntity(0);
    registry.createEntity(1);
    registry.createEntity(2);
    registry.createEntity(3);
    registry.createEntity(4);
    registry.createEntity(5);
    registry.addComponent<MyBeautifulComponent>(0, MyBeautifulComponent{.five = 6, .yes = false, .wee = 0.1f, .waa = 10.0f});
    registry.addComponent<AnotherComponent>(1);
    registry.addComponent<AnotherComponent>(2);
    registry.addComponent<AnotherComponent>(3);

    bool hasComponent = registry.hasComponent<MyBeautifulComponent>(0);
    std::cout << "hasComponent: " << ((hasComponent == 1) ? "yes" : "no") << std::endl;

    registry.removeComponent<MyBeautifulComponent>(0);

    std::cout << "entityExists: " << ((registry.entityExists(0) == 1) ? "yes" : "no") << std::endl;

    registry.destroyEntity(0);

    std::cout << "entityExists: " << ((registry.entityExists(0) == 1) ? "yes" : "no") << std::endl;

}
