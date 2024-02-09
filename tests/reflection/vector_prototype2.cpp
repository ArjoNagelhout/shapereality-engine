//
// Created by Arjo Nagelhout on 09/02/2024.
//

#include <gtest/gtest.h>
#include <reflection/type_info.h>

namespace vector_prototype2
{
    // we want to serialize and deserialize vectors.
    // a vector should not have to be separately registered, just as a property
    // registering the property gives us the type std::vector<certain type>, so
    // can write some template that registers that it is a vector<>
    // let's first test if we can detect it being a vector
    
}