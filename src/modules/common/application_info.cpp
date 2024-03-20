//
// Created by Arjo Nagelhout on 20/03/2024.
//

#include "application_info.h"

#include <cstdlib>
#include <cassert>

#include <common/logger.h>

#include <fmt/core.h>

namespace common
{
    std::string_view ApplicationInfo::applicationIdentifier()
    {
        static std::string identifier = fmt::format("{}.{}.{}", extension, organizationName, applicationName);
        return identifier;
    }

    std::filesystem::path ApplicationInfo::userHomeDirectory()
    {
        char const* path;
        if constexpr (platform == Platform::Windows)
        {
            // https://stackoverflow.com/questions/42696260/how-to-get-user-home-directory-on-windows
            path = std::getenv("USERPROFILE");
        }
        else
        {
            path = std::getenv("HOME");
        }
        assert(path && "failed to get path");
        return path;
    }

    std::filesystem::path ApplicationInfo::loggingDirectory()
    {
        std::filesystem::path path = userHomeDirectory();

        if constexpr (platform == Platform::macOS)
        {
            path /= "Library";
            path /= "Logs";
        }
        else
        {
            assert(false && "logging to disk not supported on this platform");
        }
        return path;
    }

    std::filesystem::path ApplicationInfo::persistentDataDirectory()
    {

    }
}