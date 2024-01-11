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

//    vec3 localPosition{vec3::zero};
//    Quaternion localRotation{Quaternion::identity};
//    vec3 localScale{vec3::one};
//
//    bool dirty{true};
//
//    mat4 localToParentTransform{mat4::identity}; // from local space to parent space
//    mat4 localToWorldTransform{
//        mat4::identity
//    }; // from local space to world space (with parent's transformations applied)

    // hierarchy
    size_type hierarchyCount{0}; // amount of children, computed recursively, of this entity
    size_type childCount{0}; // amount of direct children of this entity
    size_type parent{TOMBSTONE};
    size_type firstChild{TOMBSTONE};

    // doubly-linked list
    size_type previous{TOMBSTONE}; // previous sibling
    size_type next{TOMBSTONE}; // next sibling
};

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
                currentChild = childTransform.next;
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
        current = transform.next;
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
[[nodiscard]] bool setParent(Registry& registry, entity_type entity, entity_type parent, size_type childIndex)
{
    assert(parent != TOMBSTONE);

    auto& transform = registry.getComponent<TransformComponent>(entity);

    // if `parent` is a child of `entity`,
    // this would result in a cyclical dependency
    if (isChildOf(registry, parent, entity))
    {
        return false;
    }

    // ensure index not out of range
    auto& parentTransform = registry.getComponent<TransformComponent>(parent);
    if (childIndex > parentTransform.childCount)
    {
        return false; // error, index out of range
    }

    // reconnect siblings to each other where entity gets moved out from
    if (transform.previous != TOMBSTONE)
    {
        auto& previous = registry.getComponent<TransformComponent>(transform.previous);
        previous.next = transform.next;
    }

    if (transform.next != TOMBSTONE)
    {
        auto& next = registry.getComponent<TransformComponent>(transform.next);
        next.previous = transform.previous;
    }

    entity_type previous;
    entity_type next;

    if (childIndex == 0)
    {
        // set previous and next
        previous = TOMBSTONE;
        next = parentTransform.firstChild;

        // set the parent's first to this child
        parentTransform.firstChild = entity;
    }
    else
    {
        // set previous and next
        // get child we want to insert the entity to the right of, guaranteed to exist
        previous = getChild(registry, parent, childIndex-1);
        auto& previousTransform = registry.getComponent<TransformComponent>(previous);
        next = previousTransform.next;

        // set next of previous to this entity
        previousTransform.next = entity;
    }

    // link next to entity
    if (next != TOMBSTONE)
    {
        auto& nextTransform = registry.getComponent<TransformComponent>(next);
        nextTransform.previous = entity;
    }

    // update entity's previous and next
    transform.previous = previous;
    transform.next = next;

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
        current = childTransform.next;
        sum++;
    }

    transform.childCount = sum;
}

//void computeLocalToWorldMatrices(Registry& registry)
//{
//    for (auto& t: registry.getComponentStorage<TransformComponent>())
//    {
//        if (t.dirty)
//        {
//            // update the localToParentTransform
//            t.localToParentTransform = math::createTranslationRotationScaleMatrix(t.localPosition,
//                                                                                  t.localRotation,
//                                                                                  t.localScale);
//            t.dirty = false;
//        }
//
//        // calculate the localToWorldTransform
//        if (t.parent > 0)
//        {
//            TransformComponent const& parent = registry.getComponent<TransformComponent>(t.parent);
//            t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
//        }
//        else
//        {
//            t.localToWorldTransform = t.localToParentTransform;
//        }
//
//        std::cout << t.localToWorldTransform << std::endl;
//
//        vec3 translation = getMatrixTranslation(t.localToWorldTransform);
//        std::cout << "localPosition: " << t.localPosition << ", worldPosition: " << translation << std::endl;
//    }
//}

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
    for (auto& t: registry.getComponentStorage<TransformComponent>())
    {
        std::cout << "transform: " << t.name << std::endl;
    }
}

int main(int argc, char* argv[])
{
    Registry lalala;
    entity_type wee = lalala.createEntity(0);

    Registry registry;
    entity_type e1 = registry.createEntity(0);
    entity_type e2 = registry.createEntity(1);

    assert(wee == e1);

    registry.addComponent<TransformComponent>(e1, TransformComponent{.name = 'A'});
    registry.addComponent<TransformComponent>(e2, TransformComponent{.name = 'B'});

//    auto& t = registry.getComponentStorage<TransformComponent>();

    bool success;

    success = setParent(registry, e1, e2, 0);

    std::cout << success << std::endl;

    success = setParent(registry, e2, e1, 0);

    std::cout << success << std::endl;
//    computeLocalToWorldMatrices(registry);

//    printTransformHierarchy(registry);
}
