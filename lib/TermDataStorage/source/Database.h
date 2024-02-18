// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QtSql>

#include "source/tables/AppConfigTable.h"
#include "source/tables/TermGroupTable.h"
#include "source/tables/TermTable.h"

// TODO: Move db version here
// TODO: JSON send version must be synced with db version
class Database
{
public:
    QScopedPointer<TermTable>      termTable;
    QScopedPointer<TermGroupTable> groupTable;
    QScopedPointer<AppConfigTable> appConfigTable;

    explicit Database(const QString& filePath, const QString& backupPath);
    ~Database();

    Database(const Database&)       = delete;
    void operator=(const Database&) = delete;

    static QString mDbFilePath;
    static QString mDbBackupFolder;

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
