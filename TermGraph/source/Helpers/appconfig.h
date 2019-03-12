#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QtGlobal>
#include <QStringList>
#include <QDir>

namespace AppConfig {

class NetworkSettings {
public:
    constexpr static quint16 listenPort = 46377;
};

class SceneSettings {
public:
    constexpr static int FPS = 30;
};

class StdFolderPaths {
public:
    static void createDefaultFoldersIfNeed();

    static QString backupFolder();
    static QString groupsJsonFolder();
    static QString defaultDatabaseFilePath();

private:
    static QString userAppConfigFolder();
};

}

#endif // APPCONFIG_H
