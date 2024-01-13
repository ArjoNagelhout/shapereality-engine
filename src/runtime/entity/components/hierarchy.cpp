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

        entity_type currentId = entityId;
        while (currentId != TOMBSTONE)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            if (potentialParentId == current.parent)
            {
                return true;
            }
            currentId = current.parent;
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

        entity_type currentId = entity.firstChild;
        size_type i = 0;
        while (i != atIndex)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            currentId = current.next;
            i++;
        }

        return currentId;
    }

    bool remove(Registry& r, entity_type entityId)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error, provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        entity_type parentId = entity.parent;
        if (parentId == TOMBSTONE)
        {
            return true; // no error, but we don't have to do anything as it is already fully removed from any hierarchy
        }

        // reconnect siblings
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

        // fix firstChild
        auto& parent = r.getComponent<HierarchyComponent>(parentId);
        if (parent.firstChild == entityId)
        {
            // this means we have to set firstChild to entity.next
            // which could also be TOMBSTONE
            parent.firstChild = entity.next;
        }

        // update child count
        parent.childCount--;

        // subtract hierarchyCount of all entity's original parents
        entity_type currentId = parentId;
        while (currentId != TOMBSTONE)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            current.hierarchyCount -= entity.hierarchyCount;
            currentId = current.parent;
        }

        return true;
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

        remove(r, entityId);

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


        // add hierarchyCount to all entity's target parents
        entity_type currentId = targetParentId;
        while (currentId != TOMBSTONE)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            current.hierarchyCount += entityHierarchyCount;
            currentId = current.parent;
        }

        return true;
    }

    bool setChildIndex(Registry& r, entity_type entityId, size_type childIndex)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error: provided entity is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        if (entity.parent == TOMBSTONE)
        {
            return false; // error: provided entity does not have a parent (is root), so we can't set its child index
        }

        // we don't know the child index of a given node, so we need to iterate over
        // the children, similar to getChild,
        // then, similar to setParent, we "reconnect" the previous and next sibling of the entity

        auto& parent = r.getComponent<HierarchyComponent>(entity.parent);
        if (childIndex > parent.childCount - 1)
        {
            return false; // error, index out of range
        }

        entity_type currentId = parent.firstChild;
        size_type i = 0;
        while (i != childIndex)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            currentId = current.next;
            i++;
        }

        entity_type targetId = currentId;

        if (targetId == entityId)
        {
            return true; // no error, but we don't have to do anything as entity already is at given index
        }

        // remove (reconnect siblings)
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

        // insert

    }
}