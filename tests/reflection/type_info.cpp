//
// Created by Arjo Nagelhout on 06/02/2024.
//

#include <reflection/type_info.h>
#include <gtest/gtest.h>

#include <queue>

using namespace reflection;

namespace type_info_tests
{
    struct Child
    {
        float x = 1.0f;
        float y = 1.3f;
        float z = 1.6f;
    };

    struct Parent1
    {
        Child c1;
        Child c2;
        Child c3;
        float w = 5.0f;
    };

    struct Parent2
    {
        Parent1 p1;
        Parent1 p2;
        float w = 4.0f;
    };

    struct Parent3
    {
        Parent2 p1;
        Parent2 p2;
        Parent1 p3;
        Parent1 p4;
        Child c1;
        Child c2;
        Child c3;
        float w = 2.0f;
    };

    void setup(TypeInfoRegistry& r)
    {
        TypeInfoBuilder<Child>("Child")
            .addProperty<&Child::x>("x")
            .addProperty<&Child::y>("y")
            .addProperty<&Child::z>("z")
            .registerType(r);

        TypeInfoBuilder<Parent1>("Parent1")
            .addProperty<&Parent1::c1>("c1")
            .addProperty<&Parent1::c2>("c2")
            .addProperty<&Parent1::c3>("c3")
            .registerType(r);

        TypeInfoBuilder<Parent2>("Parent2")
            .addProperty<&Parent2::p1>("p1")
            .addProperty<&Parent2::p2>("p2")
            .addProperty<&Parent2::w>("w")
            .registerType(r);

        TypeInfoBuilder<Parent3>("Parent3")
            .addProperty<&Parent3::p1>("p1")
            .addProperty<&Parent3::p2>("p2")
            .addProperty<&Parent3::p3>("p3")
            .addProperty<&Parent3::p4>("p4")
            .addProperty<&Parent3::c1>("c1")
            .addProperty<&Parent3::c2>("c2")
            .addProperty<&Parent3::c3>("c3")
            .addProperty<&Parent3::w>("w")
            .registerType(r);
    }

    TEST(Reflection, Recurse)
    {
        TypeInfoRegistry r;
        setup(r);

        struct Node
        {
            std::string name;
            TypeInfo* typeInfo;
        };

        Node root = Node{
            .name = "root",
            .typeInfo = r.get<Parent3>()
        };

        // breadth first search
        std::queue<Node> queue;

        queue.push(root);

        while (!queue.empty())
        {
            Node current = queue.front();

            std::cout << current.name
                      << " ("
                      << (current.typeInfo ? current.typeInfo->name : "Unregistered type")
                      << ")"
                      << std::endl;

            queue.pop();

            if (!current.typeInfo)
            {
                continue;
            }

            for (auto& property: current.typeInfo->properties)
            {
                if (r.isPrimitive(property.type))
                {

                }
                else
                {
                    TypeInfo* propertyInfo = r.get(property.type);

                    queue.emplace(Node{
                        .name = property.name,
                        .typeInfo = propertyInfo
                    });
                }
            }
        }
    }
}