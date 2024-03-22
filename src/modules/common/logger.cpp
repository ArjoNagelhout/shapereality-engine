//
// Created by Arjo Nagelhout on 29/12/2023.
//

#include "logger.h"

#include <iostream>

#include <chrono>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <thread>
#include <optional>

#include <common/application_info.h>

namespace common::log
{
    [[nodiscard]] std::string_view toString(Severity_ severity)
    {
        //@formatter:off
        if ((severity & Severity_LoggerInfo) != 0) { return "LoggerInfo"; }
        else if ((severity & Severity_Critical) != 0) { return "Critical"; }
        else if ((severity & Severity_Error) != 0) { return "Error"; }
        else if ((severity & Severity_Warning) != 0) { return "Warning"; }
        else if ((severity & Severity_Info) != 0) { return "Info"; }
        else { return ""; }
        //@formatter:on
    }

    [[nodiscard]] std::string_view toString(Verbosity verbosity)
    {
        switch (verbosity)
        {
            case Verbosity::None: return "";
            case Verbosity::Release: return "Release";
            case Verbosity::Debug: return "Debug";
        }
    }

    bool isLogFile(std::filesystem::path const& path)
    {
        if (!std::filesystem::exists(path)) // should exist
        {
            return false;
        }

        if (!std::filesystem::is_regular_file(path)) // should be a file, not a directory
        {
            return false;
        }

        if (path.filename().string().starts_with('.')) // should not start with a dot, those are system files
        {
            return false;
        }

        if (path.extension().string() != kLogFileExtension) // should have the log file extension as its extension
        {
            return false;
        }

        return true;
    }

    Logger::Logger(
        std::filesystem::path logFilesDirectory_,
        LoggerDescriptor descriptor_,
        Target_ targetMask_,
        Severity_ severityMask_,
        Verbosity verbosity_) :
        logFilesDirectory(std::move(logFilesDirectory_)),
        descriptor(std::move(descriptor_)),
        targetMask(targetMask_),
        severityMask(severityMask_),
        verbosity(verbosity_)
    {
        // create logging directory if it does not exist
        std::filesystem::create_directories(common::ApplicationInfo::loggingDirectory());

        if ((targetMask & Target_File) != 0)
        {
            createNewLogFileIfNeeded();
        }

        timeToCheckCreateNewLogFile = descriptor.checkCreateNewLogFileInterval; // important, otherwise the -- operator will go from 0 to the unsigned int max(), thus resulting in the action never getting triggered
        timeToFlush = descriptor.flushInterval;

        log(fmt::format("\n------------------- Logging started ({}) --------------------\n\n", descriptor.loggerName),
            Severity_LoggerInfo, Verbosity::Debug);
    }

    Logger::~Logger()
    {
        // lock if thread safe
        std::optional<std::unique_lock<std::mutex>> lock;
        if (descriptor.threadSafe)
        {
            lock.emplace(logMutex);
        }

        std::cout.flush();

        if (activeLogFile.is_open())
        {
            activeLogFile.close();
        }
    }

    Logger& Logger::shared()
    {
        static Logger instance_{common::ApplicationInfo::loggingDirectory()};
        return instance_;
    }

    void Logger::log(std::string const& message, Severity_ severity_, Verbosity verbosity_)
    {
        std::string output;

        if (severity_ == Severity_LoggerInfo)
        {
            output = message;
        }
        else
        {
            output = fmt::format(
                "[{:%Y-%m-%dT%H:%M:%SZ}][{}][{}][thread {}] {}\n",
                std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()),
                toString(verbosity_),
                toString(severity_),
                std::hash<std::thread::id>()(std::this_thread::get_id()),
                message);
        }

        // check if it should be output
        if ((severity_ & severityMask) == 0)
        {
            return;
        }

        if (verbosity < verbosity_)
        {
            return;
        }

        // lock if thread safe
        std::optional<std::unique_lock<std::mutex>> lock;
        if (descriptor.threadSafe)
        {
            lock.emplace(logMutex);
        }

        bool flush;
        timeToFlush--;
        if (timeToFlush == 0)
        {
            flush = true;
            timeToFlush = descriptor.flushInterval;
        }

        if ((targetMask & Target_Console) != 0)
        {
            std::cout << output;
            if (flush)
            {
                std::cout.flush();
            }
        }

        if ((targetMask & Target_File) != 0)
        {
            activeLogFile << output;
            if (flush)
            {
                activeLogFile.flush();
            }

            // check in an interval if we need to create a new file
            timeToCheckCreateNewLogFile--;
            if (timeToCheckCreateNewLogFile == 0)
            {
                createNewLogFileIfNeeded();
                timeToCheckCreateNewLogFile = descriptor.checkCreateNewLogFileInterval;
            }
        }
    }

    unsigned int Logger::logFileCount() const
    {
        unsigned int count = 0;
        for (auto const& entry: std::filesystem::directory_iterator(logFilesDirectory))
        {
            if (!isLogFile(entry))
            {
                continue;
            }
            count++;
        }
        return count;
    }


    void Logger::deleteOldLogFilesIfNeeded() const
    {
        if (logFileCount() <= 1)
        {
            return; // no need to delete anything
        }
        std::vector<std::filesystem::path> const logFiles = sortedLogFiles();

        int amountToDelete = static_cast<int>(logFiles.size()) - static_cast<int>(descriptor.maxLogFileCount);
        if (amountToDelete <= 0)
        {
            return;
        }

        for (size_t i = 0; i < amountToDelete; i++)
        {
            std::filesystem::remove(logFiles[i]);
        }
    }

    std::vector<std::filesystem::path> Logger::sortedLogFiles() const
    {
        // sort based on name / last modified date
        std::vector<std::filesystem::path> paths;
        for (auto const& entry: std::filesystem::directory_iterator(logFilesDirectory))
        {
            if (!isLogFile(entry))
            {
                continue;
            }
            paths.emplace_back(entry);
        }

        // date modified comparison lambda
        auto compare = [](std::filesystem::path const& lhs, std::filesystem::path const& rhs) -> bool {
            // we can assume files exist
            return std::filesystem::last_write_time(lhs) < std::filesystem::last_write_time(rhs);
        };

        std::sort(paths.begin(), paths.end(), compare);
        return paths;
    }

    std::filesystem::path Logger::lastLogFile() const
    {
        if (logFileCount() == 0)
        {
            return {};
        }
        std::vector<std::filesystem::path> const logFiles = sortedLogFiles();
        return logFiles.back();
    }

    std::filesystem::path Logger::newLogFilePath() const
    {
        std::filesystem::path base = logFilesDirectory / fmt::format(
            "{}{:%Y-%m-%dT%H}",//"{}{:%Y-%m-%dT%H-%M-%SZ}",
            descriptor.logFileNamePrefix,
            std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));

        // if it already exists, try to add a postfix _n (_0, _1, etc.)
        unsigned int postfix = 1;

        std::filesystem::path target;
        do
        {
            target = base;
            target += fmt::format("_{}{}", postfix, kLogFileExtension);
            postfix++;
        }
        while (std::filesystem::exists(target));

        return target;
    }

    void Logger::createNewLogFileIfNeeded()
    {
        if (activeLogFilePath.empty())
        {
            // first try to get the last log file
            std::filesystem::path logFilePath = lastLogFile();
            if (logFilePath.empty())
            {
                // otherwise, get new log file path
                logFilePath = newLogFilePath();
            }
            activeLogFilePath = logFilePath;
        }

        // we need to close the logging file so that the file size is accurate
        if (activeLogFile.is_open())
        {
            activeLogFile.close();
        }

        // if the log file exists, check if it exceeds the max log file size
        if (std::filesystem::exists(activeLogFilePath))
        {
            unsigned int size = std::filesystem::file_size(activeLogFilePath);
            if (size > descriptor.maxLogFileSizeInBytes)
            {
                // we need to create a new log file
                activeLogFilePath = newLogFilePath();
            }
        }

        // open the file stream, app = append, default behaviour is to overwrite.
        activeLogFile.open(activeLogFilePath, std::ios_base::app);

        deleteOldLogFilesIfNeeded();
    }
}