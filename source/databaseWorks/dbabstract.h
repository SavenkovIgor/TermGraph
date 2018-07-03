#ifndef DBABSTRACT_H
#define DBABSTRACT_H

#include <QObject>
#include <QtSql>

#include "ndtbl.h"
#include "edgtbl.h"
#include "termgrouptbl.h"

class DBAbstract : public QObject
{
    Q_OBJECT
public:

    NdTbl* nodeTbl  = nullptr;
    EdgTbl* edgeTbl  = nullptr;
    TermGroupTbl* groupTbl = nullptr;

    explicit DBAbstract( QString file, QObject *parent = 0);
    ~DBAbstract();

    void openDB( QString file );

    QStringList recordToStrList(QSqlRecord q);
    QStringList queryToStrList(QSqlQuery q);

public slots:
    void makeStartBaseCheck();

private:
    QSqlDatabase *base;

    void createAllTables();
    void checkCols();

    void normalizeNodesGroupUuid();
};

#endif // DBABSTRACT_H
