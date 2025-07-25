// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QDir>
#include <QStandardPaths>
#include <QStringList>
#include <QtGlobal>

#include "source/CommonTools/Platform.h"

export module AppSettings;

export namespace AppSettings {

class Paths
{
public:
    static void createDefaultFoldersIfNeed()
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

    static QString backupFolder() { return userAppConfigFolder() + "/MigrationBackup"; }
    static QString groupsJsonFolder() { return userAppConfigFolder() + "/GroupsJson"; }
    static QString defaultDatabaseFilePath()
    {
        if (Platform::isDesktop()) {
            return userAppConfigFolder() + "/tg.termGraph";
        }

        if (Platform::isAndroid()) {
            return "tg.termGraph";
        }

        return {};
    }

private:
    static QString userAppConfigFolder()
    {
        auto paths = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        assert(!paths.empty());
        return paths.first();
    }
};

} // namespace AppSettings
