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
    float two = 2.0f;
};

struct Test3
{
    float three = 3.0f;
};

TEST(View, Test)
{
    Registry r;

    r.createEntity(0);
    r.createEntity(1);
    r.createEntity(2);

    r.addComponent<Test1>(0);
    r.addComponent<Test1>(1);
    r.addComponent<Test1>(2);

    r.addComponent<Test2>(0);
    r.addComponent<Test2>(1);

    r.addComponent<Test3>(0);

    auto& t1 = r.getComponentStorage<Test1>();
    auto& t2 = r.getComponentStorage<Test2>();
    auto& t3 = r.getComponentStorage<Test3>();

    View view(t1, t2, t3);
    View view2(t3, t2);
}