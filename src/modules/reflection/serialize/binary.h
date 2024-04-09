//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_BINARY_H
#define SHAPEREALITY_BINARY_H

#include <reflection/type_info.h>
#include <cassert>

namespace reflection
{
    // serializes the type_info into binary
    // see json.h for how to implement
    class BinarySerializer final
    {
    public:
        // delete copy constructor and assignment operator
        BinarySerializer(BinarySerializer const&) = delete;

        BinarySerializer& operator=(BinarySerializer const&) = delete;

        struct Functions
        {
            std::function<void(void* const&, std::any)> from;
            std::function<void(std::any const&, void*&)> to;
        };

        template<typename Type>
        void emplace(Functions&& f)
        {
            TypeId typeId = TypeIndex<Type>::value();
            assert(!functions.contains(typeId) && "already registered functions for type");
            functions.emplace(typeId, f);
        }

    private:
        TypeInfoRegistry& r;
        std::unordered_map<TypeId, Functions> functions;

        explicit BinarySerializer(TypeInfoRegistry& r);

        friend class Reflection;
    };
}

#endif //SHAPEREALITY_BINARY_H
