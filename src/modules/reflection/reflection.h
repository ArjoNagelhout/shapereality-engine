//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_REFLECTION_H
#define SHAPEREALITY_REFLECTION_H

#include <reflection/type_registry.h>
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

        TypeRegistry types;
        JsonSerializer json;
        BinarySerializer binary;

    private:
        explicit Reflection();
    };
}

#endif //SHAPEREALITY_REFLECTION_H
