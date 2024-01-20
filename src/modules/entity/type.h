//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_TYPE_H
#define SHAPEREALITY_TYPE_H

#include <iostream>

#include "config.h"

namespace entity
{
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
}

#endif //SHAPEREALITY_TYPE_H
