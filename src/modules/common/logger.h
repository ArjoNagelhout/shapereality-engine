//
// Created by Arjo Nagelhout on 29/12/2023.
//

#ifndef SHAPEREALITY_LOGGER_H
#define SHAPEREALITY_LOGGER_H

#include <string>

namespace common
{
    enum Severity_ : unsigned int
    {
        Severity_None = 0,

        /**
         * Error means that there was an error with calling a function, but the engine
         * can still recover / is not impacted by the function returning an error.
         * Otherwise, use asserts.
         */
        Severity_Error = 1 << 0,
        Severity_Warning = 1 << 1,
        Severity_Info = 1 << 2,
        Severity_All = Severity_Error | Severity_Warning | Severity_Info
    };

    enum class Verbosity : unsigned int
    {
        None = 0,
        Release = 1,
        Debug = 2
    };

    // add convenience macros that can be stripped out from a release build. Maybe using constexpr?

    // log to the default shared instance
    void log(std::string const& message, Severity_ severity = Severity_Info, Verbosity verbosity = Verbosity::Debug);

    class Logger
    {
    public:
        enum Target_
        {
            Target_None = 0,
            Target_Console = 1 << 0,
            Target_File = 1 << 1,
            Target_All = Target_Console | Target_File
        };

        explicit Logger(Target_ targetMask, Severity_ severityMask, Verbosity verbosity);

        ~Logger();

        // shared instance
        [[nodiscard]] static Logger& shared();

        // log
        void log(std::string const& message,
                 Severity_ severity = Severity_Info,
                 Verbosity verbosity = Verbosity::Debug);

    private:
        Target_ targetMask;

        // filtering for logging messages
        Severity_ severityMask;
        Verbosity verbosity;
    };
}

#endif //SHAPEREALITY_LOGGER_H
