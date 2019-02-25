#ifndef DBABSTRACT_H
#define DBABSTRACT_H

#include <QObject>
#include <QtSql>

#include "./ndtbl.h"
#include "./edgtbl.h"
#include "./appconfigtable.h"
#include "./termgrouptbl.h"

class DBAbstract
{
public:
    NodeTable* nodeTbl  = nullptr;
    TermGroupTable* groupTbl = nullptr;
    AppConfigTable* appConfigTable = nullptr;

    explicit DBAbstract(QString file);
    ~DBAbstract();

    QStringList recordToStrList(QSqlRecord q);
    QStringList queryToStrList(QSqlQuery q);

    void makeStartBaseCheck();

private:
    QSqlDatabase *base;

    void createAllTables();
    void checkCols();
};

#endif  // DBABSTRACT_H
