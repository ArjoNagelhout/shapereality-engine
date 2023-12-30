//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_TYPE_H
#define SHAPEREALITY_TYPE_H

#include "config.h"

namespace entity
{
    namespace internal
    {
        [[nodiscard]] static type_id getNextTypeIndex()
        {
            static type_id value = 0;
            return value++; // post-increment, first return value, then increment
        }
    }

    template<typename Type>
    [[nodiscard]] static type_id getTypeIndex()
    {
        static const type_id value = internal::getNextTypeIndex();
        return value;
    }
}

#endif //SHAPEREALITY_TYPE_H
