#include "appconfig.h"

#include <QStandardPaths>

#include "source/Helpers/fsworks.h"

using namespace AppSettings;

void StdPaths::createDefaultFoldersIfNeed()
{
    QStringList necessaryDirs;

    if constexpr (Platform::isDesktop()) {
        necessaryDirs << userAppConfigFolder();
        necessaryDirs << groupsJsonFolder();
        necessaryDirs << backupFolder();
    }

    for (const auto& path : necessaryDirs)
        FSWorks::createPath(path);
}

QString StdPaths::backupFolder()
{
    return userAppConfigFolder() + "/MigrationBackup";
}

QString StdPaths::groupsJsonFolder()
{
    return userAppConfigFolder() + "/GroupsJson";
}

QString StdPaths::defaultDatabaseFilePath()
{
    QString dbFilePath;

    if constexpr (Platform::isDesktop()) {
        dbFilePath = userAppConfigFolder() + "/tg.termGraph";
    }

    if constexpr (Platform::isMobile()) {
        dbFilePath = "tg.termGraph";
    }

    return dbFilePath;
}

QString StdPaths::userAppConfigFolder()
{
    auto path = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();
    assert(!path.isEmpty());
    return path;
}
