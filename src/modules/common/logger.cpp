//
// Created by Arjo Nagelhout on 29/12/2023.
//

#include "logger.h"

#include <iostream>

#include <chrono>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <thread>

#include <common/application_info.h>

namespace common::log
{
    [[nodiscard]] std::string_view toString(Severity_ severity)
    {
        //@formatter:off
        if ((severity & Severity_Error) != 0) { return "Error"; }
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

    void log(std::string const& message, Severity_ severity, Verbosity verbosity)
    {
        Logger::shared().log(message, severity, verbosity);
    }

    Logger::Logger(
        std::filesystem::path logFilesDirectory_,
        LoggerDescriptor const& descriptor_,
        Target_ targetMask_,
        Severity_ severityMask_,
        Verbosity verbosity_) :
        logFilesDirectory(std::move(logFilesDirectory_)),
        logFileNamePrefix(descriptor_.logFileNamePrefix),
        maxLogFileSizeInBytes(descriptor_.maxLogFileSizeInBytes),
        maxLogFileCount(descriptor_.maxLogFileCount),
        targetMask(targetMask_),
        severityMask(severityMask_),
        verbosity(verbosity_)
    {
        // create logging directory if it does not exist
        std::filesystem::create_directories(common::ApplicationInfo::loggingDirectory());

        if ((targetMask & Target_File) != 0)
        {
            // open new logging file, or append to last one if it isn't large enough
            // we need to do log rotation
            // separate file for each run, periodically check file size of all files
        }

        createNewLogFileIfNeeded();
    }

    Logger::~Logger()
    {
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
        std::string output = fmt::format(
            "[{:%Y-%m-%dT%H:%M:%SZ}][{}][{}][thread {}] {}\n",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()),
            toString(verbosity_),
            toString(severity_),
            std::hash<std::thread::id>()(std::this_thread::get_id()),
            message);

        // check if it should be output
        if ((severity_ & severityMask) == 0)
        {
            return;
        }

        if (verbosity < verbosity_)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(logMutex);

        if ((targetMask & Target_Console) != 0)
        {
            std::cout << output;
            std::cout.flush();
        }

        if ((targetMask & Target_File) != 0)
        {
            activeLogFile << output;
            activeLogFile.flush();
        }
    }

    unsigned int Logger::logFileCount() const
    {
        unsigned int count = 0;
        for (auto const& entry: std::filesystem::directory_iterator(logFilesDirectory))
        {
            if (!entry.is_regular_file()) // ignore directories, could be nested directories in the logging directory
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

        int amountToDelete = static_cast<int>(logFiles.size()) - static_cast<int>(maxLogFileCount);
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
            if (!entry.is_regular_file()) // ignore directories, could be nested directories in the logging directory
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
            "{}{:%Y-%m-%dT%HZ}",//"{}{:%Y-%m-%dT%H-%M-%SZ}.log",
            logFileNamePrefix,
            std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));

        // if it already exists, try to add a postfix _n (_0, _1, etc.)
        unsigned int postfix = 1;

        std::filesystem::path target;
        do
        {
            target = base;
            target += fmt::format("_{}.log", postfix);
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
            if (size > maxLogFileSizeInBytes)
            {
                // we need to create a new log file
                activeLogFilePath = newLogFilePath();
            }
        }

        // open the file stream, app = append, default behaviour is overwrite
        activeLogFile.open(activeLogFilePath, std::ios_base::app);

        deleteOldLogFilesIfNeeded();
    }
}