//
// Created by Arjo Nagelhout on 02/04/2024.
//

#include <gtest/gtest.h>

#include <entity/entity_registry.h>
#include <reflection/class_info.h>
#include <reflection/serialize/json.h>

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
        ClassInfoBuilder<EntityId>("Entity").emplace();
        JsonSerializer::shared().emplace<EntityId>(entityFromJson, entityToJson);

        ClassInfoBuilder<SparseSet<EntityId>>("SparseSet<Entity>")
//            .property<&SparseSet<EntityId>::sparse>("values")
//            .property<&SparseSet<EntityId>::dense>("dense")
//            .property<&SparseSet<EntityId>::denseValues>("denseValues")
            .emplace();

        ClassInfoBuilder<EntityRegistry>("EntityRegistry")
            .property<&EntityRegistry::entities>("entities")
                //.property<&EntityRegistry::components>("components")
            .emplace();

        EntityRegistry scene;
        scene.createEntity(0);
        scene.createEntity(1);
        scene.createEntity(2);

        std::string result = JsonSerializer::shared().toJsonString(scene, 2);
        std::cout << "scene: \n" << result << std::endl;
    }
}