//
// Created by Arjo Nagelhout on 12/01/2024.
//

#include "hierarchy.h"

#include <stack>

namespace entity
{
    bool isRoot(Registry& r, Entity entityId)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error: provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);
        return entity.parent == TOMBSTONE;
    }

    bool isChildOf(Registry& r, Entity entityId, Entity potentialParentId)
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

        Entity currentId = entityId;
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

    bool isParentOf(Registry& r, Entity entityId, Entity potentialChildId)
    {
        return isChildOf(r, potentialChildId, entityId);
    }

    Entity getChild(Registry& r, Entity entityId, size_type index)
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

        Entity currentId = entity.firstChild;
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
    void internalUpdateHierarchyCount(Registry& r, Entity entityId, int delta)
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
    // - does not provide checks, should be done before calling this function
    void internalInsert(Registry& r,
                        Entity entityId, HierarchyComponent& entity,
                        Entity parentId, HierarchyComponent& parent,
                        size_type index)
    {
        Entity previousId;
        Entity nextId;

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
    void internalRemove(Registry& r, Entity entityId, HierarchyComponent& entity, HierarchyComponent& parent)
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
                                       Entity entityId, HierarchyComponent& entity,
                                       Entity parentId, HierarchyComponent& parent,
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
                                       Entity entityId, HierarchyComponent& entity,
                                       Entity parentId, HierarchyComponent& parent)
    {
        internalRemove(r, entityId, entity, parent);

        // update child count
        parent.childCount--;

        // subtract hierarchy count from all parents, recursively
        internalUpdateHierarchyCount(r, parentId, -static_cast<int>(entity.hierarchyCount));
    }

    bool insert(Registry& r, Entity entityId, Entity parentId, size_type index)
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

    bool remove(Registry& r, Entity entityId)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error, provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        Entity parentId = entity.parent;
        if (parentId == TOMBSTONE)
        {
            return true; // no error, but we don't have to do anything as it is already fully removed from any hierarchy
        }

        auto& parent = r.getComponent<HierarchyComponent>(parentId);
        internalRemoveAndUpdateCounts(r, entityId, entity, parentId, parent);

        return true;
    }

    bool setParent(Registry& r, Entity entityId, Entity targetParentId, size_type childIndex)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error, provided entityId is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        Entity originalParentId = entity.parent;
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

    bool setChildIndex(Registry& r, Entity entityId, size_type childIndex)
    {
        if (entityId == TOMBSTONE)
        {
            return false; // error: provided entity is TOMBSTONE
        }

        auto& entity = r.getComponent<HierarchyComponent>(entityId);

        Entity parentId = entity.parent;
        if (parentId == TOMBSTONE)
        {
            return false; // error: provided entity does not have a parent (is root), so we can't set its child index
        }

        auto& parent = r.getComponent<HierarchyComponent>(parentId);
        if (childIndex > parent.childCount - 1)
        {
            return false; // error, index out of range
        }

        Entity targetId = parent.firstChild;
        size_type i = 0;
        while (i != childIndex)
        {
            auto& target = r.getComponent<HierarchyComponent>(targetId);
            targetId = target.next;
            i++;
        }

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

    void depthFirstSearch(Registry& r, Entity entityId, std::function<bool(Entity)> const& function)
    {
        if (entityId == TOMBSTONE)
        {
            return; // error: provided entityId is TOMBSTONE
        }

        if (!r.entityContainsComponent<HierarchyComponent>(entityId))
        {
            return;
        }

        std::stack<Entity> stack;
        stack.push(entityId);

        while (!stack.empty())
        {
            Entity const currentId = stack.top();
            stack.pop();

            // iterate over children, otherwise don't add anything
            // we assume that the top of the stack is never TOMBSTONE and contains a Hierarchy component
            auto& current = r.getComponent<HierarchyComponent>(currentId);

            // call lambda
            bool const shouldContinue = function(currentId);
            if (shouldContinue)
            {
                Entity childId = current.firstChild;

                // this means that children are iterated over in reverse order
                while (childId != TOMBSTONE)
                {
                    auto& child = r.getComponent<HierarchyComponent>(childId);
                    stack.push(childId);
                    childId = child.next;
                }
            }
        }
    }

    void sortHierarchy(Registry& r)
    {
        // sorts with reverse order
        r.sort<HierarchyComponent>([&r](Entity lhsId, Entity rhsId) {
            auto const& lhs = r.getComponent<HierarchyComponent>(lhsId);
            auto const& rhs = r.getComponent<HierarchyComponent>(rhsId);

            if (rhsId == lhsId)
            {
                return false;
            }

            // if any parent of rhs is lhs, lhs should come before rhs
            Entity parentId = lhsId;
            while (parentId != TOMBSTONE)
            {
                if (rhsId == parentId)
                {
                    // lhs should come before rhs
                    return false;
                }

                auto const& parent = r.getComponent<HierarchyComponent>(parentId);
                parentId = parent.parent;
            }

            return true;
        });
    }
}