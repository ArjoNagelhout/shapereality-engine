//
// Created by Arjo Nagelhout on 29/12/2023.
//

#include "log.h"

#include <iostream>

namespace common
{
    void log(std::string const& message,
             Severity severity,
             Verbosity verbosity)
    {
        std::cout << message << std::endl;
    }
}