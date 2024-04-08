//
// Created by Arjo Nagelhout on 08/04/2024.
//

#include "class.h"

namespace reflection
{
    ClassInfo::ClassInfo(std::string name_) : TypeInfo(std::move(name_), Type::Class) {}
}