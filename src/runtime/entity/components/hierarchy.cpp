//
// Created by Arjo Nagelhout on 12/01/2024.
//

#include "hierarchy.h"

namespace entity
{
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

    entity_type getLowestCommonAncestor(Registry& registry, entity_type lhs, entity_type rhs)
    {
        while (lhs != TOMBSTONE && rhs != TOMBSTONE)
        {
            auto& l = registry.getComponent<HierarchyComponent>(lhs);
            auto& r = registry.getComponent<HierarchyComponent>(rhs);

            if (l.depth > r.depth)
            {
                lhs = l.parent;
            }
            else if (l.depth < r.depth)
            {
                rhs = r.parent;
            }
            else
            {
                // depth equals, iterate both

            }
        }
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

    bool setParent(Registry& registry, entity_type entity, entity_type parent, size_type childIndex)
    {
        assert(parent != TOMBSTONE);

        auto& transform = registry.getComponent<HierarchyComponent>(entity);

        // if `parent` is already the parent of `entity`
        if (transform.parent == parent)
        {
            return true; // no error, but we don't have to do anything
        }

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

        // fix to recalculate child count and hierarchy counts
        // hierarchy counts only need to be calculated up until,
        // but not including, the lowest common ancestor

        return true;
    }
}