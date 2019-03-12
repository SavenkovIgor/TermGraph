#include "appconfig.h"

void AppConfig::StdFolderPaths::createDefaultFoldersIfNeed()
{
    QStringList necessaryDirs;

#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX ) || defined( Q_OS_MACOS )
    necessaryDirs << userAppConfigFolder();
    necessaryDirs << groupsJsonFolder();
    necessaryDirs << backupFolder();
#endif

    for (auto path : necessaryDirs) {
        QDir().mkpath(path);
    }
}

QString AppConfig::StdFolderPaths::backupFolder()
{
    return userAppConfigFolder() + "/DbBackup";
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
    return QDir::home().absolutePath() + "/.TermGraph";
}
