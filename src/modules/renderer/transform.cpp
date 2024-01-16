//
// Created by Arjo Nagelhout on 15/01/2024.
//

#include "transform.h"

#include "math/matrix.inl"

namespace renderer
{
    void computeLocalToWorldMatrices(entity::Registry& r)
    {
        // we assume the HierarchyComponent storage is sorted
        // we want to use the order
        for (auto [hierarchy, transform]: r.view<entity::HierarchyComponent, TransformComponent>(entity::IterationPolicy::UseFirstComponent))
        {
            if (transform.dirty)
            {
                // update the localToParentTransform
                transform.localToParentTransform = math::createTranslationRotationScaleMatrix(transform.localPosition,
                                                                                              transform.localRotation,
                                                                                              transform.localScale);
                transform.dirty = false;
            }

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
    }
}