//
// Created by Arjo Nagelhout on 02/04/2024.
//

#include <gtest/gtest.h>

#include <entity/entity_registry.h>
#include <reflection/class.h>
#include <reflection/serialize/json.h>
#include <reflection/reflection.h>

using namespace reflection;
using namespace entity;

namespace serialize_registry_test
{
    void entityFromJson(nlohmann::json const& in, EntityId* out)
    {
        *out = in.get<EntityId>();
    }

    void entityToJson(EntityId* in, nlohmann::json& out)
    {
        out = *in;
    }

    TEST(Entity, SerializeRegistry)
    {
        Reflection& reflection = Reflection::shared();
        ClassInfoBuilder<EntityId>("Entity").emplace(reflection.types);
        reflection.json.emplace<EntityId>(entityFromJson, entityToJson);

        ClassInfoBuilder<SparseSet<EntityId>>("SparseSet<Entity>")
//            .property<&SparseSet<EntityId>::sparse>("values")
//            .property<&SparseSet<EntityId>::dense>("dense")
//            .property<&SparseSet<EntityId>::denseValues>("denseValues")
            .emplace(reflection.types);

        ClassInfoBuilder<EntityRegistry>("EntityRegistry")
            .member<&EntityRegistry::entities>("entities")
                //.property<&EntityRegistry::components>("components")
            .emplace(reflection.types);

        EntityRegistry scene;
        scene.createEntity(0);
        scene.createEntity(1);
        scene.createEntity(2);

        std::string result = reflection.json.toJsonString(scene, 2);
        std::cout << "scene: \n" << result << std::endl;
    }
}