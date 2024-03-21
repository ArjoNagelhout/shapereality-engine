//
// Created by Arjo Nagelhout on 21/03/2024.
//

// directories should be accessed via the NSFileManager instead of hard-coding.
//https://stackoverflow.com/questions/36634632/accessing-standard-directories-on-os-x-with-c

#include <common/application_info.h>

#include <cassert>

#include <common/platform/apple/apple.h>

#import <Foundation/NSFileManager.h>

namespace common
{
    // todo: this should be changed once we create an app bundle

    std::filesystem::path ApplicationInfo::userHomeDirectory()
    {
        static std::filesystem::path path = toPath(NSFileManager.defaultManager.homeDirectoryForCurrentUser);
        return path;
    }

    std::filesystem::path ApplicationInfo::loggingDirectory()
    {
        static std::filesystem::path path = userHomeDirectory() / "Library" / "Logs" / bundleIdentifier();
        return path;
    }

    std::filesystem::path ApplicationInfo::persistentDataDirectory()
    {
        NSArray *urls = [NSFileManager.defaultManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
        assert(urls.count > 0 && "path not found");

        return toPath([urls objectAtIndex:0]) / bundleIdentifier();
    }
}