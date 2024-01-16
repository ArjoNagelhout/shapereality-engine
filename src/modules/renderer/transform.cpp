//
// Created by Arjo Nagelhout on 15/01/2024.
//

#include "transform.h"

#include "math/matrix.inl"

namespace renderer
{
    void computeLocalToWorldMatrices(entity::Registry& r)
    {
        for (auto [hierarchy, t]: r.view<entity::HierarchyComponent, TransformComponent>())
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
            if (hierarchy.parent != entity::TOMBSTONE)
            {
                TransformComponent const& parent = r.getComponent<TransformComponent>(hierarchy.parent);
                t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
            }
            else
            {
                t.localToWorldTransform = t.localToParentTransform;
            }

            std::cout << t.localToWorldTransform << std::endl;

            math::Vector3 translation = getMatrixTranslation(t.localToWorldTransform);
            std::cout << "localPosition: " << t.localPosition << ", worldPosition: " << translation << std::endl;
        }
    }
}