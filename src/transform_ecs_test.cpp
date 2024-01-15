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
#include "entity/view.h"

using namespace math;
using namespace entity;

// only contains data, no logic
struct TransformComponent
{
    char name{'_'};

//    vec3 localPosition{vec3::zero};
//    Quaternion localRotation{Quaternion::identity};
//    vec3 localScale{vec3::one};
//
//    bool dirty{true};
//
//    mat4 localToParentTransform{mat4::identity}; // from local space to parent space
//    mat4 localToWorldTransform{
//        mat4::identity
//    }; // from local space to world space (with parent's transformations applied)
};

void printTransformHierarchy(Registry& registry)
{
    for (auto& t: registry.getComponentStorage<TransformComponent>())
    {
        std::cout << "transform: " << t.name << std::endl;
    }
}
