//
// Created by Arjo Nagelhout on 11/01/2024.
//

#include "gtest/gtest.h"

#include "entity/registry.h"
#include "entity/components/hierarchy.h"

using namespace entity;

entity_type rootId = 0;
entity_type parentId = 1;
entity_type child1Id = 2;
entity_type child2Id = 3;
entity_type child3Id = 4;

void createTestHierarchy(Registry& registry)
{
    registry.createEntity(rootId);
    registry.createEntity(parentId);
    registry.createEntity(child1Id);
    registry.createEntity(child2Id);
    registry.createEntity(child3Id);

    registry.addComponent<HierarchyComponent>(rootId, HierarchyComponent{
        .hierarchyCount = 5,
        .childCount = 1,
        .firstChild = parentId,
    });
    registry.addComponent<HierarchyComponent>(parentId, HierarchyComponent{
        .hierarchyCount = 4,
        .childCount = 3,
        .parent = rootId,
        .firstChild = child1Id,
    });
    registry.addComponent<HierarchyComponent>(child1Id, HierarchyComponent{
        .parent = parentId,
        .next = child2Id
    });
    registry.addComponent<HierarchyComponent>(child2Id, HierarchyComponent{
        .parent = parentId,
        .previous = child1Id,
        .next = child3Id
    });
    registry.addComponent<HierarchyComponent>(child3Id, HierarchyComponent{
        .parent = parentId,
        .previous = child2Id,
        .next = TOMBSTONE
    });
}

// for the test, we manually create the data structure, as
// the automatic creation of the data structure is dependent on
// this function (setParent uses isChildOf).
TEST(Hierarchy, IsChildOfIsParentOf)
{
    Registry r;

    createTestHierarchy(r);

    ASSERT_TRUE(isChildOf(r, parentId, rootId));
    ASSERT_FALSE(isChildOf(r, rootId, parentId));
    ASSERT_TRUE(isParentOf(r, rootId, parentId));
    ASSERT_FALSE(isParentOf(r, parentId, rootId));

    ASSERT_TRUE(isChildOf(r, child3Id, rootId));
    ASSERT_TRUE(isParentOf(r, rootId, child2Id));
}

// validate whether the hierarchyCount gets calculated properly
TEST(Hierarchy, HierarchyCount)
{
    Registry r;
    createTestHierarchy(r);

    computeHierarchyCountRecurseUp(r, child3Id);

    auto& root = r.getComponent<HierarchyComponent>(rootId);
    ASSERT_EQ(root.hierarchyCount, 5);

    auto& parent = r.getComponent<HierarchyComponent>(parentId);
    ASSERT_EQ(parent.hierarchyCount, 4);
}

TEST(Hierarchy, LowestCommonAncestor)
{
    Registry r;
}

// validate whether the childCount gets calculated properly
TEST(Hierarchy, ChildCount)
{
    Registry r;
    createTestHierarchy(r);


}

// edge cases:
// child index == TOMBSTONE should clear parent
// child index > size() should be out of range
//
TEST(Hierarchy, SetParent)
{

}
