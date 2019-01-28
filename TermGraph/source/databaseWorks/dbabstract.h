#ifndef DBABSTRACT_H
#define DBABSTRACT_H

#include <QObject>
#include <QtSql>

#include "./ndtbl.h"
#include "./edgtbl.h"
#include "./termgrouptbl.h"

class DBAbstract
{
public:
    NodeTable* nodeTbl  = nullptr;
    TermGroupTable* groupTbl = nullptr;

    explicit DBAbstract(QString file);
    ~DBAbstract();

    void openDB(QString file);

    QStringList recordToStrList(QSqlRecord q);
    QStringList queryToStrList(QSqlQuery q);

    void makeStartBaseCheck();

private:
    QSqlDatabase *base;

    void createAllTables();
    void checkCols();

    void normalizeNodesGroupUuid();  // TODO: Delete
};

#endif  // DBABSTRACT_H
