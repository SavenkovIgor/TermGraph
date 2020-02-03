/*
 * Copyright (c) New Cloud Technologies, Ltd., 2013-2019
 *
 * You can not use the contents of the file in any way without
 * New Cloud Technologies, Ltd. written permission.
 *
 * To obtain such a permit, you should contact New Cloud Technologies, Ltd.
 * at http://ncloudtech.com/contact.html
 *
 */

#pragma once

#include <QtGlobal>

class Platform
{
private:
    enum class PlatformType { Linux = 0, Windows, MacOS, Android, IOS };

    constexpr static PlatformType currentPlatform()
    {
#if defined(Q_OS_ANDROID)
        return PlatformType::Android;
#endif
#if defined(Q_OS_IOS)
        return PlatformType::IOS;
#endif
#if defined(Q_OS_LINUX)
        return PlatformType::Linux;
#endif
#if defined(Q_OS_WIN)
        return PlatformType::Windows;
#endif
#if defined(Q_OS_MACOS)
        return PlatformType::MacOS;
#endif
    }

public:
    constexpr static bool isDesktop()
    {
        return currentPlatform() == PlatformType::Linux || currentPlatform() == PlatformType::Windows
               || currentPlatform() == PlatformType::MacOS;
    }

    constexpr static bool isMobile()
    {
        return currentPlatform() == PlatformType::Android || currentPlatform() == PlatformType::IOS;
    }

    constexpr static bool isLinux() { return currentPlatform() == PlatformType::Linux; }
    constexpr static bool isWindows() { return currentPlatform() == PlatformType::Windows; }
    constexpr static bool isMacOs() { return currentPlatform() == PlatformType::MacOS; }
    constexpr static bool isAndroid() { return currentPlatform() == PlatformType::Android; }
    constexpr static bool isIos() { return currentPlatform() == PlatformType::IOS; }
};
