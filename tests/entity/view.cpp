//
// Created by Arjo Nagelhout on 13/01/2024.
//

#include "gtest/gtest.h"

#include "entity/entity_registry.h"
#include "entity/view.h"

using namespace entity;

struct Component1
{
    int value = 0;
};

struct Component2
{
    int value2 = 0;
};

struct Component3
{
    int value3 = 0;
};

Entity entity1 = 0;
Entity entity2 = 13;
Entity entity3 = 83;
Entity entity4 = 100;
Entity entity5 = 101;
Entity entity6 = 102;
Entity entity7 = 105;

TEST(View, Test)
{
    EntityRegistry r;

    r.createEntity(entity1);
    r.createEntity(entity2);
    r.createEntity(entity3);
    r.createEntity(entity4);
    r.createEntity(entity5);
    r.createEntity(entity6);
    r.createEntity(entity7);

    r.addComponent<Component1>(entity1);
    r.addComponent<Component1>(entity2);
    r.addComponent<Component1>(entity3);
    r.addComponent<Component1>(entity4);
    r.addComponent<Component1>(entity5);
    r.addComponent<Component1>(entity6);
    r.addComponent<Component1>(entity7);

    r.addComponent<Component2>(entity1);
    r.addComponent<Component2>(entity2);
    r.addComponent<Component2>(entity3);
    r.addComponent<Component2>(entity4);
    r.addComponent<Component2>(entity5);
    r.addComponent<Component2>(entity6);
    r.addComponent<Component1>(entity7);

    r.addComponent<Component3>(entity1);
    r.addComponent<Component3>(entity2);
    r.addComponent<Component3>(entity3);
    r.addComponent<Component3>(entity4);
    r.addComponent<Component3>(entity5);
    r.addComponent<Component3>(entity6);
    r.addComponent<Component1>(entity7);
}