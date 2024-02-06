//
// Created by Arjo Nagelhout on 06/02/2024.
//

#include <reflection/type_info.h>
#include <gtest/gtest.h>

#include <queue>
#include <stack>

using namespace reflection;

namespace type_info_tests
{
    struct Child
    {
        float x = 1.0f;
        float y = 1.3f;
        float z = 1.6f;
    };

    struct Child2
    {
        double something = 1235.5;
        std::string yourValue = "wowzers";
        std::string weValue = "your work";
        bool veryImportant = false;
    };

    struct Parent1
    {
        Child c1;
        Child2 c2;
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
        std::string something;
        double aValue = 13.3;
        bool someValue = true;
        int myValue = 1234;
        float ourValue = 1.222222f;
        Child c1;
        Child c2;
        Child c3;
        float w = 2.0f;
    };

    void setup(TypeInfoRegistry& r)
    {
        r.registerPrimitive<float>(PrimitiveInfo{
            .name = "float"
        });

        r.registerPrimitive<int>(PrimitiveInfo{
            .name = "int"
        });

        r.registerPrimitive<bool>(PrimitiveInfo{
            .name = "bool"
        });

        r.registerPrimitive<double>(PrimitiveInfo{
            .name = "double"
        });

        r.registerPrimitive<std::string>(PrimitiveInfo{
            .name = "string"
        });

        TypeInfoBuilder<Child>("Child")
            .addProperty<&Child::x>("x")
            .addProperty<&Child::y>("y")
            .addProperty<&Child::z>("z")
            .registerType(r);

        TypeInfoBuilder<Child2>("Child2")
            .addProperty<&Child2::something>("something")
            .addProperty<&Child2::yourValue>("yourValue")
            .addProperty<&Child2::weValue>("weValue")
            .addProperty<&Child2::veryImportant>("veryImportant")
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
            .addProperty<&Parent3::something>("something")
            .addProperty<&Parent3::aValue>("aValue")
            .addProperty<&Parent3::someValue>("someValue")
            .addProperty<&Parent3::myValue>("myValue")
            .addProperty<&Parent3::ourValue>("ourValue")
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
            .typeInfo = r.getTypeInfo<Parent3>()
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
                if (r.isPrimitive(property.typeId))
                {

                }
                else
                {
                    TypeInfo* propertyInfo = r.getTypeInfo(property.typeId);

                    queue.emplace(Node{
                        .name = property.name,
                        .typeInfo = propertyInfo
                    });
                }
            }
        }
    }

    size_t currentDepth = 0;

    void beginTreeNode()
    {
        currentDepth++;
    }

    void endTreeNode()
    {
        currentDepth--;
    }

    TEST(Reflection, RecurseWithStack)
    {
        TypeInfoRegistry r;
        setup(r);

        TypeInfo* root = r.getTypeInfo<Parent3>();

        struct StackFrame
        {
            std::string name;
            TypeInfo* typeInfo;
            std::any value; // pointer to value
            size_t index = 0;
        };

        std::stack<StackFrame> stack;
        stack.emplace(StackFrame{.name = "root", .typeInfo = root});

        while (!stack.empty())
        {
            StackFrame& top = stack.top();

            if (top.index == 0)
            {
                beginTreeNode();
                std::cout << std::string(4 * currentDepth, ' ')
                          << top.name
                          << " ("
                          << (top.typeInfo ? top.typeInfo->name : "Unregistered type")
                          << ")"
                          << std::endl;
            }

            if (top.typeInfo && top.index < top.typeInfo->properties.size())
            {
                PropertyInfo& property = top.typeInfo->properties[top.index];
                stack.emplace(StackFrame{
                    .name = property.name,
                    .typeInfo = r.getTypeInfo(property.typeId)
                });
                top.index++;
            }
            else
            {
                endTreeNode();
                stack.pop();
            }
        }
    }

    struct SimpleData
    {
        float value1;
        int value2;
        bool value3;
    };

    TEST(Reflection, GetSet)
    {
        SimpleData d1{
            .value1 = 3.0f,
            .value2 = 32,
            .value3 = true,
        };

        SimpleData d2{
            .value1 = 123.0f,
            .value2 = 10,
            .value3 = false
        };

        TypeInfoRegistry r;

        TypeInfoBuilder<SimpleData>("SimpleData")
            .addProperty<&SimpleData::value1>("value1")
            .addProperty<&SimpleData::value2>("value2")
            .addProperty<&SimpleData::value3>("value3")
            .registerType(r);

        TypeInfo* info = r.getTypeInfo<SimpleData>();
        auto& prop = info->properties[0];

        auto d1_value1 = prop.get<float>(d1);
        auto d2_value1 = prop.get<float>(d2);

        std::cout << "d_value1: " << d1_value1 << std::endl;
        std::cout << "d2_value1: " << d2_value1 << std::endl;

        prop.set(d1, 100.0f);
        prop.set(d2, 3252.0f);

        d1_value1 = prop.get<float>(d1);
        d2_value1 = prop.get<float>(d2);

        std::cout << "d_value1: " << d1_value1 << std::endl;
        std::cout << "d2_value1: " << d2_value1 << std::endl;
    }

    TEST(Reflection, RecurseWithStack2)
    {
        TypeInfoRegistry r;
        setup(r);

        TypeInfo* root = r.getTypeInfo<Parent3>();

        struct StackFrame
        {
            std::string name;
            type_id typeId;
            bool isPrimitive; // cached value to avoid calling isPrimitive multiple times
            TypeInfo* typeInfo; // cached value to avoid calling TypeInfoRegistry::get(type_id) multiple times
            size_t index = 0;
        };

        std::stack<StackFrame> stack;
        stack.emplace(StackFrame{.name = "root", .typeId = TypeIndex<Parent3>::value()});

        while (!stack.empty())
        {
            StackFrame& top = stack.top();

            if (top.isPrimitive)
            {
                PrimitiveInfo* primitiveInfo = r.getPrimitiveInfo(top.typeId);

                std::cout << std::string(4 * (currentDepth + 1), ' ')
                          << top.name
                          << " ("
                          << primitiveInfo->name
                          << ")"
                          << std::endl;

                // render primitive
                if (top.typeId == TypeIndex<float>::value())
                {
                    //std::cout << "let's render a float thing" << std::endl;
                }
                else
                {
                    //std::cout << "poor thing, it doesn't know" << std::endl;
                }

                stack.pop();
            }
            else
            {
                // iterate over properties and render header
                if (top.index == 0)
                {
                    beginTreeNode();
                    top.typeInfo = r.getTypeInfo(top.typeId);
                    std::cout << std::string(4 * currentDepth, ' ')
                              << top.name
                              << " ("
                              << (top.typeInfo ? top.typeInfo->name : "Unregistered type")
                              << ")"
                              << std::endl;
                }

                if (top.typeInfo && top.index < top.typeInfo->properties.size())
                {
                    PropertyInfo& property = top.typeInfo->properties[top.index];
                    stack.emplace(StackFrame{
                        .name = property.name,
                        .typeId = property.typeId,
                        .isPrimitive = r.isPrimitive(property.typeId)
                    });
                    top.index++;
                }
                else
                {
                    endTreeNode();
                    stack.pop();
                }
            }
        }
    }
}