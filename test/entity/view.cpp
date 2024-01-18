//
// Created by Arjo Nagelhout on 13/01/2024.
//

#include "gtest/gtest.h"

#include "entity/registry.h"
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

entity_type entity1 = 0;
entity_type entity2 = 13;
entity_type entity3 = 83;
entity_type entity4 = 100;
entity_type entity5 = 101;
entity_type entity6 = 102;
entity_type entity7 = 105;

TEST(View, Test)
{
    Registry r;

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