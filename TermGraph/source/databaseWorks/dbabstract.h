#ifndef DBABSTRACT_H
#define DBABSTRACT_H

#include <QObject>
#include <QtSql>

#include "./ndtbl.h"
#include "./edgtbl.h"
#include "./termgrouptbl.h"

class DBAbstract : public QObject
{
    Q_OBJECT
public:
    NodeTable* nodeTbl  = nullptr;
    TermGroupTable* groupTbl = nullptr;

    explicit DBAbstract(QString file, QObject *parent = nullptr);
    ~DBAbstract();

    void openDB(QString file);

    QStringList recordToStrList(QSqlRecord q);
    QStringList queryToStrList(QSqlQuery q);

public slots:
    void makeStartBaseCheck();

private:
    QSqlDatabase *base;

    void createAllTables();
    void checkCols();

    void normalizeNodesGroupUuid();  // TODO: Delete
};

#endif  // DBABSTRACT_H
