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
        qDebug() << "baseIsOpen";
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

    makeDbUpdatesIfNeed();
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

void DBAbstract::makeDbUpdatesIfNeed()
{
    if (appConfigTable->isDbVersionActual())
        return;

    auto dbVersion = appConfigTable->getDbVersion();
    qDebug() << "Updating database!";
    qDebug() << "Start version: " << dbVersion;

    if (dbVersion < 1) {
        qDebug() << "Initing appConfig table";
        appConfigTable->initTable();
    }

    appConfigTable->updateDbVersionNumber();
    qDebug() << "Update finished. New db version:" << appConfigTable->getDbVersion();
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
