//
// Created by Arjo Nagelhout on 18/01/2024.
//

#include "gtest/gtest.h"

#include "entity/entity_registry.h"

//#include <random>

using namespace entity;

// namespace is to avoid duplicate symbols
namespace sparse_set_tests
{
    struct Test1
    {
        float value = 0.0f;
    };

    TEST(SparseSet, Sort)
    {
        EntityRegistry r;

        // sort using sort function exposed to registry
        for (int i = 0; i < 100; i++)
        {
            r.createEntity(i);
            r.addComponent<Test1>(i);

            auto& component = r.getComponent<Test1>(i);
            component.value = static_cast<float>(rand() % 100);
        }

        r.sort<Test1>([&r](Entity lhs, Entity rhs){
            auto& lhsValue = r.getComponent<Test1>(lhs);
            auto& rhsValue = r.getComponent<Test1>(rhs);

            return lhsValue.value < rhsValue.value;
        });

        float lastValue = std::numeric_limits<float>::max();
        for (auto [entityId, test1]: r.view<Test1>())
        {
            ASSERT_TRUE(test1.value < lastValue);
        }
    }
}