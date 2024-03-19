//
// Created by Arjo Nagelhout on 29/12/2023.
//

#include "logger.h"

#include <iostream>

namespace common
{
    std::string_view toString(Severity_ severity)
    {
        if ((severity & Severity_Error) != 0)
        {
            return "Error";
        }
        else if ((severity & Severity_Warning) != 0)
        {
            return "Warning";
        }
        else if ((severity & Severity_Info) != 0)
        {
            return "Info";
        }
        else
        {
            return "";
        }
    }

    std::string_view toString(Verbosity verbosity)
    {
        switch (verbosity)
        {
            case Verbosity::None:
            {
                return "";
            }
            case Verbosity::Release:
            {
                return "Release";
            }
            case Verbosity::Debug:
            {
                return "Debug";
            }
        }
    }

    void log(std::string const& message, Severity_ severity, Verbosity verbosity)
    {
        Logger::shared().log(message, severity, verbosity);
    }

    Logger::Logger(Logger::Target_ targetMask_,
                   Severity_ severityMask_,
                   Verbosity verbosity_)
        : targetMask(targetMask_),
          severityMask(severityMask_),
          verbosity(verbosity_){}

    Logger::~Logger() = default;

    Logger& Logger::shared()
    {
        static Logger instance_{Target_All, Severity_All, Verbosity::Debug};
        return instance_;
    }

    void Logger::log(std::string const& message, Severity_ severity_, Verbosity verbosity_)
    {
        // logger should add the date-time stamp, severity and verbosity level:
        // [2024-03-17T22:09:00, Info, Debug] Message here

        // check if it should be output
        if ((severity_ & severityMask) == 0)
        {
            return;
        }

        if (verbosity < verbosity_)
        {
            return;
        }

        if ((targetMask & Target_Console) != 0)
        {
            std::cout << "[" << toString(verbosity_) << ", " << toString(severity_) << "] " << message << std::endl;
        }

        if ((targetMask & Target_File) != 0)
        {

        }
    }
}