//
// Created by Arjo Nagelhout on 10/01/2024.
//

#ifndef SHAPEREALITY_HIERARCHY_H
#define SHAPEREALITY_HIERARCHY_H

#include <entity/entity_registry.h>

#include <cassert>
#include <functional>

namespace entity
{
    // reusable hierarchy component that can be used to create a
    // node hierarchy or scene hierarchy for example

    // use composition over inheritance. In ECS, this can even mean adding two separate types:
    // add TransformComponent *and* HierarchyComponent
    struct HierarchyComponent final
    {
        // hierarchy
        size_type hierarchyCount{1}; // count of all child nodes + this node
        size_type childCount{0}; // amount of direct child nodes of this node
        size_type parent{kNullEntityId};
        size_type firstChild{kNullEntityId};

        // doubly-linked list
        size_type previous{kNullEntityId}; // previous sibling
        size_type next{kNullEntityId}; // next sibling
    };

    // whether `entity` is root, i.e. does not have a parent
    [[nodiscard]] bool isRoot(EntityRegistry& r, EntityId entityId);

    // whether `entity` is a child of `potentialParent`
    [[nodiscard]] bool isChildOf(EntityRegistry& r, EntityId entityId, EntityId potentialParentId);

    // whether `entity` is a parent of `potentialChild`
    [[nodiscard]] bool isParentOf(EntityRegistry& r, EntityId entityId, EntityId potentialChildId);

    // returns TOMBSTONE if no children, or if index outside of range of childCount
    [[nodiscard]] EntityId getChild(EntityRegistry& r, EntityId entityId, size_type index);

    // removes the entity from its parent
    bool remove(EntityRegistry& r, EntityId entityId);

    // insert entity to a parent at the given index
    bool insert(EntityRegistry& r, EntityId entityId, EntityId parentId, size_type index);

    /**
     * @param entityId entity to change the parent of
     * @param targetParentId target parent to add the entity to, if TOMBSTONE, entity will become root
     * @param childIndex the child index to insert the entity into
     */
    bool setParent(EntityRegistry& r, EntityId entityId, EntityId targetParentId, size_type childIndex);

    // sets the child index of the given entity
    bool setChildIndex(EntityRegistry& r, EntityId entityId, size_type childIndex);

    // iterates over the hierarchy of a given entityId using a depth first search (DFS) algorithm
    // at each entity, a provided lambda is called, which should return whether to recurse to its children
    void depthFirstSearch(EntityRegistry& r, EntityId entityId, std::function<bool(EntityId)> const& function);

    // sorts the entire hierarchy
    void sortHierarchy(EntityRegistry& lhsId);
}

#endif //SHAPEREALITY_HIERARCHY_H
