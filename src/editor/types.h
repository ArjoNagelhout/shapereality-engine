//
// Created by Arjo Nagelhout on 07/02/2024.
//

#ifndef SHAPEREALITY_TYPES_H
#define SHAPEREALITY_TYPES_H

#include <reflection/type_info.h>

using namespace reflection;

namespace editor
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

    void registerTypes(TypeInfoRegistry& r);

    void render(TypeInfoRegistry& r, Parent3& value);

    [[nodiscard]] std::string toJson(TypeInfoRegistry& r, Parent3& value);

    [[nodiscard]] Parent3 fromJson(TypeInfoRegistry& r, std::string const& json);
}

#endif //SHAPEREALITY_TYPES_H
