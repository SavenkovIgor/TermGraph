// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/appconfig.h"

#include <QStandardPaths>

#include <CommonTools/Platform.h>

#include "source/helpers/fsworks.h"

using namespace AppSettings;

void StdPaths::createDefaultFoldersIfNeed()
{
    QStringList necessaryDirs;

    if (Platform::isDesktop()) {
        necessaryDirs << userAppConfigFolder();
        necessaryDirs << groupsJsonFolder();
        necessaryDirs << backupFolder();
    }

    for (const auto& path : necessaryDirs)
        FSWorks::createPath(path);
}

QString StdPaths::backupFolder() { return userAppConfigFolder() + "/MigrationBackup"; }

QString StdPaths::groupsJsonFolder() { return userAppConfigFolder() + "/GroupsJson"; }

QString StdPaths::defaultDatabaseFilePath()
{
    QString dbFilePath;

    if (Platform::isDesktop()) {
        dbFilePath = userAppConfigFolder() + "/tg.termGraph";
    }

    if (Platform::isAndroid()) {
        dbFilePath = "tg.termGraph";
    }

    return dbFilePath;
}

QString StdPaths::userAppConfigFolder()
{
    auto paths = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    assert(!paths.empty());
    return paths.first();
}
