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
    // transform should wrap certain functionality from hierarchy and add the transform dirty component
    // otherwise, we would require event based programming, which becomes messy quickly in combination with an ECS.
    // so we simply add a flag.
    // these flags could potentially be standardized so that hierarchy always "emits" a HierarchyChangedEvent component,
    // which could simply be removed as a final pass in the main loop.

    struct TransformDirtyComponent final
    {
    };

    struct TransformComponent final
    {
        math::Vector3 localPosition{math::Vector3::zero};
        math::Quaternion localRotation{math::Quaternion::identity};
        math::Vector3 localScale{math::Vector3::one};

        math::Matrix4 localToParentTransform{math::Matrix4::identity}; // from local space to parent space
        math::Matrix4 localToWorldTransform{
            math::Matrix4::identity
        }; // from local space to world space (with parent's transformations applied)
    };

    // sorts the hierarchy, with parents located at the end of the dense array
    // to ensure right execution order.
    //
    // this could be moved to Hierarchy, but right now hierarchy doesn't make any assumptions to how the hierarchy is
    // used
    void sortTransformHierarchy(entity::Registry& r);

    void computeLocalToWorldMatrices(entity::Registry& r);
}

#endif //SHAPEREALITY_TRANSFORM_H
