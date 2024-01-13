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
        size_type depth{0}; // for each parent a node has, depth increases by one. This enables fast lowest-common-ancestor algorithm.
        size_type parent{TOMBSTONE};
        size_type firstChild{TOMBSTONE};

        // doubly-linked list
        size_type previous{TOMBSTONE}; // previous sibling
        size_type next{TOMBSTONE}; // next sibling
    };

    // whether `entity` is a child of `potentialParent`
    bool isChildOf(Registry& registry, entity_type entity, entity_type potentialParent);

    // whether `entity` is a parent of `potentialChild`
    bool isParentOf(Registry& registry, entity_type entity, entity_type potentialChild);

    // returns TOMBSTONE if no children, or if index outside of range of childCount
    [[nodiscard]] entity_type getChild(Registry& registry, entity_type entity, size_type atIndex);

    // returns the lowest common ancestor of the two provided entities,
    // if no common ancestor exists, it returns TOMBSTONE
    [[nodiscard]] entity_type getLowestCommonAncestor(Registry& registry, entity_type lhs, entity_type rhs);

    void computeHierarchyCountRecurseUp(Registry& registry, entity_type entity);

    // note: we probably don't need this, as we can use setParent to alter the child count
    void computeChildCount(Registry& registry, entity_type entity);

    /**
     * @param entityId entity to change the parent of
     * @param parentId the entity will be added as a child to the parent
     * @param childIndex the child index to insert the entity into
     */
    bool setParent(Registry& registry, entity_type entityId, entity_type parentId, size_type childIndex);
}

#endif //SHAPEREALITY_HIERARCHY_H
