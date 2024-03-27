//
// Created by Arjo Nagelhout on 20/03/2024.
//

#ifndef SHAPEREALITY_APPLICATION_INFO_H
#define SHAPEREALITY_APPLICATION_INFO_H

#include <filesystem>

// platform conditionals

#ifdef __APPLE__

#include "TargetConditionals.h"

#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PLATFORM_WINDOWS
#elif __APPLE__
#if TARGET_OS_MAC
#define PLATFORM_MACOS
#elif TARGET_OS_IPHONE
#define PLATFORM_IOS
#elif TARGET_OS_TV
#define PLATFORM_TVOS
#elif TARGET_OS_VISION
#define PLATFORM_VISIONOS
#endif
#elif __ANDROID__
#define PLATFORM_ANDROID
#elif __linux__
#define PLATFORM_LINUX
#elif __unix__
#define PLATFORM_LINUX
#elif defined (_POSIX_VERSION)
#define PLATFORM_LINUX
#endif

namespace common
{
    // global directories:

    // log (on macOS ~/Library/Logs/com.organization_name.app_name/)
    // user data (global settings and cached data)

    enum class Platform
    {
        Windows = 0,
        macOS,
        iOS,
        tvOS,
        visionOS,
        Android,
        Linux
    };

    class ApplicationInfo
    {
    public:
        // platform checking using constexpr instead of macros
        constexpr static Platform platform = Platform::
#ifdef PLATFORM_WINDOWS
        Windows
#elifdef PLATFORM_MACOS
        macOS
#elifdef PLATFORM_IOS
        iOS
#elifdef PLATFORM_TVOS
        tvOS
#elifdef PLATFORM_VISIONOS
        visionOS
#elifdef PLATFORM_ANDROID
        Android
#elifdef PLATFORM_LINUX
        Linux
#endif
        ;

        constexpr static std::string_view extension = "com";

        constexpr static std::string_view applicationName = "Engine";

        constexpr static std::string_view organizationName = "ShapeReality";

        // runtime

        [[nodiscard]] static std::string_view bundleIdentifier();

        // converts ~ to the absolute path (only useful on desktop operating systems)
        [[nodiscard]] static std::filesystem::path userHomeDirectory();

        // e.g. on macOS: the loggingDirectory is ~/Library/Logging/com.organizationName.applicationName/
        [[nodiscard]] static std::filesystem::path loggingDirectory();

        // e.g. on Android: the dataDirectory is the app bundle path
        [[nodiscard]] static std::filesystem::path dataDirectory();

        // e.g. on macOS: the persistentDataDirectory is Library/ApplicationData/com.organizationName.applicationName/
        [[nodiscard]] static std::filesystem::path persistentDataDirectory();

        // trash, on Android we might place this somewhere else
        [[nodiscard]] static std::filesystem::path trashDirectory();

        // https://superuser.com/questions/720836/what-is-the-difference-between-temporary-files-and-cache#:~:text=Cache%20files%20can%20remain%20even,sometimes%20also%20communication%20between%20processes.

        // very temporary, only for things like undo / redo state,
        // or rendering results
        [[nodiscard]] static std::filesystem::path temporaryDirectory();

        // intended to be used for longer, as long as the data for which the
        // cache was created has not been changed. e.g. thumbnails
        [[nodiscard]] static std::filesystem::path cacheDirectory();
    };
}

#endif //SHAPEREALITY_APPLICATION_INFO_H
