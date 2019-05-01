#include "database.h"

Database::Database() : Database(AppConfig::StdFolderPaths::defaultDatabaseFilePath()) { }

Database::Database(const QString& filePath) :
    nodeTbl(nullptr),
    groupTbl(nullptr),
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

    nodeTbl.reset(new NodeTable(base));
    groupTbl.reset(new TermGroupTable(base));
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
    nodeTbl->initTable();
    groupTbl->initTable();
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
    dbFile.copy(AppConfig::StdFolderPaths::backupFolder() + "/" + fileName);
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
}

QStringList Database::recordToStrList(QSqlRecord q)
{
    QStringList ret;

    for (int i = 0; i < q.count(); i++) {
        ret << q.value(i).toString();
    }
    return ret;
}

QStringList Database::queryToStrList(QSqlQuery q)
{
    QStringList ret;

    for (int i = 0; i < 100000000; i++) {
        if (!q.next())
            break;
        ret << recordToStrList(q.record()).join(" ");
    }

    return ret;
}

bool Database::databaseExists(const QString &dbFilePath) const
{
    return FSWorks::fileExist(dbFilePath);
}
