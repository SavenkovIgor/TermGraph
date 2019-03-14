#ifndef DBABSTRACT_H
#define DBABSTRACT_H

#include <QObject>
#include <QtSql>

#include "./ndtbl.h"
#include "./edgtbl.h"
#include "./appconfigtable.h"
#include "./termgrouptbl.h"
#include "../Helpers/appconfig.h"
#include "../Helpers/fsworks.h"

class DBAbstract
{
public:
    NodeTable* nodeTbl  = nullptr;
    TermGroupTable* groupTbl = nullptr;
    AppConfigTable* appConfigTable = nullptr;

    explicit DBAbstract(const QString& filePath);
    ~DBAbstract();

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

#endif  // DBABSTRACT_H
