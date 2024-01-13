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
void createTestHierarchy(Registry& r)
{
    r.createEntity(rootId);
    r.createEntity(parentId);
    r.createEntity(child1Id);
    r.createEntity(child2Id);
    r.createEntity(child3Id);
    r.createEntity(root2Id);
    r.createEntity(parent2Id);
    r.createEntity(child4Id);
    r.createEntity(child5Id);
    r.createEntity(parent3Id);
    r.createEntity(child6Id);
    r.createEntity(child7Id);

    // hierarchy 1
    r.addComponent<HierarchyComponent>(rootId, HierarchyComponent{
        .hierarchyCount = 5,
        .childCount = 1,
        .firstChild = parentId,
    });
    r.addComponent<HierarchyComponent>(parentId, HierarchyComponent{
        .hierarchyCount = 4,
        .childCount = 3,
        .parent = rootId,
        .firstChild = child1Id,
    });
    r.addComponent<HierarchyComponent>(child1Id, HierarchyComponent{
        .parent = parentId,
        .next = child2Id
    });
    r.addComponent<HierarchyComponent>(child2Id, HierarchyComponent{
        .parent = parentId,
        .previous = child1Id,
        .next = child3Id
    });
    r.addComponent<HierarchyComponent>(child3Id, HierarchyComponent{
        .parent = parentId,
        .previous = child2Id,
    });

    // hierarchy 2
    r.addComponent<HierarchyComponent>(root2Id, HierarchyComponent{
        .hierarchyCount = 7,
        .childCount = 2,
        .firstChild = parent2Id
    });
    r.addComponent<HierarchyComponent>(parent2Id, HierarchyComponent{
        .hierarchyCount = 3,
        .childCount = 2,
        .parent = root2Id,
        .firstChild = child4Id,
        .next = parent3Id
    });
    r.addComponent<HierarchyComponent>(child4Id, HierarchyComponent{
        .parent = parent2Id,
        .next = child5Id
    });
    r.addComponent<HierarchyComponent>(child5Id, HierarchyComponent{
        .parent = parent2Id,
        .previous = child4Id
    });
    r.addComponent<HierarchyComponent>(parent3Id, HierarchyComponent{
        .hierarchyCount = 3,
        .childCount = 2,
        .parent = root2Id,
        .firstChild = child6Id,
        .previous = parent2Id
    });
    r.addComponent<HierarchyComponent>(child6Id, HierarchyComponent{
        .parent = parent3Id,
        .next = child7Id
    });
    r.addComponent<HierarchyComponent>(child7Id, HierarchyComponent{
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

TEST(Hierarchy, SetParentError)
{
    Registry r;
    createTestHierarchy(r);

    // test 1: TOMBSTONE
    ASSERT_FALSE(setParent(r, TOMBSTONE, parentId, 0));

    // test 2: out of range
    ASSERT_FALSE(setParent(r, parent2Id, parentId, 4));
    ASSERT_FALSE(setParent(r, child3Id, rootId, 2));

    // test 3: cyclical dependency
    ASSERT_FALSE(setParent(r, rootId, child3Id, 0));
}

TEST(Hierarchy, SetParent)
{
    Registry r;
    createTestHierarchy(r);

    // reparent child3 from parent to root
    ASSERT_TRUE(setParent(r, child3Id, rootId, 0));
    ASSERT_EQ(getChild(r, rootId, 0), child3Id);
    ASSERT_EQ(getChild(r, rootId, 1), parentId); // parent was moved to index one

    auto& root = r.getComponent<HierarchyComponent>(rootId);
    auto& parent = r.getComponent<HierarchyComponent>(parentId);
    ASSERT_EQ(root.childCount, 2);
    ASSERT_EQ(parent.childCount, 2);
    ASSERT_EQ(parent.hierarchyCount, 3);
}

TEST(Hierarchy, SetParent2)
{
    Registry r;
    createTestHierarchy(r);

    // reparent child6 from parent3 to root2 at index 2
    ASSERT_TRUE(setParent(r, child6Id, root2Id, 2));
    ASSERT_EQ(getChild(r, root2Id, 2), child6Id);
    ASSERT_EQ(getChild(r, parent3Id, 0), child7Id); // test if firstChild of parent3 becomes child7

    auto& root2 = r.getComponent<HierarchyComponent>(root2Id);
    auto& parent3 = r.getComponent<HierarchyComponent>(parent3Id);
    ASSERT_EQ(root2.childCount, 3);
    ASSERT_EQ(parent3.childCount, 1);
    ASSERT_EQ(root2.hierarchyCount, 7);
    ASSERT_EQ(parent3.hierarchyCount, 2);
}

TEST(Hierarchy, ClearParent)
{
    Registry r;
    createTestHierarchy(r);

    // clear parent
    ASSERT_TRUE(setParent(r, parent3Id, TOMBSTONE, 0));
    ASSERT_EQ(getChild(r, root2Id, 2), TOMBSTONE);

    auto& parent3 = r.getComponent<HierarchyComponent>(parent3Id);
    ASSERT_EQ(parent3.parent, TOMBSTONE);
}

TEST(Hierarchy, SetParent3)
{
    Registry r;
    createTestHierarchy(r);

    // reparent root2 to root, see if hierarchy count gets calculated properly
    ASSERT_TRUE(setParent(r, root2Id, rootId, 1));

    auto& root = r.getComponent<HierarchyComponent>(rootId);
    ASSERT_EQ(root.childCount, 2);
    ASSERT_EQ(root.hierarchyCount, 12);
}

TEST(Hierarchy, SetChildIndex)
{

}