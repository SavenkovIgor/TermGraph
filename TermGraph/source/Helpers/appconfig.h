#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QtGlobal>
#include <QStringList>
#include <QDir>

namespace AppConfig {

class NetworkSettings {
public:
    static quint16 listenPort;
};

class StdFolderPaths {
public:
    static void createDefaultFoldersIfNeed();

    static QString groupsJsonFolder();
    static QString defaultDatabaseFilePath();

private:
    static QString userAppConfigFolder();
};

}

#endif // APPCONFIG_H
