#pragma once

#include <QObject>
#include <QtSql>

#include "source/databaseWorks/ndtbl.h"
#include "source/databaseWorks/edgtbl.h"
#include "source/databaseWorks/appconfigtable.h"
#include "source/databaseWorks/termgrouptbl.h"
#include "source/Helpers/appconfig.h"
#include "source/Helpers/fsworks.h"

// Singleton!
class Database
{
public:
    QScopedPointer<NodeTable> nodeTable;
    QScopedPointer<TermGroupTable> groupTable;
    QScopedPointer<AppConfigTable> appConfigTable;

    static Database& instance()
    {
        static Database instance;
        return instance;
    }

    QStringList recordToStrList(QSqlRecord q);
    QStringList queryToStrList(QSqlQuery q);

    Database(const Database&) = delete;
    void operator = (const Database&) = delete;

private:
    explicit Database();
    explicit Database(const QString& filePath);
    ~Database();

    QSqlDatabase *base;

    bool databaseExists(const QString& dbFilePath) const;
    void InitAllTables();

    int currentDbVersion();
    bool needDbUpdate();
    void makeBackupBeforeUpdate(const QString& filePath, const int& oldDbVersion);
    void makeDbUpdate();
    void execMigrationConditions(const int& currentDbVersion);
};
