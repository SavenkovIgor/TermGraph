#include "appconfig.h"

#include <QStandardPaths>

#include "source/Helpers/fsworks.h"

void AppConfig::StdFolderPaths::createDefaultFoldersIfNeed()
{
    QStringList necessaryDirs;

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    necessaryDirs << userAppConfigFolder();
    necessaryDirs << groupsJsonFolder();
    necessaryDirs << backupFolder();
#endif

    for (const auto& path : necessaryDirs)
        FSWorks::createPath(path);
}

QString AppConfig::StdFolderPaths::backupFolder()
{
    return userAppConfigFolder() + "/MigrationBackup";
}

QString AppConfig::StdFolderPaths::groupsJsonFolder()
{
    return userAppConfigFolder() + "/GroupsJson";
}

QString AppConfig::StdFolderPaths::defaultDatabaseFilePath()
{
    QString dbFilePath;

#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX ) || defined( Q_OS_MACOS )
    dbFilePath = userAppConfigFolder() + "/tg.termGraph";
#endif

#if defined( Q_OS_ANDROID )
    dbFilePath = "tg.termGraph";
#endif

    return dbFilePath;
}

QString AppConfig::StdFolderPaths::userAppConfigFolder()
{
    auto path = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();
    assert(!path.isEmpty());
    return path;
}
