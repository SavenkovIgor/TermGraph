#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

#include "./ndtbl.h"
#include "./edgtbl.h"
#include "./appconfigtable.h"
#include "./termgrouptbl.h"
#include "../Helpers/appconfig.h"
#include "../Helpers/fsworks.h"

// Singleton!
class Database
{
public:
    NodeTable* nodeTbl  = nullptr;
    TermGroupTable* groupTbl = nullptr;
    AppConfigTable* appConfigTable = nullptr;

    explicit Database(const QString& filePath);
    ~Database();

    QStringList recordToStrList(QSqlRecord q);
    QStringList queryToStrList(QSqlQuery q);

private:
    QSqlDatabase *base;

    bool databaseExists(const QString& dbFilePath) const;
    void InitAllTables();

    int currentDbVersion();
    bool needDbUpdate();
    void makeBackupBeforeUpdate(const QString& filePath, const int& oldDbVersion);
    void makeDbUpdate();
    void execMigrationConditions(const int& currentDbVersion);
};

#endif  // DATABASE_H
