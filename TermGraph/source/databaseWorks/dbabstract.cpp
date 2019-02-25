#include "dbabstract.h"

DBAbstract::DBAbstract(QString file)
{
    base = new QSqlDatabase();
    (*base) = QSqlDatabase::addDatabase("QSQLITE");

    base->setDatabaseName(file);
    if (base->open()) {
        qDebug() << "baseIsOpen";
    } else {
        qDebug() << "cantOpenBase" << base->lastError().text();
    }

    nodeTbl  = new NodeTable(base);
    groupTbl = new TermGroupTable(base);
    appConfigTable = new AppConfigTable(base);

//    base.setHostName("127.0.0.1");
//    base.setUserName("root");
//    base.setPassword("12345");
//    base.setPort(3306);
//    if( base->open() ) {
//        ui->connState->setText("connect");
//    } else {
//        ui->connState->setText("notConnected");
//        qDebug()<<base->lastError().text();
//    }
}

DBAbstract::~DBAbstract()
{
    delete nodeTbl;
    delete groupTbl;
    delete appConfigTable;
    delete base;
}

void DBAbstract::checkCols()
{
    nodeTbl->checkCols();
    groupTbl->checkCols();
}

void DBAbstract::createAllTables()
{
    nodeTbl->createTable();
    groupTbl->createTable();
    appConfigTable->createTable();
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

void DBAbstract::makeStartBaseCheck()
{
    createAllTables();
    //checkCols();
}
