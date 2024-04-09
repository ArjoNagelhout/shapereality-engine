//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_REFLECTION_H
#define SHAPEREALITY_REFLECTION_H

#include <reflection/type_info_registry.h>
#include <reflection/serialize/json.h>
#include <reflection/serialize/binary.h>

namespace reflection
{
    /**
     * We enable the creation of the serializers and type info registry at once,
     * and make it easier to pass these three types around as a bundle.
     */
    struct Reflection
    {
        [[nodiscard]] static Reflection& shared();

        TypeInfoRegistry types;
        JsonSerializer json;
        BinarySerializer binary;

    private:
        explicit Reflection();
    };
}

// I dislike macros, but this enables easily changing the function signature
//
// usage in register.h:
// namespace module_name
// {
//      REGISTER_REFLECTION;
// }
//
// and in register.cpp
//
// namespace module_name
// {
//      REGISTER_REFLECTION
//      {
//          // use reflection.types, reflection.json and reflection.binary here
//      }
// }
#define REGISTER_REFLECTION void register_(reflection::Reflection& reflection)

#endif //SHAPEREALITY_REFLECTION_H
