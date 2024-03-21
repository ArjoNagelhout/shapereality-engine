//
// Created by Arjo Nagelhout on 29/12/2023.
//

#ifndef SHAPEREALITY_LOGGER_H
#define SHAPEREALITY_LOGGER_H

#include <string>
#include <fstream>
#include <vector>

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

    struct LoggerDescriptor final
    {
        unsigned int maxLogFileSizeInBytes;
        unsigned int maxLogFileCount;
    };

    constexpr LoggerDescriptor kDefaultLoggerDescriptor = {
        .maxLogFileSizeInBytes = 5 * 1024 * 1024, // 1 mebibyte = 1024 * 1024 bytes
        .maxLogFileCount = 10
    };

    // add convenience macros that can be stripped out from a release build. Maybe using constexpr?

    // log to the default shared instance
    void log(std::string const& message, Severity_ severity = Severity_Info, Verbosity verbosity = Verbosity::Debug);

    /**
     * Each time the logger gets created, it will create a new log file, or append to the last one
     */
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

        explicit Logger(
            std::filesystem::path logFilesDirectory,
            std::string logFileNamePrefix,
            LoggerDescriptor descriptor = kDefaultLoggerDescriptor,
            Target_ targetMask = Target_All,
            Severity_ severityMask = Severity_All,
            Verbosity verbosity = Verbosity::Debug);

        ~Logger();

        // shared instance
        [[nodiscard]] static Logger& shared();

        // log
        void log(
            std::string const& message,
            Severity_ severity = Severity_Info,
            Verbosity verbosity = Verbosity::Debug);

    private:
        // filtering of which messages to display / log
        Target_ targetMask;
        Severity_ severityMask;
        Verbosity verbosity;

        std::filesystem::path logFilesDirectory;
        unsigned int maxLogFileSizeInBytes;
        unsigned int maxLogFileCount;

        std::filesystem::path activeLogFilePath;
        std::string logFileNamePrefix;
        std::ofstream activeLogFile;

        //
        [[nodiscard]] unsigned int logFileCount() const;

        //
        [[nodiscard]] std::vector<std::filesystem::path> sortedLogFiles() const;

        //
        void deleteFirstLogFile() const;

        //
        [[nodiscard]] std::filesystem::path lastLogFile() const;

        //
        [[nodiscard]] std::filesystem::path newLogFilePath() const;

        // should be called periodically
        // will check if active log file has exceeded max
        // size, and if so create a new one
        // if this would increase the log file count above the max amount of log files
        // it will delete the first created log file
        void createNewLogFileIfNeeded();
    };
}

#endif //SHAPEREALITY_LOGGER_H
