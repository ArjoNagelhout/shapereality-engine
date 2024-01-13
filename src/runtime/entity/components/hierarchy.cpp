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

    entity_type getChild(Registry& r, entity_type entityId, size_type index)
    {
        if (entityId == TOMBSTONE)
        {
            return TOMBSTONE; // error: provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        // assume childCount is correct
        if (index > entity.childCount)
        {
            return TOMBSTONE; // error: atIndex out of range
        }

        entity_type currentId = entity.firstChild;
        size_type i = 0;
        while (i != index)
        {
            auto& current = r.getComponent<HierarchyComponent>(currentId);
            currentId = current.next;
            i++;
        }

        return currentId;
    }

    // recurse up tree to change hierarchy count by provided delta
    void internalUpdateHierarchyCount(Registry& r, entity_type entityId, int delta)
    {
        while (entityId != TOMBSTONE)
        {
            auto& entity = r.getComponent<HierarchyComponent>(entityId);
            entity.hierarchyCount += delta;
            entityId = entity.parent;
        }
    }

    // warning:
    // - does not update hierarchy count or child count, should be done manually
    // - does not provide checks, should be done before calling this function,
    //   this is done to avoid checking each property double
    void internalInsert(Registry& r,
                        entity_type entityId, HierarchyComponent& entity,
                        entity_type parentId, HierarchyComponent& parent,
                        size_type index)
    {
        entity_type previousId;
        entity_type nextId;

        if (index == 0)
        {
            // set previous and next
            previousId = TOMBSTONE;
            nextId = parent.firstChild;

            // set the parent's first to this child
            parent.firstChild = entityId;
        }
        else
        {
            // set previous and next
            // get child we want to insert the entity to the right of, guaranteed to exist
            previousId = getChild(r, parentId, index - 1);
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

        // set entity's parent to target parent
        entity.parent = parentId;
    }

    // warning:
    // - does not update hierarchy count or child count, this should be done manually
    // - does not provide checks, should be done before calling this function
    // entity should have a parent
    void internalRemove(Registry& r, entity_type entityId, HierarchyComponent& entity, HierarchyComponent& parent)
    {
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
        if (parent.firstChild == entityId)
        {
            // this means we have to set firstChild to entity.next
            // which could also be TOMBSTONE
            parent.firstChild = entity.next;
        }

        entity.parent = TOMBSTONE;
    }

    // updates hierarchy and child count
    void internalInsertAndUpdateCounts(Registry& r,
                                       entity_type entityId, HierarchyComponent& entity,
                                       entity_type parentId, HierarchyComponent& parent,
                                       size_type index)
    {
        internalInsert(r, entityId, entity, parentId, parent, index);

        // update child count
        parent.childCount++;

        // add hierarchyCount to all entity's target parents
        internalUpdateHierarchyCount(r, parentId, static_cast<int>(entity.hierarchyCount));
    }

    // updates hierarchy and child count
    void internalRemoveAndUpdateCounts(Registry& r,
                                       entity_type entityId, HierarchyComponent& entity,
                                       entity_type parentId, HierarchyComponent& parent)
    {
        internalRemove(r, entityId, entity, parent);

        // update child count
        parent.childCount--;

        internalUpdateHierarchyCount(r, parentId, -static_cast<int>(entity.hierarchyCount));
    }

    bool insert(Registry& r, entity_type entityId, entity_type parentId, size_type index)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error, provided entityId is TOMBSTONE
        }

        if (parentId == TOMBSTONE)
        {
            return false; // error, provided parentId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        if (entity.parent != TOMBSTONE)
        {
            return false; // error, can't insert entity that already has parent, entity should be removed first
        }

        if (isChildOf(r, parentId, entityId))
        {
            return false; // error, provided entity is parent of provided parent, which would result in a cyclical dependency
        }

        auto& parent = r.getComponent<HierarchyComponent>(parentId);

        if (index > parent.childCount)
        {
            return false; // error, index out of range
        }

        internalInsertAndUpdateCounts(r, entityId, entity, parentId, parent, index);

        return true;
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

        auto& parent = r.getComponent<HierarchyComponent>(parentId);

        internalRemoveAndUpdateCounts(r, entityId, entity, parentId, parent);

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
        if (originalParentId == targetParentId)
        {
            return true; // no error, but `parent` is already the parent of `entity`
        }

        if (isChildOf(r, targetParentId, entityId))
        {
            return false; // error, target `parent` is a child of `entity`, which would result in a cyclical dependency
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

        // remove entity from original parent
        if (originalParentId != TOMBSTONE)
        {
            auto& originalParent = r.getComponent<HierarchyComponent>(originalParentId);
            internalRemoveAndUpdateCounts(r, entityId, entity, originalParentId, originalParent);
        }

        // insert entity into target parent at given index
        if (targetParentId != TOMBSTONE)
        {
            auto& targetParent = r.getComponent<HierarchyComponent>(targetParentId);
            internalInsertAndUpdateCounts(r, entityId, entity, targetParentId, targetParent, childIndex);
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

        entity_type parentId = entity.parent;
        if (parentId == TOMBSTONE)
        {
            return false; // error: provided entity does not have a parent (is root), so we can't set its child index
        }

        // we don't know the child index of a given node, so we need to iterate over
        // the children, similar to getChild,
        // then, similar to setParent, we "reconnect" the previous and next sibling of the entity

        auto& parent = r.getComponent<HierarchyComponent>(parentId);
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

        internalRemove(r, entityId, entity, parent);
        internalInsert(r, entityId, entity, parentId, parent, childIndex);

        // we don't have to change the hierarchy count or child count of the parents, as this
        // doesn't change when only changing the child index of the provided entity
        return true;
    }
}