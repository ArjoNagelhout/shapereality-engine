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

//void computeLocalToWorldMatrices(Registry& registry)
//{
//    for (auto& t: registry.getComponentStorage<TransformComponent>())
//    {
//        if (t.dirty)
//        {
//            // update the localToParentTransform
//            t.localToParentTransform = math::createTranslationRotationScaleMatrix(t.localPosition,
//                                                                                  t.localRotation,
//                                                                                  t.localScale);
//            t.dirty = false;
//        }
//
//        // calculate the localToWorldTransform
//        if (t.parent > 0)
//        {
//            TransformComponent const& parent = registry.getComponent<TransformComponent>(t.parent);
//            t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
//        }
//        else
//        {
//            t.localToWorldTransform = t.localToParentTransform;
//        }
//
//        std::cout << t.localToWorldTransform << std::endl;
//
//        vec3 translation = getMatrixTranslation(t.localToWorldTransform);
//        std::cout << "localPosition: " << t.localPosition << ", worldPosition: " << translation << std::endl;
//    }
//}

void sortTransformHierarchy(Registry& registry)
{
    registry.sort<TransformComponent>([](TransformComponent const& lhs, TransformComponent const rhs) {
        // we want to compare the parent index with the entity index
        // i.e.: parent index of lhs should always be smaller than entity index of rhs

        // how to implement this?

        std::cout << lhs.name << std::endl;
        return true;
    });
}

void printTransformHierarchy(Registry& registry)
{
    for (auto& t: registry.getComponentStorage<TransformComponent>())
    {
        std::cout << "transform: " << t.name << std::endl;
    }
}

int main(int argc, char* argv[])
{
    Registry lalala;
    entity_type wee = lalala.createEntity(0);

    Registry registry;
    entity_type e1 = registry.createEntity(0);
    entity_type e2 = registry.createEntity(1);

    assert(wee == e1);

    registry.addComponent<TransformComponent>(e1, TransformComponent{.name = 'A'});
    registry.addComponent<TransformComponent>(e2, TransformComponent{.name = 'B'});

//    auto& t = registry.getComponentStorage<TransformComponent>();

    bool success;

//    success = setParent(registry, e1, e2, 0);

    std::cout << success << std::endl;

//    success = setParent(registry, e2, e1, 0);

    std::cout << success << std::endl;
//    computeLocalToWorldMatrices(registry);

//    printTransformHierarchy(registry);
}
