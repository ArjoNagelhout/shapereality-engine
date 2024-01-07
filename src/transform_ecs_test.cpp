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

    vec3 localPosition{vec3::zero};
    Quaternion localRotation{Quaternion::identity};
    vec3 localScale{vec3::one};

    bool dirty{true};

    mat4 localToParentTransform{mat4::identity}; // from local space to parent space
    mat4 localToWorldTransform{
        mat4::identity}; // from local space to world space (with parent's transformations applied)

    size_type parentIndex{0};
    size_type hierarchyCount{0};
    size_type childCount{0};
};

/**
 * will automatically add it to the end of the parent's children
 *
 * @param entity
 * @param parent
 */
void setParent(Registry& registry, entity_type entity, entity_type parent)
{

}

/**
 * whether `entity` is a child of `parent`
 */
bool isChildOf(Registry& registry, entity_type entity, entity_type parent)
{
    return false;
}

/**
 *
 * @param entity entity to change the parent of
 * @param parent the entity will be added as a child to the parent
 * @param index the child index to insert the entity into
 */
bool setParent(Registry& registry, entity_type entity, entity_type parent, size_type childIndex)
{
    auto& e = registry.getComponent<TransformComponent>(entity);
    auto& p = registry.getComponent<TransformComponent>(parent);

    // if `parent` is a child of `entity`,
    // this would result in a cyclical dependency
    if (isChildOf(registry, parent, entity))
    {
        return false;
    }

    // get amount of objects in entity's hierarchy
    size_type childCount = e.childCount;

    // get index of parent

    return true;
}

void computeLocalToWorldMatrices(Registry& registry)
{
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
    for (auto& t: registry.getComponentType<TransformComponent>())
    {
        std::cout << "transform: " << t.name << std::endl;
    }
}

int main(int argc, char* argv[])
{
    Registry registry;
    registry.createEntity(0);
    registry.createEntity(1);
    registry.createEntity(2);
    registry.createEntity(3);
    registry.createEntity(4);
    registry.createEntity(5);
    registry.createEntity(6);
    registry.createEntity(7);
    registry.createEntity(8);

    // should be in reverse order (sorting will be done next)
    registry.addComponent<TransformComponent>(8, TransformComponent{.name = 'A',});
    registry.addComponent<TransformComponent>(7, TransformComponent{.name = 'B',});
    registry.addComponent<TransformComponent>(6, TransformComponent{.name = 'C',});
    registry.addComponent<TransformComponent>(5, TransformComponent{.name = 'D',});
    registry.addComponent<TransformComponent>(4, TransformComponent{.name = 'E',});
    registry.addComponent<TransformComponent>(3, TransformComponent{.name = 'F',});
    registry.addComponent<TransformComponent>(2, TransformComponent{.name = 'G', .localPosition = vec3{{3, 0, 1}}});
    registry.addComponent<TransformComponent>(1, TransformComponent{.name = 'H',});
    registry.addComponent<TransformComponent>(0, TransformComponent{.name = 'I', .localPosition = vec3{
        {0, 5, 0}}, .parentIndex = 2});

    sortTransformHierarchy(registry);

    computeLocalToWorldMatrices(registry);

    printTransformHierarchy(registry);
}
