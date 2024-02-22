//
// Created by Arjo Nagelhout on 29/12/2023.
//

#ifndef SHAPEREALITY_LOG_H
#define SHAPEREALITY_LOG_H

#include <string>

namespace common
{
    enum class Severity
    {
        Info,
        Warning,
        Error
    };

    enum class Verbosity
    {
        Debug,
        Release
    };

    void log(std::string const& message,
             Severity severity = Severity::Info,
             Verbosity verbosity = Verbosity::Debug);
}

#endif //SHAPEREALITY_LOG_H
