//
// Created by Arjo Nagelhout on 15/01/2024.
//

#ifndef SHAPEREALITY_TRANSFORM_H
#define SHAPEREALITY_TRANSFORM_H

#include "entity/registry.h"
#include "entity/components/hierarchy.h"

#include "math/vector.h"
#include "math/quaternion.h"
#include "math/matrix.h"

namespace renderer
{
    struct TransformComponent final
    {
        math::Vector3 localPosition{math::Vector3::zero};
        math::Quaternion localRotation{math::Quaternion::identity};
        math::Vector3 localScale{math::Vector3::one};

        bool dirty{true};

        math::Matrix4 localToParentTransform{math::Matrix4::identity}; // from local space to parent space
        math::Matrix4 localToWorldTransform{
            math::Matrix4::identity
        }; // from local space to world space (with parent's transformations applied)
    };

    void computeLocalToWorldMatrices(entity::Registry& r)
    {
//        for (auto [hierarchy, transform] : r.view<entity::IterationPolicy::UseSmallestComponent, entity::HierarchyComponent, TransformComponent>())
//        {
//
//        }
//        for (auto& t: registry.getComponentStorage<TransformComponent>())
//        {
//            if (t.dirty)
//            {
//                // update the localToParentTransform
//                t.localToParentTransform = math::createTranslationRotationScaleMatrix(t.localPosition,
//                                                                                      t.localRotation,
//                                                                                      t.localScale);
//                t.dirty = false;
//            }
//
//            // calculate the localToWorldTransform
//            if (t.parent > 0)
//            {
//                TransformComponent const& parent = registry.getComponent<TransformComponent>(t.parent);
//                t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
//            }
//            else
//            {
//                t.localToWorldTransform = t.localToParentTransform;
//            }
//
//            std::cout << t.localToWorldTransform << std::endl;
//
//            vec3 translation = getMatrixTranslation(t.localToWorldTransform);
//            std::cout << "localPosition: " << t.localPosition << ", worldPosition: " << translation << std::endl;
//        }
    }

//    void sortTransformHierarchy(entity::Registry& registry)
//    {
//        registry.sort<TransformComponent>([](TransformComponent const& lhs, TransformComponent const rhs) {
//            // we want to compare the parent index with the entity index
//            // i.e.: parent index of lhs should always be smaller than entity index of rhs
//
//            // how to implement this?
//
//            std::cout << lhs.name << std::endl;
//            return true;
//        });
//    }
}

#endif //SHAPEREALITY_TRANSFORM_H
