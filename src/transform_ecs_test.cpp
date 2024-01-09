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
    size_type hierarchyCount{0}; // amount of children, computed recursively, of this entity
    size_type childCount{0}; // amount of direct children of this entity
    size_type parent{TOMBSTONE};
    size_type firstChild{TOMBSTONE};

    // doubly-linked list
    size_type previousSibling{TOMBSTONE};
    size_type nextSibling{TOMBSTONE};
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
bool isChildOf(Registry& registry, entity_type entity, entity_type potentialParent)
{
    assert(potentialParent != TOMBSTONE && "provided parent is TOMBSTONE");

    // recurse up from entity to see if it has provided parent as its parent
    // this is quicker than iterating over all children

    entity_type current = entity;
    while (current != TOMBSTONE)
    {
        auto& e = registry.getComponent<TransformComponent>(current);
        if (potentialParent == e.parent)
        {
            return true;
        }
        current = e.parent;
    }

    return false;
}

bool isParentOf(Registry& registry, entity_type entity, entity_type potentialChild)
{
    return isChildOf(registry, potentialChild, entity);
}

void computeHierarchyCountRecurseUp(Registry& registry, entity_type entity)
{
    entity_type current = entity;

    // recurse up from entity to its uppermost parent
    while (current != TOMBSTONE)
    {
        auto& transform = registry.getComponent<TransformComponent>(current);
        int sum = 1; // hierarchyCount includes the entity itself

        if (transform.firstChild != TOMBSTONE)
        {
            // iterate over children
            entity_type currentChild = transform.firstChild;
            while (currentChild != TOMBSTONE)
            {
                // get child
                auto& childTransform = registry.getComponent<TransformComponent>(currentChild);
                sum += static_cast<int>(childTransform.hierarchyCount);
                currentChild = childTransform.nextSibling;
            }
        }

        transform.hierarchyCount = sum;

        // recurse up
        current = transform.parent;
    }
}

// returns TOMBSTONE if no children, or index outside of range of children
entity_type getChild(Registry& registry, entity_type entity, size_type atIndex)
{
    entity_type current = entity;
    size_type i = 0;
    while (i != atIndex)// || current != TOMBSTONE)
    {
        auto& transform = registry.getComponent<TransformComponent>(entity);
        current = transform.nextSibling;
        i++;
    }

    return current;
}

/**
 * if childIndex is greater than the amount of
 *
 * @param entity entity to change the parent of
 * @param parent the entity will be added as a child to the parent
 * @param childIndex the child index to insert the entity into
 */
bool setParent(Registry& registry, entity_type entity, entity_type parent, size_type childIndex)
{
    assert(parent != TOMBSTONE);

    auto& transform = registry.getComponent<TransformComponent>(entity);

    // if `parent` is a child of `entity`,
    // this would result in a cyclical dependency
    if (isChildOf(registry, parent, entity))
    {
        return false;
    }

    // reconnect siblings to each other where entity gets moved out from
    if (transform.previousSibling != TOMBSTONE)
    {
        auto& prev = registry.getComponent<TransformComponent>(transform.previousSibling);
        prev.nextSibling = transform.nextSibling;
    }

    if (transform.nextSibling != TOMBSTONE)
    {
        auto& nextTransform = registry.getComponent<TransformComponent>(transform.nextSibling);
        nextTransform.previousSibling = transform.previousSibling;
    }

    // get child we want to insert the entity to the left of
    entity_type childAtIndex = getChild(registry, parent, childIndex);

    if (childAtIndex == TOMBSTONE)
    {
        // there is no child, so we only have to clear the current siblings of the entity
        transform.previousSibling = TOMBSTONE;
        transform.nextSibling = TOMBSTONE;
    }
    else
    {
        // otherwise, we'll have to set the siblings
        auto& childAtIndexTransform = registry.getComponent<TransformComponent>(childAtIndex);

        // set the next sibling of the previous sibling to this entity, if it exists
        if (childAtIndexTransform.previousSibling != TOMBSTONE)
        {
            auto& prev = registry.getComponent<TransformComponent>(childAtIndexTransform.previousSibling);
            prev.nextSibling = entity;
        }

        // set previous and next siblings
        transform.previousSibling = childAtIndexTransform.previousSibling;
        transform.nextSibling = childAtIndex;

        // set previous sibling of the child we want to insert to the left of to this entity
        childAtIndexTransform.previousSibling = entity;
    }

    // also update target parent transform's firstChild if needed
    // that is, if it has no children, or the entity has been inserted at index 0
    if (childAtIndex == TOMBSTONE || childIndex == 0)
    {
        auto& parentTransform = registry.getComponent<TransformComponent>(parent);
        parentTransform.firstChild = entity;
    }

    // set entity's parent to target parent
    transform.parent = parent;

    // recursively walk up to all parents to set the hierarchyCount
    computeHierarchyCountRecurseUp(registry, entity);

    return true;
}

void computeChildCount(Registry& registry, entity_type entity)
{
    auto& transform = registry.getComponent<TransformComponent>(entity);

    int sum = 0;

    entity_type current = transform.firstChild;
    while (current != TOMBSTONE)
    {
        auto& childTransform = registry.getComponent<TransformComponent>(current);
        current = childTransform.nextSibling;
        sum++;
    }

    transform.childCount = sum;
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
