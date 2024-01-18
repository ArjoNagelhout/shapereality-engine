//
// Created by Arjo Nagelhout on 18/01/2024.
//

#include "gtest/gtest.h"

#include "entity/registry.h"

using namespace entity;

// namespace is to avoid duplicate symbols
namespace sparse_set_tests
{
    struct ParentComponent
    {
        entity_type parentId = TOMBSTONE;
    };

    entity_type root = 0;
    entity_type parent1 = 1;
    entity_type child1 = 2;
    entity_type child2 = 3;
    entity_type parent2 = 4;
    entity_type child3 = 5;
    entity_type child4 = 6;
    entity_type root2 = 7;
    entity_type child5 = 8;
    entity_type root3 = 9;
    entity_type child6 = 10;

    std::vector<entity_type> nodes{
        root, parent1, child1, child2, parent2, child3, child4, root2, child5, root3, child6
    };

    /*
     * - root               0
     *      - parent1       1
     *          - child1    2
     *          - child2    3
     *      - parent2       4
     *          - child3    5
     *          - child4    6
     * - root2              7
     *      - child5        8
     * - root3              9
     *      - child6        10
     * parent2 should always be before any of its children.
     * i.e.
     */
    void createTestHierarchy(Registry& r)
    {
        for (auto nodeId : nodes)
        {
            r.createEntity(nodeId);
        }

        r.addComponent<ParentComponent>(root);
        r.addComponent<ParentComponent>(parent1, ParentComponent{root});
        r.addComponent<ParentComponent>(child1, ParentComponent{parent1});
        r.addComponent<ParentComponent>(child2, ParentComponent{parent2});
        r.addComponent<ParentComponent>(parent2, ParentComponent{root});
        r.addComponent<ParentComponent>(child3, ParentComponent{parent2});
        r.addComponent<ParentComponent>(child4, ParentComponent{parent2});
        r.addComponent<ParentComponent>(root2);
        r.addComponent<ParentComponent>(child5, ParentComponent{root2});
        r.addComponent<ParentComponent>(root3);
        r.addComponent<ParentComponent>(child6, ParentComponent{root3});
    }

    // as sort is generic (not just for hierarchy), it should work on
    // different types of data
    TEST(SparseSet, Sort)
    {
        Registry r;
        createTestHierarchy(r);

        // direct sort using component type

        auto* parentType = r.getComponentType<ParentComponent>();

        parentType->sort([&r](entity_type lhs, entity_type rhs){
            auto& rhsValue = r.getComponent<ParentComponent>(rhs);

            return true;
        });
    }

    TEST(SparseSet, Sort2)
    {
        // sort using sort function exposed to registry
    }
}