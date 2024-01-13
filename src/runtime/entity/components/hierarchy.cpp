//
// Created by Arjo Nagelhout on 12/01/2024.
//

#include "hierarchy.h"

namespace entity
{
    bool isChildOf(Registry& registry, entity_type entity, entity_type potentialParent)
    {
        assert(entity != TOMBSTONE);
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
        assert(entity != TOMBSTONE);

        auto& transform = registry.getComponent<HierarchyComponent>(entity);

        // assume childCount is correct
        if (atIndex > transform.childCount)
        {
            return TOMBSTONE;
        }

        entity_type current = transform.firstChild;
        size_type i = 0;
        while (i != atIndex)
        {
            auto& child = registry.getComponent<HierarchyComponent>(current);
            current = child.next;
            i++;
        }

        return current;
    }

    entity_type getLowestCommonAncestor(Registry& registry, entity_type lhs, entity_type rhs)
    {
        while (lhs != rhs)
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
                lhs = l.parent;
                rhs = r.parent;
            }
        }

        return lhs;
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

    bool setParent(Registry& registry, entity_type entityId, entity_type parentId, size_type childIndex)
    {
        if (entityId == TOMBSTONE || parentId == TOMBSTONE)
        {
            return false; // error, can't set invalid entity or parent
        }

        auto& entity = registry.getComponent<HierarchyComponent>(entityId);

        // if `parent` is already the parent of `entity`
        if (entity.parent == parentId)
        {
            return true; // no error, but we don't have to do anything
        }

        // if `parent` is a child of `entity`,
        // this would result in a cyclical dependency
        if (isChildOf(registry, parentId, entityId))
        {
            return false; // error, target `parent` is a child of `entity`
        }

        // ensure index not out of range
        auto& targetParent = registry.getComponent<HierarchyComponent>(parentId);
        if (childIndex > targetParent.childCount)
        {
            return false; // error, index out of range
        }

        // reconnect siblings to each other where entity gets moved out from
        if (entity.previous != TOMBSTONE)
        {
            auto& previous = registry.getComponent<HierarchyComponent>(entity.previous);
            previous.next = entity.next;
        }

        if (entity.next != TOMBSTONE)
        {
            auto& next = registry.getComponent<HierarchyComponent>(entity.next);
            next.previous = entity.previous;
        }

        entity_type previousId;
        entity_type nextId;

        if (childIndex == 0)
        {
            // set previous and next
            previousId = TOMBSTONE;
            nextId = targetParent.firstChild;

            // set the parent's first to this child
            targetParent.firstChild = entityId;
        }
        else
        {
            // set previous and next
            // get child we want to insert the entity to the right of, guaranteed to exist
            previousId = getChild(registry, parentId, childIndex - 1);
            auto& previous = registry.getComponent<HierarchyComponent>(previousId);
            nextId = previous.next;

            // set next of previous to this entity
            previous.next = entityId;
        }

        // link next to entity
        if (nextId != TOMBSTONE)
        {
            auto& next = registry.getComponent<HierarchyComponent>(nextId);
            next.previous = entityId;
        }

        // update entity's previous and next
        entity.previous = previousId;
        entity.next = nextId;

        // set entity's parent to target parent
        entity_type originalParentId = entity.parent;
        entity.parent = parentId;

        // fix to recalculate child count and hierarchy counts
        // hierarchy counts only need to be calculated up until,
        // but not including, the lowest common ancestor

        // update depth

        // update hierarchyCount

        // update childCount
        if (originalParentId != TOMBSTONE)
        {
            auto& originalParent = registry.getComponent<HierarchyComponent>(originalParentId);
            originalParent.childCount--;
        }

        targetParent.childCount++;

        return true;
    }
}