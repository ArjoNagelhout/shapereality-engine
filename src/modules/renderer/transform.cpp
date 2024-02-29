//
// Created by Arjo Nagelhout on 15/01/2024.
//

#include "transform.h"

#include "math/matrix.inl"

namespace renderer
{
    void setDirty(entity::Registry& r, entity::Entity entityId)
    {
        entity::depthFirstSearch(r, entityId, [&r](auto childId) {
            if (r.entityContainsComponent<TransformDirtyComponent>(childId))
            {
                // if already dirty, we don't have to continue traversal
                return false;
            }

            // add dirty component
            r.addComponent<TransformDirtyComponent>(childId);

            return true;
        });
    }

    void computeLocalToParentTransform(TransformComponent& transform)
    {
        transform.localToParentTransform = math::createTranslationRotationScaleMatrix(transform.localPosition,
                                                                                      transform.localRotation,
                                                                                      transform.localScale);
    }

    void setLocalPosition(entity::Registry& r, entity::Entity entityId, math::Vector3 localPosition)
    {
        auto& entity = r.getComponent<TransformComponent>(entityId);
        entity.localPosition = localPosition;
        computeLocalToParentTransform(entity);
        setDirty(r, entityId);
    }

    void setLocalRotation(entity::Registry& r, entity::Entity entityId, math::Quaternion localRotation)
    {
        auto& entity = r.getComponent<TransformComponent>(entityId);
        entity.localRotation = localRotation;
        computeLocalToParentTransform(entity);
        setDirty(r, entityId);
    }

    void setLocalScale(entity::Registry& r, entity::Entity entityId, math::Vector3 localScale)
    {
        auto& entity = r.getComponent<TransformComponent>(entityId);
        entity.localScale = localScale;
        computeLocalToParentTransform(entity);
        setDirty(r, entityId);
    }

    void computeLocalToWorldMatrices(entity::Registry& r)
    {
        // we want to use the order of the HierarchyComponent, as
        // we assume that it is sorted with the parents being at the start of the iteration, i.e.
        // children will always come after their parent in iteration.
        for (auto [entityId, hierarchy, dirty, transform]: r.view<entity::HierarchyComponent, TransformDirtyComponent, TransformComponent>(
            entity::IterationPolicy::UseFirstComponent))
        {
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