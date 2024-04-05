//
// Created by Arjo Nagelhout on 02/04/2024.
//

#include <gtest/gtest.h>

#include <entity/entity_registry.h>
#include <reflection/type_info.h>
#include <reflection/serialize/json.h>

using namespace reflection;
using namespace entity;

namespace serialize_registry_test
{
    void entityFromJson(nlohmann::json const& in, Entity* out)
    {
        *out = in.get<Entity>();
    }

    void entityToJson(Entity* in, nlohmann::json& out)
    {
        out = *in;
    }

    TEST(Entity, SerializeRegistry)
    {
        TypeInfoBuilder<Entity>("Entity").emplace();
        JsonSerializer::shared().emplace<Entity>(entityFromJson, entityToJson);

        TypeInfoBuilder<SparseSet<Entity>>("SparseSet<Entity>")
            .property<&SparseSet<Entity>::sparse>("values")
            .property<&SparseSet<Entity>::dense>("dense")
            .property<&SparseSet<Entity>::denseValues>("denseValues")
            .emplace();

        TypeInfoBuilder<EntityRegistry>("EntityRegistry")
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