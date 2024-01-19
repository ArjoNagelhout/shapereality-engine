//
// Created by Arjo Nagelhout on 15/01/2024.
//

#include "transform.h"

#include "math/matrix.inl"

namespace renderer
{
    void sortTransformHierarchy(entity::Registry& r)
    {
//        r.sort<entity::HierarchyComponent>(
//            [](entity::HierarchyComponent const& lhs, entity::HierarchyComponent const& rhs) {
//                // we want to compare the parent index with the entity index
//                // i.e.: parent index of lhs should always be smaller than entity index of rhs
//            });
    }

    // how do we set whether an item is dirty or not? we simply create a dirty component
    // as a result, when creating a view, we only iterate over a few dirty transforms

    void computeLocalToWorldMatrices(entity::Registry& r)
    {
        // first we emplace the dirty components for all children using depth first search


        // we want to use the order of the HierarchyComponent, as
        // we assume that it is sorted with the parents being at the start of the iteration, i.e.
        // children will always come after their parent in iteration.
        for (auto [entityId, dirty, transform, hierarchy]: r.view<TransformDirtyComponent, TransformComponent, entity::HierarchyComponent>(
            entity::IterationPolicy::UseFirstComponent))
        {
            // update the localToParentTransform
            transform.localToParentTransform = math::createTranslationRotationScaleMatrix(transform.localPosition,
                                                                                          transform.localRotation,
                                                                                          transform.localScale);

            // calculate the localToWorldTransform
            if (hierarchy.parent != entity::TOMBSTONE)
            {
                TransformComponent const& parent = r.getComponent<TransformComponent>(hierarchy.parent);
                transform.localToWorldTransform = parent.localToWorldTransform * transform.localToParentTransform;
            }
            else
            {
                transform.localToWorldTransform = transform.localToParentTransform;
            }
        }

        // remove all dirty components
        r.removeComponentType<TransformDirtyComponent>();
    }
}