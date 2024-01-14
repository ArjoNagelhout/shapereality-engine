//
// Created by Arjo Nagelhout on 13/01/2024.
//

#include "gtest/gtest.h"

#include "entity/registry.h"
#include "entity/view.h"

using namespace entity;

struct Test1
{
    float one = 1.0f;
};

struct Test2
{
    float one = 2.0f;
};

struct Test3
{
    float one = 3.0f;
};

TEST(View, Test)
{
    Registry r;

    entity_type entity1 = 0;
    entity_type entity2 = 13;
    entity_type entity3 = 83;

    r.createEntity(entity1);
    r.createEntity(entity2);
    r.createEntity(entity3);

    r.addComponent<Test1>(entity1);
    r.addComponent<Test1>(entity2);
    r.addComponent<Test1>(entity3);

    r.addComponent<Test2>(entity1);
    r.addComponent<Test2>(entity2);
    r.addComponent<Test2>(entity3);

    r.addComponent<Test3>(entity1);

    auto& t1 = r.getComponentStorage<Test1>();
    auto& t2 = r.getComponentStorage<Test2>();
    auto& t3 = r.getComponentStorage<Test3>();

    View view(&t1, &t2, &t3);
    View view2(&t1, &t2);
    View view3(&t1, &t3);

    std::cout << "view size: " << view.size() << std::endl;
    std::cout << "view2 size: " << view2.size() << std::endl;
    std::cout << "view3 size: " << view3.size() << std::endl;

    for (auto it = view2.begin(); it != view2.end(); it++)
    {
        auto value = *it;

        // structured binding for tuple:
        // https://en.cppreference.com/w/cpp/language/structured_binding
        auto [a, b] = *it;

        a.one = 523.f;
        b.one = 12.f;

        std::cout << "iterator resulting thing view 2" << std::endl;
    }

    for (auto [a, b] : view2)
    {
        
    }

    std::cout << "test" << std::endl;
}