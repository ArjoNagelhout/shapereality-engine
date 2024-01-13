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
entity_type root2Id = 5;
entity_type parent2Id = 6;
entity_type child4Id = 7;
entity_type child5Id = 8;
entity_type parent3Id = 9;
entity_type child6Id = 10;
entity_type child7Id = 11;

/* hierarchy
 *
 *  root
 *      parent
 *          child1
 *          child2
 *          child3
 *  root2
 *      parent2
 *          child4
 *          child5
 *      parent3
 *          child6
 *          child7
 */
void createTestHierarchy(Registry& registry)
{
    registry.createEntity(rootId);
    registry.createEntity(parentId);
    registry.createEntity(child1Id);
    registry.createEntity(child2Id);
    registry.createEntity(child3Id);
    registry.createEntity(root2Id);
    registry.createEntity(parent2Id);
    registry.createEntity(child4Id);
    registry.createEntity(child5Id);
    registry.createEntity(parent3Id);
    registry.createEntity(child6Id);
    registry.createEntity(child7Id);

    // the test hierarchy contains two separate trees (these are not

    // hierarchy 1
    registry.addComponent<HierarchyComponent>(rootId, HierarchyComponent{
        .hierarchyCount = 5,
        .childCount = 1,
        .depth = 0,
        .firstChild = parentId,
    });
    registry.addComponent<HierarchyComponent>(parentId, HierarchyComponent{
        .hierarchyCount = 4,
        .childCount = 3,
        .depth = 1,
        .parent = rootId,
        .firstChild = child1Id,
    });
    registry.addComponent<HierarchyComponent>(child1Id, HierarchyComponent{
        .depth = 2,
        .parent = parentId,
        .next = child2Id
    });
    registry.addComponent<HierarchyComponent>(child2Id, HierarchyComponent{
        .depth = 2,
        .parent = parentId,
        .previous = child1Id,
        .next = child3Id
    });
    registry.addComponent<HierarchyComponent>(child3Id, HierarchyComponent{
        .depth = 2,
        .parent = parentId,
        .previous = child2Id,
    });

    // hierarchy 2
    registry.addComponent<HierarchyComponent>(root2Id, HierarchyComponent{
        .hierarchyCount = 7,
        .childCount = 2,
        .depth = 0,
        .firstChild = parent2Id
    });
    registry.addComponent<HierarchyComponent>(parent2Id, HierarchyComponent{
        .hierarchyCount = 3,
        .childCount = 2,
        .depth = 1,
        .parent = root2Id,
        .firstChild = child4Id,
        .next = parent3Id
    });
    registry.addComponent<HierarchyComponent>(child4Id, HierarchyComponent{
        .depth = 2,
        .parent = parent2Id,
        .next = child5Id
    });
    registry.addComponent<HierarchyComponent>(child5Id, HierarchyComponent{
        .depth = 2,
        .parent = parent2Id,
        .previous = child4Id
    });
    registry.addComponent<HierarchyComponent>(parent3Id, HierarchyComponent{
        .hierarchyCount = 3,
        .childCount = 2,
        .depth = 1,
        .parent = root2Id,
        .firstChild = child6Id,
        .previous = parent2Id
    });
    registry.addComponent<HierarchyComponent>(child6Id, HierarchyComponent{
        .depth = 2,
        .parent = parent3Id,
        .next = child7Id
    });
    registry.addComponent<HierarchyComponent>(child7Id, HierarchyComponent{
        .depth = 2,
        .parent = parent3Id,
        .previous = child6Id
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
}

TEST(Hierarchy, LowestCommonAncestor)
{
    Registry r;
    createTestHierarchy(r);

    ASSERT_EQ(getLowestCommonAncestor(r, child4Id, child7Id), root2Id);
    ASSERT_EQ(getLowestCommonAncestor(r, rootId, rootId), rootId);
    ASSERT_EQ(getLowestCommonAncestor(r, child7Id, parentId), TOMBSTONE);
    ASSERT_EQ(getLowestCommonAncestor(r, child1Id, child3Id), parentId);
    ASSERT_EQ(getLowestCommonAncestor(r, child7Id, root2Id), root2Id);
}

// validate whether the childCount gets calculated properly
TEST(Hierarchy, ChildCount)
{
    Registry r;
    createTestHierarchy(r);


}

TEST(Hierarchy, GetChild)
{
    Registry r;
    createTestHierarchy(r);

    ASSERT_EQ(getChild(r, root2Id, 0), parent2Id);
    ASSERT_EQ(getChild(r, parentId, 2), child3Id);
    ASSERT_EQ(getChild(r, parent2Id, 1), child5Id);
    ASSERT_EQ(getChild(r, rootId, 1), TOMBSTONE);
    ASSERT_EQ(getChild(r, root2Id, 2), TOMBSTONE);
    ASSERT_EQ(getChild(r, child1Id, 0), TOMBSTONE);
}

TEST(Hierarchy, Clear)
{
    Registry r;
    createTestHierarchy(r);

    ASSERT_EQ(r.entityCount(), 12);

    r.clear();

    ASSERT_EQ(r.entityCount(), 0);
}

TEST(Hierarchy, ComputeDepth)
{
    Registry r;
    createTestHierarchy(r);
}

TEST(Hierarchy, SetParent)
{
    Registry r;
    createTestHierarchy(r);

    // out of range
    ASSERT_FALSE(setParent(r, parent2Id, parentId, 4));
    ASSERT_FALSE(setParent(r, child3Id, rootId, 2));

    auto& root = r.getComponent<HierarchyComponent>(rootId);
    auto& parent = r.getComponent<HierarchyComponent>(parentId);

    ASSERT_EQ(root.childCount, 1);
    ASSERT_EQ(parent.childCount, 3);

    ASSERT_TRUE(setParent(r, child3Id, rootId, 0));
    ASSERT_EQ(getChild(r, rootId, 0), child3Id);
    ASSERT_EQ(getChild(r, rootId, 1), parentId); // parent was moved to index one
    ASSERT_EQ(root.childCount, 2);
    ASSERT_EQ(parent.childCount, 2);

    // reset hierarchy
    r.clear();
    createTestHierarchy(r);


}
