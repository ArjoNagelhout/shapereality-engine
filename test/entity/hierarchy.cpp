//
// Created by Arjo Nagelhout on 11/01/2024.
//

#include "gtest/gtest.h"

#include "entity/registry.h"
#include "entity/components/hierarchy.h"

using namespace entity;

TEST(Hierarchy, IsChildOf)
{
    Registry r;
//    r.addComponent<>()
}

TEST(Hierarchy, IsParentOf)
{

}

// validate whether the hierarchyCount gets calculated properly
TEST(Hierarchy, HierarchyCount)
{

}

// validate whether the childCount gets calculated properly
TEST(Hierarchy, ChildCount)
{

}

// edge cases:
// child index == TOMBSTONE should clear parent
// child index > size() should be out of range
//
TEST(Hierarchy, SetParent)
{

}
