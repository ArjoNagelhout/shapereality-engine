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
        struct type_index final
        {
            [[nodiscard]] static type_id getNextTypeIndex()
            {
                static type_id value = 0;
                return value++; // post-increment, first return value, then increment
            }
        };
    }

    template<typename Type>
    struct type_index
    {
        [[nodiscard]] static type_id value() noexcept {
            static const type_id value = internal::type_index::getNextTypeIndex();
            return value;
        }

        [[nodiscard]] constexpr operator type_id() const noexcept {
            return value();
        }
    };
}

#endif //SHAPEREALITY_TYPE_H
