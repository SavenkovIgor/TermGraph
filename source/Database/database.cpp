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

#include "source/Database/database.h"

#include <QDateTime>
#include <QString>
#include <QUuid>

#include "source/Database/tools/dbtools.h"
#include "source/Managers/notificationmanager.h"

Database::Database(const QString& filePath)
    : nodeTable(nullptr)
    , groupTable(nullptr)
    , appConfigTable(nullptr)
{
    base    = new QSqlDatabase();
    (*base) = QSqlDatabase::addDatabase("QSQLITE");

    // Check base exist
    auto baseExists = databaseExists(filePath);

    if (baseExists) {
        qInfo("Base file is exists");
    } else {
        qInfo("Base file don't exist");
    }

    // Create database if not exist earlier
    base->setDatabaseName(filePath);

    if (base->open()) {
        qInfo("Database opened");
    } else {
        auto msg = QString("Can't open database %1").arg(base->lastError().text());
        NotificationManager::showDebug(msg);
    }

    nodeTable.reset(new NodeTable(base));
    groupTable.reset(new TermGroupTable(base));
    appConfigTable.reset(new AppConfigTable(base));

    // If database just created, create all tables
    if (!baseExists) {
        qInfo("Creating tables");
        InitAllTables();
    }

    if (needDbUpdate()) {
        auto oldDbVersion = currentDbVersion();
        // Close, make backup, open again, and then update base
        qInfo("Closing database");
        base->close();
        makeBackupBeforeUpdate(filePath, oldDbVersion);
        qInfo("Opening database");
        base->open();
        makeDbUpdate();

        // Recreate tables after update
        nodeTable.reset(new NodeTable(base));
        groupTable.reset(new TermGroupTable(base));
        appConfigTable.reset(new AppConfigTable(base));
    } else {
        qInfo("Database is already actual");
    }
}

Database::~Database() { delete base; }

void Database::InitAllTables()
{
    nodeTable->initTable();
    groupTable->initTable();
    appConfigTable->initTable();
}

int Database::currentDbVersion() { return appConfigTable->getDbVersion(); }

bool Database::needDbUpdate() { return !appConfigTable->isDbVersionActual(); }

void Database::makeBackupBeforeUpdate(const QString& filePath, const int& oldDbVersion)
{
    qInfo("Making backup");
    QFile   dbFile(filePath);
    QString fileName;
    fileName += "dbVersion_" + QString::number(oldDbVersion);
    fileName += " date_" + QDateTime::currentDateTime().toString(Qt::ISODate);
    fileName += ".termGraph";
    fileName.replace(':', '_');
    dbFile.copy(AppSettings::StdPaths::backupFolder() + "/" + fileName);
}

void Database::makeDbUpdate()
{
    auto dbVersion = currentDbVersion();

    qInfo("Updating database!");
    qInfo("Start version: %d", dbVersion);

    execMigrationConditions(dbVersion);

    appConfigTable->updateDbVersionNumber();

    qInfo("Update finished. New db version: %d", currentDbVersion());
}

void Database::execMigrationConditions(const int& currentDbVersion)
{
    if (currentDbVersion < 1) {
        qInfo("Initing appConfig table");
        appConfigTable->initTable();
    }

    if (currentDbVersion < 2) {
        qInfo("Move node table");
        DbTools::startTransaction(base);

        updateNodesToSecondVersion();
        updateGroupsToSecondVersion();

        DbTools::endTransaction(base);

        // Reduce database file
        DbTools::reduceSpace(base);
    }
}

void Database::updateNodesToSecondVersion()
{
    // Changes list
    // 1) Rename of table termNode -> terms
    // 2) Remove columns in termNode: uid, lastRemindNum, remindNum, atLearn
    // 3) Update columns:
    //    - longUID -> uuid; TEXT -> TEXT PRIMARY KEY NOT NULL
    //    - term; TEXT -> TEXT NOT NULL
    //    - wikiRef -> wikiUrl
    //    - wikiImg -> wikiImage
    //    - termGroup -> groupUuid; INTEGER DEFAULT '-1' -> TEXT NOT NULL
    //    - lastEdit; TEXT -> TEXT NOT NULL

    // Creating new table
    QStringList nodeColumns;
    nodeColumns << "uuid        TEXT PRIMARY KEY NOT NULL";
    nodeColumns << "term        TEXT NOT NULL";
    nodeColumns << "definition  TEXT";
    nodeColumns << "description TEXT";
    nodeColumns << "examples    TEXT";
    nodeColumns << "wikiUrl     TEXT";
    nodeColumns << "wikiImage   TEXT";
    nodeColumns << "groupUuid   TEXT NOT NULL";
    nodeColumns << "lastEdit    TEXT NOT NULL";

    QString createTermsTableQuery = "CREATE TABLE terms (" + nodeColumns.join(", ") + ")";
    DbTools::startQuery(base, createTermsTableQuery);

    // Selecting all previous data
    auto oldNodeTable = DbTools::startQuery(base, "SELECT * FROM termNode");

    // Copy data from old table to new
    while (oldNodeTable.next()) {
        InsertContainer::List values;

        values.push_back(InsertContainer("uuid", oldNodeTable.value("longUID").toString()));
        values.push_back(InsertContainer("term", oldNodeTable.value("term").toString()));
        values.push_back(InsertContainer("definition", oldNodeTable.value("definition").toString()));
        values.push_back(InsertContainer("description", oldNodeTable.value("description").toString()));
        values.push_back(InsertContainer("examples", oldNodeTable.value("examples").toString()));
        values.push_back(InsertContainer("wikiUrl", oldNodeTable.value("wikiRef").toString()));
        values.push_back(InsertContainer("wikiImage", oldNodeTable.value("wikiImg").toString()));
        values.push_back(InsertContainer("groupUuid", oldNodeTable.value("termGroup").toString()));
        values.push_back(InsertContainer("lastEdit", oldNodeTable.value("lastEdit").toString()));

        auto insertQuery = SqlQueryConstructor::insertQuery("terms", values);
        DbTools::startQuery(base, insertQuery);
    }

    auto countInOld = DbTools::recordsCount("termNode");
    auto countInNew = DbTools::recordsCount("terms");

    qInfo("Old count: %d new count: %d", countInOld, countInNew);

    if (countInOld == countInNew) {
        // Dropping old table
        DbTools::startQuery(SqlQueryConstructor::dropTable("termNode"));
    }
}

void Database::updateGroupsToSecondVersion()
{
    // Changes list
    // 1) Rename of table termGroup -> groups
    // 2) Removed: type
    // 3) Update columns:
    //    - longUID -> uuid; TEXT                 -> TEXT PRIMARY KEY NOT NULL
    //    - name;            TEXT UNIQUE          -> TEXT UNIQUE NOT NULL

    // Creating new table
    QStringList nodeColumns;
    nodeColumns << "uuid    TEXT PRIMARY KEY NOT NULL";
    nodeColumns << "name    TEXT UNIQUE NOT NULL";
    nodeColumns << "comment TEXT";

    QString createGroupsTableQuery = "CREATE TABLE groups (" + nodeColumns.join(", ") + ")";
    DbTools::startQuery(base, createGroupsTableQuery);

    // Selecting all previous data
    auto oldGroupsTable = DbTools::startQuery(base, "SELECT * FROM termGroup");

    // Copy data from old table to new and drop type
    while (oldGroupsTable.next()) {
        InsertContainer::List values;

        values.push_back(InsertContainer("uuid", oldGroupsTable.value("longUID").toString()));
        values.push_back(InsertContainer("name", oldGroupsTable.value("name").toString()));
        values.push_back(InsertContainer("comment", oldGroupsTable.value("comment").toString()));

        auto insertQuery = SqlQueryConstructor::insertQuery("groups", values);
        DbTools::startQuery(base, insertQuery);
    }

    auto countInOld = DbTools::recordsCount("termGroup");
    auto countInNew = DbTools::recordsCount("groups");

    qInfo("Old count: %d new count: %d", countInOld, countInNew);

    if (countInOld == countInNew) {
        // Dropping old table
        DbTools::startQuery(SqlQueryConstructor::dropTable("termGroup"));
    }
}

bool Database::databaseExists(const QString& dbFilePath) const { return FSWorks::fileExist(dbFilePath); }
