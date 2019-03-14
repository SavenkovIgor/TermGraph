#include "dbabstract.h"

DBAbstract::DBAbstract(const QString &filePath)
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

    nodeTbl  = new NodeTable(base);
    groupTbl = new TermGroupTable(base);
    appConfigTable = new AppConfigTable(base);

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

DBAbstract::~DBAbstract()
{
    delete nodeTbl;
    delete groupTbl;
    delete appConfigTable;
    delete base;
}

void DBAbstract::InitAllTables()
{
    nodeTbl->initTable();
    groupTbl->initTable();
    appConfigTable->initTable();
}

int DBAbstract::currentDbVersion()
{
    return appConfigTable->getDbVersion();
}

bool DBAbstract::needDbUpdate()
{
    return !appConfigTable->isDbVersionActual();
}

void DBAbstract::makeBackupBeforeUpdate(const QString &filePath, const int &oldDbVersion)
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

void DBAbstract::makeDbUpdate()
{
    auto dbVersion = currentDbVersion();
    qDebug() << "Updating database!";
    qDebug() << "Start version: " << dbVersion;

    execMigrationConditions(dbVersion);

    appConfigTable->updateDbVersionNumber();
    qDebug() << "Update finished. New db version:" << currentDbVersion();
}

void DBAbstract::execMigrationConditions(const int &currentDbVersion)
{
    if (currentDbVersion < 1) {
        qDebug() << "Initing appConfig table";
        appConfigTable->initTable();
    }
}

QStringList DBAbstract::recordToStrList(QSqlRecord q)
{
    QStringList ret;

    for (int i = 0; i < q.count(); i++) {
        ret << q.value(i).toString();
    }
    return ret;
}

QStringList DBAbstract::queryToStrList(QSqlQuery q)
{
    QStringList ret;

    for (int i = 0; i < 100000000; i++) {
        if (!q.next())
            break;
        ret << recordToStrList(q.record()).join(" ");
    }

    return ret;
}

bool DBAbstract::databaseExists(const QString &dbFilePath) const
{
    return FSWorks::fileExist(dbFilePath);
}
