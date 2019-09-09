#include "database.h"

#include <QString>
#include <QUuid>
#include <QDateTime>

#include "source/databaseWorks/tools/dbtools.h"

Database::Database() : Database(AppSettings::StdPaths::defaultDatabaseFilePath()) { }

Database::Database(const QString& filePath) :
    nodeTable(nullptr),
    groupTable(nullptr),
    appConfigTable(nullptr)
{
    base = new QSqlDatabase();
    (*base) = QSqlDatabase::addDatabase("QSQLITE");

    // Check base exist
    auto baseExists = databaseExists(filePath);

    if (baseExists) {
        qDebug() << "Base file is exists";
    } else {
        qDebug() << "Base file don't exist";
    }

    // Create database if not exist earlier
    base->setDatabaseName(filePath);

    if (base->open()) {
        qDebug() << "Database opened";
    } else {
        qDebug() << "cantOpenBase" << base->lastError().text();
    }

    nodeTable.reset(new NodeTable(base));
    groupTable.reset(new TermGroupTable(base));
    appConfigTable.reset(new AppConfigTable(base));

    // If database just created, create all tables
    if (!baseExists) {
        qDebug() << "Creating tables";
        InitAllTables();
    }

    if (needDbUpdate()) {
        auto oldDbVersion = currentDbVersion();
        // Close, make backup, open again, and then update base
        qDebug() << "Closing database";
        base->close();
        makeBackupBeforeUpdate(filePath, oldDbVersion);
        qDebug() << "Opening database";
        base->open();
        makeDbUpdate();

        // Recreate tables after update
        nodeTable.reset(new NodeTable(base));
        groupTable.reset(new TermGroupTable(base));
        appConfigTable.reset(new AppConfigTable(base));
    } else {
        qDebug() << "Database is actual. No need to update";
    }
}

Database::~Database()
{
    delete base;
}

void Database::InitAllTables()
{
    nodeTable->initTable();
    groupTable->initTable();
    appConfigTable->initTable();
}

int Database::currentDbVersion()
{
    return appConfigTable->getDbVersion();
}

bool Database::needDbUpdate()
{
    return !appConfigTable->isDbVersionActual();
}

void Database::makeBackupBeforeUpdate(const QString &filePath, const int &oldDbVersion)
{
    qDebug() << "Making backup";
    QFile dbFile(filePath);
    QString fileName;
    fileName += "dbVersion_" + QString::number(oldDbVersion);
    fileName += " date_" + QDateTime::currentDateTime().toString(Qt::ISODate);
    fileName += ".termGraph";
    fileName.replace(':','_');
    dbFile.copy(AppSettings::StdPaths::backupFolder() + "/" + fileName);
}

void Database::makeDbUpdate()
{
    auto dbVersion = currentDbVersion();
    qDebug() << "Updating database!";
    qDebug() << "Start version: " << dbVersion;

    execMigrationConditions(dbVersion);

    appConfigTable->updateDbVersionNumber();
    qDebug() << "Update finished. New db version:" << currentDbVersion();
}

void Database::execMigrationConditions(const int &currentDbVersion)
{
    if (currentDbVersion < 1) {
        qDebug() << "Initing appConfig table";
        appConfigTable->initTable();
    }

    if (currentDbVersion < 2) {
        qDebug() << "Move node table";
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

        values.push_back(InsertContainer("uuid",        oldNodeTable.value("longUID").toString()));
        values.push_back(InsertContainer("term",        oldNodeTable.value("term").toString()));
        values.push_back(InsertContainer("definition",  oldNodeTable.value("definition").toString()));
        values.push_back(InsertContainer("description", oldNodeTable.value("description").toString()));
        values.push_back(InsertContainer("examples",    oldNodeTable.value("examples").toString()));
        values.push_back(InsertContainer("wikiUrl",     oldNodeTable.value("wikiRef").toString()));
        values.push_back(InsertContainer("wikiImage",   oldNodeTable.value("wikiImg").toString()));
        values.push_back(InsertContainer("groupUuid",   oldNodeTable.value("termGroup").toString()));
        values.push_back(InsertContainer("lastEdit",    oldNodeTable.value("lastEdit").toString()));

        auto insertQuery = SqlQueryConstructor::insertQuery("terms", values);
        DbTools::startQuery(base, insertQuery);
    }

    auto countInOld = DbTools::recordsCount(base, "termNode");
    auto countInNew = DbTools::recordsCount(base, "terms");
    qDebug() << "Old count" << countInOld << "new count" << countInNew;

    if (countInOld == countInNew) {
        // Dropping old table
        DbTools::startQuery(base, SqlQueryConstructor::dropTable("termNode"));
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

        values.push_back(InsertContainer("uuid",    oldGroupsTable.value("longUID").toString()));
        values.push_back(InsertContainer("name",    oldGroupsTable.value("name").toString()));
        values.push_back(InsertContainer("comment", oldGroupsTable.value("comment").toString()));

        auto insertQuery = SqlQueryConstructor::insertQuery("groups", values);
        DbTools::startQuery(base, insertQuery);
    }

    auto countInOld = DbTools::recordsCount(base, "termGroup");
    auto countInNew = DbTools::recordsCount(base, "groups");
    qDebug() << "Old count" << countInOld << "new count" << countInNew;

    if (countInOld == countInNew) {
        // Dropping old table
        DbTools::startQuery(base, SqlQueryConstructor::dropTable("termGroup"));
    }
}

bool Database::databaseExists(const QString &dbFilePath) const
{
    return FSWorks::fileExist(dbFilePath);
}
