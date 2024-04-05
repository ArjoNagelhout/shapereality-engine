//
// Created by Arjo Nagelhout on 11/01/2024.
//

#include "gtest/gtest.h"

#include "entity/entity_registry.h"
#include "entity/components/hierarchy.h"

#include <algorithm>
#include <random>

using namespace entity;

namespace hierarchy_tests
{
    EntityId rootId = 0;
    EntityId parentId = 1;
    EntityId child1Id = 2;
    EntityId child2Id = 3;
    EntityId child3Id = 4;
    EntityId root2Id = 5;
    EntityId parent2Id = 6;
    EntityId child4Id = 7;
    EntityId child5Id = 8;
    EntityId parent3Id = 9;
    EntityId child6Id = 10;
    EntityId child7Id = 11;

    /* hierarchy
     *
     *          child5      8
     *          child4      7
     *          child3      4
     *      parent2         6
     *          child1      2
     *          child6      10
     *          child7      11
     *      parent3         9
     *  root2               5
     *          child2      3
     *      parent          1
     *  root                0
     */

    /* hierarchy
     *
     *  root                0
     *      parent          1
     *          child1      2
     *          child2      3
     *          child3      4
     *  root2               5
     *      parent2         6
     *          child4      7
     *          child5      8
     *      parent3         9
     *          child6      10
     *          child7      11
     */
    void createTestHierarchy(EntityRegistry& r)
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
        EntityRegistry r;

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
        EntityRegistry r;
        createTestHierarchy(r);

        ASSERT_EQ(getChild(r, root2Id, 0), parent2Id);
        ASSERT_EQ(getChild(r, parentId, 2), child3Id);
        ASSERT_EQ(getChild(r, parent2Id, 1), child5Id);
        ASSERT_EQ(getChild(r, rootId, 1), kNullEntityId);
        ASSERT_EQ(getChild(r, root2Id, 2), kNullEntityId);
        ASSERT_EQ(getChild(r, child1Id, 0), kNullEntityId);
    }

    TEST(Hierarchy, SetParentError)
    {
        EntityRegistry r;
        createTestHierarchy(r);

        // TOMBSTONE
        ASSERT_FALSE(setParent(r, kNullEntityId, parentId, 0));

        // out of range
        ASSERT_FALSE(setParent(r, parent2Id, parentId, 4));
        ASSERT_FALSE(setParent(r, child3Id, rootId, 2));

        // cyclical dependency
        ASSERT_FALSE(setParent(r, rootId, child3Id, 0));
    }

    TEST(Hierarchy, SetParent)
    {
        EntityRegistry r;
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
        EntityRegistry r;
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
        EntityRegistry r;
        createTestHierarchy(r);

        // clear parent
        ASSERT_TRUE(setParent(r, parent3Id, kNullEntityId, 0));
        ASSERT_EQ(getChild(r, root2Id, 2), kNullEntityId);

        auto& parent3 = r.getComponent<HierarchyComponent>(parent3Id);
        ASSERT_EQ(parent3.parent, kNullEntityId);
    }

    TEST(Hierarchy, SetParent3)
    {
        EntityRegistry r;
        createTestHierarchy(r);

        // reparent root2 to root, see if hierarchy count gets calculated properly
        ASSERT_TRUE(setParent(r, root2Id, rootId, 1));

        auto& root = r.getComponent<HierarchyComponent>(rootId);
        ASSERT_EQ(root.childCount, 2);
        ASSERT_EQ(root.hierarchyCount, 12);
    }

    TEST(Hierarchy, IsRoot)
    {
        EntityRegistry r;
        createTestHierarchy(r);

        // TOMBSTONE
        ASSERT_FALSE(isRoot(r, kNullEntityId));

        ASSERT_FALSE(isRoot(r, child7Id));
        ASSERT_FALSE(isRoot(r, parentId));
        ASSERT_TRUE(isRoot(r, rootId));
        ASSERT_TRUE(isRoot(r, root2Id));
    }

    TEST(Hierarchy, SetChildIndex)
    {
        EntityRegistry r;
        createTestHierarchy(r);

        // TOMBSTONE
        ASSERT_FALSE(setChildIndex(r, kNullEntityId, 0));

        // out of index
        ASSERT_FALSE(setChildIndex(r, child3Id, 3));

        // update first child
        ASSERT_TRUE(setChildIndex(r, child3Id, 0));
        auto& parent = r.getComponent<HierarchyComponent>(parentId);
        ASSERT_EQ(parent.firstChild, child3Id);

        ASSERT_TRUE(setChildIndex(r, child6Id, 1));
        ASSERT_EQ(getChild(r, parent3Id, 0), child7Id);
        ASSERT_EQ(getChild(r, parent3Id, 1), child6Id);
    }

    TEST(Hierarchy, DepthFirstSearch)
    {
        EntityRegistry r;
        createTestHierarchy(r);

        // test 1
        std::vector<EntityId> result{};
        std::vector<EntityId> expected{
            rootId, parentId, child3Id, child2Id, child1Id
        };

        depthFirstSearch(r, rootId, [&result](EntityId entityId) {
            result.emplace_back(entityId);
            return true;
        });

        ASSERT_EQ(result, expected);

        // test 2
        std::vector<EntityId> result2{};
        std::vector<EntityId> expected2{
            root2Id, parent3Id, child7Id, child6Id, parent2Id, child5Id, child4Id
        };

        depthFirstSearch(r, root2Id, [&result2](EntityId entityId) {
            result2.emplace_back(entityId);
            return true;
        });
        ASSERT_EQ(result2, expected2);

        // test 3, single entity
        std::vector<EntityId> result3{};
        std::vector<EntityId> expected3{child4Id};

        depthFirstSearch(r, child4Id, [&result3](EntityId entityId) {
            result3.emplace_back(entityId);
            return true;
        });
        ASSERT_EQ(result3, expected3);
    }

    TEST(Hierarchy, Sort)
    {
        EntityRegistry r;
        createTestHierarchy(r);


        std::vector<float> values(child7Id+1);
        std::iota(values.begin(), values.end(), 0);

        auto rng = std::default_random_engine{};
        rng.seed(2253323);
        std::shuffle(std::begin(values), std::end(values), rng);
//
//        for (auto v: values)
//        {
//            std::cout << v << std::endl;
//        }

        // unsort the hierarchy
        for (size_t i = 0; i < values.size(); ++i)
        {
            r.addComponent<int>(i, values[i]);
        }

        r.sort<HierarchyComponent>([&r](EntityId lhs, EntityId rhs){
            auto const& clhs = r.getComponent<int>(lhs);
            auto const& crhs = r.getComponent<int>(rhs);
            return clhs < crhs;
        });

        std::cout << "unsorted: " << std::endl;
        for (auto [entityId, hierarchy] : r.view<HierarchyComponent>())
        {
            std::cout << entityId << std::endl;
        }

        sortHierarchy(r);

        std::cout << "sorted: " << std::endl;
        for (auto [entityId, hierarchy] : r.view<HierarchyComponent>())
        {
            std::cout << entityId << std::endl;
        }
    }
}