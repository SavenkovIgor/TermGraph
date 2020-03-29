/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#pragma once

#include <QObject>
#include <QtSql>

#include "source/Helpers/appconfig.h"
#include "source/Helpers/fsworks.h"
#include "source/databaseWorks/appconfigtable.h"
#include "source/databaseWorks/ndtbl.h"
#include "source/databaseWorks/termgrouptbl.h"

// TODO: Move db version here
// TODO: JSON send version must be synced with db version
class Database
{
public:
    QScopedPointer<NodeTable>      nodeTable;
    QScopedPointer<TermGroupTable> groupTable;
    QScopedPointer<AppConfigTable> appConfigTable;

    explicit Database(const QString& filePath = AppSettings::StdPaths::defaultDatabaseFilePath());
    ~Database();

    Database(const Database&) = delete;
    void operator=(const Database&) = delete;

private:
    QSqlDatabase* base;

    bool databaseExists(const QString& dbFilePath) const;
    void InitAllTables();

    int  currentDbVersion();
    bool needDbUpdate();
    void makeBackupBeforeUpdate(const QString& filePath, const int& oldDbVersion);
    void makeDbUpdate();
    void execMigrationConditions(const int& currentDbVersion);

    // Migrations
    void updateNodesToSecondVersion();
    void updateGroupsToSecondVersion();
};
