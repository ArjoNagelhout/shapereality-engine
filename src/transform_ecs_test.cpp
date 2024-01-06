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
    vec3 localPosition{vec3::zero};
    Quaternion localRotation{Quaternion::identity};
    vec3 localScale{vec3::one};

    bool dirty{true};

    mat4 localToParentTransform{mat4::identity}; // from local space to parent space
    mat4 localToWorldTransform{mat4::identity}; // from local space to world space (with parent's transformations applied)

    size_t parentIndex{0};
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
    registry.addComponent<TransformComponent>(0, TransformComponent{ .localPosition = vec3{{0, 5, 0}}, .parentIndex = 2});
    registry.addComponent<TransformComponent>(1, TransformComponent{ });
    registry.addComponent<TransformComponent>(2, TransformComponent{ .localPosition = vec3{{3, 0, 1}}});
    registry.addComponent<TransformComponent>(3, TransformComponent{ });
    registry.addComponent<TransformComponent>(4, TransformComponent{ });
    registry.addComponent<TransformComponent>(5, TransformComponent{ });

    for (auto& t: registry.getComponentType<TransformComponent>())
    {
        if (t.dirty)
        {
            // update the localToParentTransform
            t.localToParentTransform = math::createTranslationRotationScaleMatrix(t.localPosition,
                                                                                  t.localRotation,
                                                                                  t.localScale);
            t.dirty = false;
        }

        // calculate the localToWorldTransform
        if (t.parentIndex > 0)
        {
            TransformComponent const& parent = registry.getComponent<TransformComponent>(t.parentIndex);
            t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
        }
        else
        {
            t.localToWorldTransform = t.localToParentTransform;
        }

        std::cout << t.localToWorldTransform << std::endl;

        vec3 translation = getMatrixTranslation(t.localToWorldTransform);
        std::cout << "localPosition: " << t.localPosition << ", worldPosition: " << translation << std::endl;
    }
}
