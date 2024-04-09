//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_REGISTER_H
#define SHAPEREALITY_REGISTER_H

namespace reflection
{
    // forward declaration for reflection
    struct TypeInfoRegistry;

    struct JsonSerializer;
}

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
//          // use r and json here
//      }
// }
#define REGISTER_REFLECTION void register_(reflection::TypeInfoRegistry& types, reflection::JsonSerializer& json)

#endif //SHAPEREALITY_REGISTER_H
