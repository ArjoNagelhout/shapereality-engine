//
// Created by Arjo Nagelhout on 11/01/2024.
//


#include "gtest/gtest.h"

#include "entity/entity_registry.h"

using namespace entity;

TEST(Registry, CreateDestroyEntities)
{
    EntityId one = 1232;
    EntityId two = 15;

    EntityRegistry r;
    ASSERT_FALSE(r.entityExists(one));

    r.createEntity(one);
    r.createEntity(two);
    ASSERT_TRUE(r.entityExists(one));
    ASSERT_TRUE(r.entityExists(two));
    ASSERT_EQ(r.entityCount(), 2);
    ASSERT_EQ(r.size(), one + 1);

    r.destroyEntity(one);
    ASSERT_FALSE(r.entityExists(one));
    ASSERT_EQ(r.entityCount(), 1);

    r.destroyEntity(two);
    ASSERT_EQ(r.entityCount(), 0);
}

TEST(Registry, AddRemoveComponents)
{
    EntityId entity = 0;

    struct SimpleComponent
    {
        float first = 0.5f;
        float second = 1.0f;
        float third = 1.5f;
    };

    EntityRegistry r;

    bool success;

    // can't add component if entity does not exist
    success = r.addComponent<SimpleComponent>(entity);
    ASSERT_FALSE(success);

    // can't remove if entity does not exist
    success = r.removeComponent<SimpleComponent>(entity);
    ASSERT_FALSE(success);

    r.createEntity(0);

    // can't remove if component does not exist
    success = r.removeComponent<SimpleComponent>(entity);
    ASSERT_FALSE(success);

    // can add if entity exists
    success = r.addComponent<SimpleComponent>(entity);
    ASSERT_TRUE(success);

    // can't add if component already exists
    success = r.addComponent<SimpleComponent>(entity);
    ASSERT_FALSE(success);
}

TEST(Registry, Clear)
{
    EntityRegistry r;
    for (int i = 0; i < 12; i++)
    {
        r.createEntity(i);
    }

    ASSERT_EQ(r.entityCount(), 12);

    r.clear();

    ASSERT_EQ(r.entityCount(), 0);
}

TEST(Registry, Sort)
{
    EntityRegistry r;

}