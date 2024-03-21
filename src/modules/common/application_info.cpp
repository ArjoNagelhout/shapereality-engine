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
    std::string_view ApplicationInfo::bundleIdentifier()
    {
        static std::string identifier = fmt::format("{}.{}.{}", extension, organizationName, applicationName);
        return identifier;
    }
}