//
// Created by Arjo Nagelhout on 12/01/2024.
//

#include "hierarchy.h"

namespace entity
{
    bool isRoot(Registry& r, entity_type entityId)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error: provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);
        return entity.parent == TOMBSTONE;
    }

    bool isChildOf(Registry& r, entity_type entityId, entity_type potentialParentId)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error: provided entityId is TOMBSTONE
        }

        if (potentialParentId == TOMBSTONE)
        {
            return false; // error: provided potentialParentId is TOMBSTONE
        }

        // recurse up from entity to see if it has provided parent as its parent
        // this is quicker than iterating over all children

        entity_type current = entityId;
        while (current != TOMBSTONE)
        {
            auto& e = r.getComponent<HierarchyComponent>(current);
            if (potentialParentId == e.parent)
            {
                return true;
            }
            current = e.parent;
        }

        return false;
    }

    bool isParentOf(Registry& r, entity_type entityId, entity_type potentialChildId)
    {
        return isChildOf(r, potentialChildId, entityId);
    }

    entity_type getChild(Registry& r, entity_type entityId, size_type atIndex)
    {
        if (entityId == TOMBSTONE)
        {
            return TOMBSTONE; // error: provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        // assume childCount is correct
        if (atIndex > entity.childCount)
        {
            return TOMBSTONE; // error: atIndex out of range
        }

        entity_type current = entity.firstChild;
        size_type i = 0;
        while (i != atIndex)
        {
            auto& child = r.getComponent<HierarchyComponent>(current);
            current = child.next;
            i++;
        }

        return current;
    }

    bool setParent(Registry& r, entity_type entityId, entity_type targetParentId, size_type childIndex)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error, provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);
        entity_type originalParentId = entity.parent;

        // if `parent` is already the parent of `entity`
        if (originalParentId == targetParentId)
        {
            return true; // no error, but we don't have to do anything
        }

        // if `parent` is a child of `entity`,
        // this would result in a cyclical dependency
        if (isChildOf(r, targetParentId, entityId))
        {
            return false; // error, target `parent` is a child of `entity`
        }

        // early return, because otherwise we would already have unparented
        // the entity, and would have to revert that operation
        if (targetParentId != TOMBSTONE)
        {
            auto& targetParent = r.getComponent<HierarchyComponent>(targetParentId);
            if (childIndex > targetParent.childCount)
            {
                return false; // error, index out of range
            }
        }

        // connect siblings to each other where entity gets moved out from
        if (entity.previous != TOMBSTONE)
        {
            auto& previous = r.getComponent<HierarchyComponent>(entity.previous);
            previous.next = entity.next;
        }

        if (entity.next != TOMBSTONE)
        {
            auto& next = r.getComponent<HierarchyComponent>(entity.next);
            next.previous = entity.previous;
        }

        // fix firstChild of original parent
        if (originalParentId != TOMBSTONE)
        {
            // make sure we clear firstChild if it was set, and set
            auto& originalParent = r.getComponent<HierarchyComponent>(originalParentId);
            if (originalParent.firstChild == entityId)
            {
                // this means we have to set firstChild to entity.next
                // which could also be TOMBSTONE
                originalParent.firstChild = entity.next;
            }
        }

        // insert entity into target parent at given index
        if (targetParentId != TOMBSTONE)
        {
            // insert at the provided index in target parent
            auto& targetParent = r.getComponent<HierarchyComponent>(targetParentId);

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
                previousId = getChild(r, targetParentId, childIndex - 1);
                auto& previous = r.getComponent<HierarchyComponent>(previousId);
                nextId = previous.next;

                // set next of previous to this entity
                previous.next = entityId;
            }

            // link next to entity
            if (nextId != TOMBSTONE)
            {
                auto& next = r.getComponent<HierarchyComponent>(nextId);
                next.previous = entityId;
            }

            // update entity's previous and next
            entity.previous = previousId;
            entity.next = nextId;

            // update child count
            targetParent.childCount++;
        }

        // set entity's parent to target parent
        entity.parent = targetParentId;

        // update childCount
        if (originalParentId != TOMBSTONE)
        {
            auto& originalParent = r.getComponent<HierarchyComponent>(originalParentId);
            originalParent.childCount--;
        }

        // update hierarchyCount
        //
        // This might result in duplicate work, as we recurse all the way up to the root twice: once for decrementing
        // and once for incrementing hierarchyCount.
        //
        // However, otherwise we have to calculate, the lowest common ancestor (LCA) to know where to stop, which would
        // require depth to be precomputed. This would be recalculated on reparenting as well, and would require a
        // depth-first search (DFS) over all children of entity, which could be more costly.
        //
        // Maybe if a certain LCA algorithm does not require precomputing depth, then this optimisation can be
        // implemented.

        // amount to increment or decrement
        size_type entityHierarchyCount = entity.hierarchyCount;

        // subtract hierarchyCount of all entity's original parents
        entity_type currentId = originalParentId;
        while (currentId != TOMBSTONE)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            current.hierarchyCount -= entityHierarchyCount;
            currentId = current.parent;
        }

        // add hierarchyCount to all entity's target parents
        currentId = targetParentId;
        while (currentId != TOMBSTONE)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            current.hierarchyCount += entityHierarchyCount;
            currentId = current.parent;
        }

        return true;
    }
}