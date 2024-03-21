//
// Created by Arjo Nagelhout on 29/12/2023.
//

#include "logger.h"

#include <iostream>

#include <chrono>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <cassert>
#include <utility>
#include <vector>

#include <common/application_info.h>

namespace common
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
        std::string logFileNamePrefix_,
        LoggerDescriptor descriptor_,
        Target_ targetMask_,
        Severity_ severityMask_,
        Verbosity verbosity_) :
        logFilesDirectory(std::move(logFilesDirectory_)),
        logFileNamePrefix(std::move(logFileNamePrefix_)),
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
        static Logger instance_{common::ApplicationInfo::loggingDirectory(), "log_"};
        return instance_;
    }

    void Logger::log(std::string const& message, Severity_ severity_, Verbosity verbosity_)
    {
        std::string output = fmt::format(
            "[{:%Y-%m-%dT%H:%M:%SZ}][{}][{}] {}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()),
            toString(verbosity_),
            toString(severity_),
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

        if ((targetMask & Target_Console) != 0)
        {
            std::cout << output << std::endl;
        }

        if ((targetMask & Target_File) != 0)
        {
            activeLogFile << output << "\n";
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


    void Logger::deleteFirstLogFile() const
    {
        if (logFileCount() == 0)
        {
            return; // no need to delete anything
        }
        std::vector<std::filesystem::path> const logFiles = sortedLogFiles();
        std::filesystem::path const& first = logFiles.front();
        std::filesystem::remove(first);
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
        std::sort(paths.begin(), paths.end());
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
        return logFilesDirectory / fmt::format(
            "{}{:%Y-%m-%dT%H-%M-%SZ}.log",
            logFileNamePrefix,
            std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
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

        // open the file stream
        activeLogFile.open(activeLogFilePath);
    }
}