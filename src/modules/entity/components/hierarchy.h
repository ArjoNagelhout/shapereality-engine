//
// Created by Arjo Nagelhout on 10/01/2024.
//

#ifndef SHAPEREALITY_HIERARCHY_H
#define SHAPEREALITY_HIERARCHY_H

#include "../registry.h"
#include <cassert>

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
        size_type parent{TOMBSTONE};
        size_type firstChild{TOMBSTONE};

        // doubly-linked list
        size_type previous{TOMBSTONE}; // previous sibling
        size_type next{TOMBSTONE}; // next sibling
    };

    // whether `entity` is root, i.e. does not have a parent
    [[nodiscard]] bool isRoot(Registry& r, entity_type entityId);

    // whether `entity` is a child of `potentialParent`
    [[nodiscard]] bool isChildOf(Registry& r, entity_type entityId, entity_type potentialParentId);

    // whether `entity` is a parent of `potentialChild`
    [[nodiscard]] bool isParentOf(Registry& r, entity_type entityId, entity_type potentialChildId);

    // returns TOMBSTONE if no children, or if index outside of range of childCount
    [[nodiscard]] entity_type getChild(Registry& r, entity_type entityId, size_type index);

    // removes the entity from its parent
    bool remove(Registry& r, entity_type entityId);

    // insert entity to a parent at the given index
    bool insert(Registry& r, entity_type entityId, entity_type parentId, size_type index);

    /**
     * @param entityId entity to change the parent of
     * @param targetParentId target parent to add the entity to, if TOMBSTONE, entity will become root
     * @param childIndex the child index to insert the entity into
     */
    bool setParent(Registry& r, entity_type entityId, entity_type targetParentId, size_type childIndex);

    // sets the child index of the given entity
    bool setChildIndex(Registry& r, entity_type entityId, size_type childIndex);

    // iterates over the hierarchy of a given entityId using a depth first search (DFS) algorithm
    // at each entity, a provided lambda is called, which should return whether to continue
    void depthFirstSearch(Registry& r, entity_type entityId, std::function<bool(entity_type)> const& function);
}

#endif //SHAPEREALITY_HIERARCHY_H
