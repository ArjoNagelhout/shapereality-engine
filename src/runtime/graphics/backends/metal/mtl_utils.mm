//
// Created by Arjo Nagelhout on 14/12/2023.
//

#include "mtl_utils.h"

#include <iostream>

namespace graphics
{
    void checkMetalError(NSError* error, std::string const& messageIfFailed)
    {
        if (error == nullptr)
        {
            return; // success, no error
        }
        std::cout << messageIfFailed;
        if (error.debugDescription != nullptr)
        {
            std::cout << "(" << error.debugDescription << ")";
        }
        std::cout << std::endl;
        exit(1);
    }
}