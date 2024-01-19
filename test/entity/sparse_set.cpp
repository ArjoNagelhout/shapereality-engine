//
// Created by Arjo Nagelhout on 18/01/2024.
//

#include "gtest/gtest.h"

#include "entity/registry.h"

//#include <random>

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

        for (auto [entityId, parent]: r.view<ParentComponent>())
        {
            std::cout << entityId << std::endl;
        }

        auto* parentType = r.getComponentType<ParentComponent>();

        parentType->sort([&r](entity_type lhs, entity_type rhs){
            auto& lhsValue = r.getComponent<ParentComponent>(lhs);
            entity_type const parentId = lhsValue.parentId;
            // return true if lhs should be earlier in array than rhs

            // lhs should be earlier in array if its parent id is TOMBSTONE
            // otherwise we should check if its parent id is less than rhs

            return parentId == TOMBSTONE || parentId < rhs;
        });

        for (auto [entityId, parent]: r.view<ParentComponent>())
        {
            std::cout << entityId << std::endl;
        }

//        - root             0
//           - parent1       1

//               - child2    3

//               - child3    5
//               - child4    6
//      - root2              7
//           - child5        8
//           - parent2       4
//      - root3              9
//               - child1    2
//           - child6        10
    }

    struct Test1
    {
        float value = 0.0f;
    };

    TEST(SparseSet, Sort2)
    {
        Registry r;

        // sort using sort function exposed to registry
        for (int i = 0; i < 20; i++)
        {
            r.createEntity(i);
            r.addComponent<Test1>(i);

            auto& component = r.getComponent<Test1>(i);
            component.value = static_cast<float>(rand() % 20);
        }

        for (auto [entityId, test1]: r.view<Test1>())
        {
            std::cout << test1.value << std::endl;
        }

        r.sort<Test1>([&r](entity_type lhs, entity_type rhs){
            auto& lhsValue = r.getComponent<Test1>(lhs);
            auto& rhsValue = r.getComponent<Test1>(rhs);

            return lhsValue.value < rhsValue.value;
        });

        std::cout << "sorted: " << std::endl;

        for (auto [entityId, test1]: r.view<Test1>())
        {
            std::cout << test1.value << std::endl;
        }
    }
}