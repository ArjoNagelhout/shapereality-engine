//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_SERIALIZER_H
#define SHAPEREALITY_SERIALIZER_H

#include <reflection/type_info.h>

namespace reflection
{
    // SerializationType = the type to serialize to or from
    template<typename SerializationType>
    class Serializer final
    {
    public:
        struct Functions
        {
            std::function<void(SerializationType const&, std::any)> from;
            std::function<void(std::any const&, SerializationType&)> to;
        };

        template<typename Type>
        void emplace(Functions&& f)
        {
            type_id typeId = TypeIndex<Type>::value();
            assert(!functions.contains(typeId) && "already registered functions for type");
            functions.emplace(typeId, f);
        }

        [[nodiscard]] bool from(type_id typeId, SerializationType const& in, std::any out);

        [[nodiscard]] bool to(type_id typeId, std::any const& in, SerializationType& out);

    private:
        std::unordered_map<type_id, Functions> functions;
    };
}

#endif //SHAPEREALITY_SERIALIZER_H
