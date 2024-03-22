//
// Created by Arjo Nagelhout on 29/12/2023.
//

#ifndef SHAPEREALITY_LOGGER_H
#define SHAPEREALITY_LOGGER_H

#include <string>
#include <fstream>
#include <vector>
#include <mutex>
#include <fmt/format.h>

namespace common::log
{
    enum Severity_ : unsigned int
    {
        /**
         * Don't use when logging, only for masking which severities to output
         */
        Severity_None = 0,

        /**
         * For the logger itself to log things like "begin logging, end logging"
         */
        Severity_LoggerInfo = 1 << 0,

        /**
         * Unrecoverable crash
         */
        Severity_Critical = 1 << 1,

        /**
         * Error means that there was an error with calling a function, but the engine
         * can still recover / is not impacted by the function returning an error.
         * Otherwise, use Severity_Critical
         */
        Severity_Error = 1 << 2,

        /**
         *
         */
        Severity_Warning = 1 << 3,

        /**
         *
         */
        Severity_Info = 1 << 4,

        /**
         * Don't use when logging, only for masking which severities to output
         */
        Severity_All = Severity_LoggerInfo | Severity_Critical | Severity_Error | Severity_Warning | Severity_Info
    };

    enum class Verbosity : unsigned int
    {
        None = 0,
        Release = 1,
        Debug = 2
    };

    struct LoggerDescriptor final
    {
        std::string loggerName = "Default Logger";
        std::string logFileNamePrefix = "log_";
        unsigned int maxLogFileSizeInBytes = 5 * 1024 * 1024; // 1 mebibyte = 1024 * 1024 bytes
        unsigned int maxLogFileCount = 10;
        unsigned int checkCreateNewLogFileInterval = 100; // after how many times we should check if we need to create a new logging file if it exceeds the log file size
        unsigned int flushInterval = 10;
        bool threadSafe = true; // uses a mutex for logging from different threads. could be more intelligent like using a lock-free queue or channels, but this works.
    };

    /**
     * Each time the logger gets created, it will create a new log file, or append to the last one
     */
    class Logger final
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
            LoggerDescriptor descriptor = {},
            Target_ targetMask = Target_All,
            Severity_ severityMask = Severity_All,
            Verbosity verbosity = Verbosity::Debug);

        ~Logger();

        // shared instance
        [[nodiscard]] static Logger& shared();

        template<typename... Args>
        void errorDebug(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Error, Verbosity::Debug, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void warningDebug(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Warning, Verbosity::Debug, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void infoDebug(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Info, Verbosity::Debug, std::forward<Args>(args)...);
        }

        // critical is always included in release, will terminate the program
        template<typename... Args>
        void critical(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Critical, Verbosity::Release, std::forward<Args>(args)...);
            exit(EXIT_FAILURE);
        }

        // default verbosity: release

        template<typename... Args>
        void error(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Error, Verbosity::Release, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void warning(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Warning, Verbosity::Release, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void info(fmt::format_string<Args...> fmt, Args&& ...args)
        {
            log(fmt, Severity_Info, Verbosity::Release, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void log(fmt::format_string<Args...> fmt, Severity_ severity_, Verbosity verbosity_, Args&& ...args)
        {
            log(fmt::format(fmt, std::forward<Args>(args)...), severity_, verbosity_);
        }

        // log
        void log(
            std::string const& message,
            Severity_ severity = Severity_Info,
            Verbosity verbosity = Verbosity::Debug);

    private:
        std::filesystem::path logFilesDirectory;
        LoggerDescriptor descriptor;

        // filtering of which messages to log
        Target_ targetMask;
        Severity_ severityMask;
        Verbosity verbosity;

        unsigned int timeToCheckCreateNewLogFile; // counts down from interval defined in descriptor and then performs check
        unsigned int timeToFlush; // counts down from interval defined in descriptor and then flushes file and console output

        std::filesystem::path activeLogFilePath;
        std::ofstream activeLogFile;
        std::mutex logMutex;

        //
        [[nodiscard]] unsigned int logFileCount() const;

        //
        [[nodiscard]] std::vector<std::filesystem::path> sortedLogFiles() const;

        //
        void deleteOldLogFilesIfNeeded() const;

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

    template<typename... Args>
    void errorDebug(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().errorDebug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warningDebug(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().warningDebug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void infoDebug(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().infoDebug(fmt, std::forward<Args>(args)...);
    }

    // critical is always included in release
    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().critical(fmt, std::forward<Args>(args)...);
    }

    // default verbosity: release

    template<typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().warning(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&& ...args)
    {
        Logger::shared().info(fmt, std::forward<Args>(args)...);
    }
}

#endif //SHAPEREALITY_LOGGER_H
