//
// Created by Arjo Nagelhout on 11/01/2024.
//


#include "gtest/gtest.h"

#include "entity/registry.h"

using namespace entity;

TEST(Registry, CreateDestroyEntities)
{
    entity_type one = 1232;
    entity_type two = 15;

    Registry r;
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

}