//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_ID_H
#define SHAPEREALITY_TYPE_ID_H

#include <cstdint>

namespace reflection
{
    using TypeId = uint32_t;

    namespace internal
    {
        struct TypeIndex final
        {
            [[nodiscard]] static TypeId getNextTypeId()
            {
                static TypeId value = 1;
                return value++; // post-increment, first return value, then increment
            }
        };
    }

    constexpr TypeId kNullTypeId = 0;

    template<typename Type>
    struct TypeIndex final
    {
        [[nodiscard]] static TypeId value() noexcept {
            static const TypeId value = internal::TypeIndex::getNextTypeId();
            return value;
        }
    };

    template<typename Type>
    [[nodiscard]] bool isType(TypeId id)
    {
        return id == TypeIndex<Type>::value();
    }
}

#endif //SHAPEREALITY_TYPE_ID_H
