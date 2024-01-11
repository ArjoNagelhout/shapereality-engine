//
// Created by Arjo Nagelhout on 10/01/2024.
//

#ifndef SHAPEREALITY_HIERARCHY_H
#define SHAPEREALITY_HIERARCHY_H

#include "../registry.h"
#include <cassert>

namespace entity
{
    // reusable hierarchy component that can be used to create a node hierarchy or scene hierarchy for example

    // use composition over inheritance. In ECS, this can even mean adding two separate types:
    // add TransformComponent *and* HierarchyComponent
    struct HierarchyComponent final
    {
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
            auto& e = registry.getComponent<HierarchyComponent>(current);
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
            auto& transform = registry.getComponent<HierarchyComponent>(current);
            int sum = 1; // hierarchyCount includes the entity itself

            if (transform.firstChild != TOMBSTONE)
            {
                // iterate over children
                entity_type currentChild = transform.firstChild;
                while (currentChild != TOMBSTONE)
                {
                    // get child
                    auto& childTransform = registry.getComponent<HierarchyComponent>(currentChild);
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
            auto& transform = registry.getComponent<HierarchyComponent>(entity);
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

        auto& transform = registry.getComponent<HierarchyComponent>(entity);

        // if `parent` is a child of `entity`,
        // this would result in a cyclical dependency
        if (isChildOf(registry, parent, entity))
        {
            return false;
        }

        // ensure index not out of range
        auto& parentTransform = registry.getComponent<HierarchyComponent>(parent);
        if (childIndex > parentTransform.childCount)
        {
            return false; // error, index out of range
        }

        // reconnect siblings to each other where entity gets moved out from
        if (transform.previous != TOMBSTONE)
        {
            auto& previous = registry.getComponent<HierarchyComponent>(transform.previous);
            previous.next = transform.next;
        }

        if (transform.next != TOMBSTONE)
        {
            auto& next = registry.getComponent<HierarchyComponent>(transform.next);
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
            auto& previousTransform = registry.getComponent<HierarchyComponent>(previous);
            next = previousTransform.next;

            // set next of previous to this entity
            previousTransform.next = entity;
        }

        // link next to entity
        if (next != TOMBSTONE)
        {
            auto& nextTransform = registry.getComponent<HierarchyComponent>(next);
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
        auto& transform = registry.getComponent<HierarchyComponent>(entity);

        int sum = 0;

        entity_type current = transform.firstChild;
        while (current != TOMBSTONE)
        {
            auto& childTransform = registry.getComponent<HierarchyComponent>(current);
            current = childTransform.next;
            sum++;
        }

        transform.childCount = sum;
    }
}

#endif //SHAPEREALITY_HIERARCHY_H
