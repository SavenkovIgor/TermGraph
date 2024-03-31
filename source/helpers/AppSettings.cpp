// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/AppSettings.h"

#include <QStandardPaths>

#include "source/CommonTools/Platform.h"

using namespace AppSettings;

void Paths::createDefaultFoldersIfNeed()
{
    QStringList necessaryDirs;

    if (Platform::isDesktop()) {
        necessaryDirs << userAppConfigFolder();
        necessaryDirs << groupsJsonFolder();
        necessaryDirs << backupFolder();
    }

    auto dir = QDir();

    for (const auto& path : necessaryDirs) {
        dir.mkpath(path);
    }
}

QString Paths::backupFolder() { return userAppConfigFolder() + "/MigrationBackup"; }

QString Paths::groupsJsonFolder() { return userAppConfigFolder() + "/GroupsJson"; }

QString Paths::defaultDatabaseFilePath()
{
    if (Platform::isDesktop()) {
        return userAppConfigFolder() + "/tg.termGraph";
    }

    if (Platform::isAndroid()) {
        return "tg.termGraph";
    }

    return {};
}

QString Paths::userAppConfigFolder()
{
    auto paths = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    assert(!paths.empty());
    return paths.first();
}
