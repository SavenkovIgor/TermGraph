// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtGlobal>

class Platform
{
private:
    enum class PlatformType { Linux = 0, Windows, MacOS, Android, IOS, Wasm };

    consteval static PlatformType currentPlatform()
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
#if defined(Q_OS_WASM)
        return PlatformType::Wasm;
#endif
    }

public:
    consteval static bool isDesktop()
    {
        return isLinux() || isWindows() || isMacOs();
    }

    consteval static bool isMobile()
    {
        return isAndroid() || isIos();
    }

    consteval static bool isWeb()
    {
        return isWasm();
    }

    // clang-format off
    consteval static inline bool isLinux()   { return currentPlatform() == PlatformType::Linux;   }
    consteval static inline bool isWindows() { return currentPlatform() == PlatformType::Windows; }
    consteval static inline bool isMacOs()   { return currentPlatform() == PlatformType::MacOS;   }
    consteval static inline bool isAndroid() { return currentPlatform() == PlatformType::Android; }
    consteval static inline bool isIos()     { return currentPlatform() == PlatformType::IOS;     }
    consteval static inline bool isWasm()    { return currentPlatform() == PlatformType::Wasm;    }
    // clang-format on
};
