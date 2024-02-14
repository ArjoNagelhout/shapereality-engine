//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_ID_H
#define SHAPEREALITY_TYPE_ID_H

#include <cstdint>

/**
 * @namespace reflection
 * @brief simple reflection of plain-old datastructures (POD) or compositions of PODs
 */
namespace reflection
{
    using type_id = uint32_t;

    namespace internal
    {
        struct TypeIndex final
        {
            [[nodiscard]] static type_id getNextTypeIndex()
            {
                static type_id value = 0;
                return value++; // post-increment, first return value, then increment
            }
        };
    }

    template<typename Type>
    struct TypeIndex
    {
        [[nodiscard]] static type_id value() noexcept {
            static const type_id value = internal::TypeIndex::getNextTypeIndex();
            return value;
        }
    };

    template<typename Type>
    [[nodiscard]] bool isType(type_id id)
    {
        return id == TypeIndex<Type>::value();
    }
}

#endif //SHAPEREALITY_TYPE_ID_H
