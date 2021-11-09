/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QtGlobal>

class Platform
{
private:
    enum class PlatformType { Linux = 0, Windows, MacOS, Android, IOS };

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
    }

public:
    consteval static bool isDesktop()
    {
        return currentPlatform() == PlatformType::Linux || currentPlatform() == PlatformType::Windows
            || currentPlatform() == PlatformType::MacOS;
    }

    consteval static bool isMobile()
    {
        return currentPlatform() == PlatformType::Android || currentPlatform() == PlatformType::IOS;
    }

    consteval static bool isLinux() { return currentPlatform() == PlatformType::Linux; }
    consteval static bool isWindows() { return currentPlatform() == PlatformType::Windows; }
    consteval static bool isMacOs() { return currentPlatform() == PlatformType::MacOS; }
    consteval static bool isAndroid() { return currentPlatform() == PlatformType::Android; }
    consteval static bool isIos() { return currentPlatform() == PlatformType::IOS; }
};
