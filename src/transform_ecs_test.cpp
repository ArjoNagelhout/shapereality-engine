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
        mat4::identity
    }; // from local space to world space (with parent's transformations applied)

    // hierarchy
    size_type childCount{0};
    size_type parent{TOMBSTONE};
    size_type firstChild{TOMBSTONE};

    // doubly-linked list
    size_type previousChild{TOMBSTONE};
    size_type nextChild{TOMBSTONE};
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

    // objects to move includes the object itself
    size_type objectsToMoveCount = e.childCount + 1;

    // determine position (go through all children)
    if (childIndex > 0)
    {
        size_type targetChildIndex = std::clamp(childIndex, size_type(0), p.childCount);
    }
    //size_type parent
    //+p.childCount

    // go through all children
//    for (size_type i = 0; i < childCount + 1; i++)
//    {
//
//    }

    // on insertion of a new latter indices with one

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
        if (t.parent > 0)
        {
            TransformComponent const& parent = registry.getComponent<TransformComponent>(t.parent);
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
    //@formatter:off
    registry.addComponent<TransformComponent>(0, TransformComponent{.name = 'A', .parent = 0});
    registry.addComponent<TransformComponent>(1, TransformComponent{.name = 'B', .parent = 1});
    registry.addComponent<TransformComponent>(2, TransformComponent{.name = 'C', .parent = 1});
    registry.addComponent<TransformComponent>(3, TransformComponent{.name = 'D', .parent = 1});
    registry.addComponent<TransformComponent>(4, TransformComponent{.name = 'E', .parent = 3});
    registry.addComponent<TransformComponent>(5, TransformComponent{.name = 'F', .parent = 3});
    registry.addComponent<TransformComponent>(6, TransformComponent{.name = 'G', .parent = 5});
    registry.addComponent<TransformComponent>(7, TransformComponent{.name = 'H', .parent = 5});
    registry.addComponent<TransformComponent>(8, TransformComponent{.name = 'I', .parent = 1});
    //@formatter:on

    sortTransformHierarchy(registry);

    // set entity 0's parent to entity 2, at index 0
    setParent(registry, 3, 1, 0);

    computeLocalToWorldMatrices(registry);

    printTransformHierarchy(registry);
}
