#pragma once

#include <QtGlobal>
#include <QStringList>
#include <QDir>

namespace AppSettings {

class Platform
{
private:
    enum class PlatformType { Linux = 0, Windows, MacOS, Android, IOS };

    constexpr static PlatformType currentPlatform()
    {
#if defined(Q_OS_LINUX)
        return PlatformType::Linux;
#endif
#if defined(Q_OS_WIN)
        return PlatformType::Windows;
#endif
#if defined(Q_OS_MACOS)
        return PlatformType::MacOS;
#endif
#if defined(Q_OS_ANDROID)
        return PlatformType::Android;
#endif
#if defined(Q_OS_IOS)
        return PlatformType::IOS;
#endif
    }

public:
    constexpr static bool isDesktop()
    {
        return currentPlatform() == PlatformType::Linux
               || currentPlatform() == PlatformType::Windows
               || currentPlatform() == PlatformType::MacOS;
    }

    constexpr static bool isMobile()
    {
        return currentPlatform() == PlatformType::Android || currentPlatform() == PlatformType::IOS;
    }
};

class Network {
public:
    constexpr static quint16 listenPort = 46377;
};

class Scene {
public:
    constexpr static int FPS = 30;
};

class StdPaths {
public:
    static void createDefaultFoldersIfNeed();

    static QString backupFolder();
    static QString groupsJsonFolder();
    static QString defaultDatabaseFilePath();

private:
    static QString userAppConfigFolder();
};

}
