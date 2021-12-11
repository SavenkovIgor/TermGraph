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

#include "source/helpers/appconfig.h"

#include <QStandardPaths>

#include <CommonTools/Platform.h>

#include "source/helpers/fsworks.h"

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

QString StdPaths::backupFolder() { return userAppConfigFolder() + "/MigrationBackup"; }

QString StdPaths::groupsJsonFolder() { return userAppConfigFolder() + "/GroupsJson"; }

QString StdPaths::defaultDatabaseFilePath()
{
    QString dbFilePath;

    if constexpr (Platform::isDesktop()) {
        dbFilePath = userAppConfigFolder() + "/tg.termGraph";
    }

    if constexpr (Platform::isAndroid()) {
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
