#include "dbabstract.h"

DBAbstract::DBAbstract(QString file, QObject *parent) :
    QObject(parent)
{
    base = new QSqlDatabase();
    (*base) = QSqlDatabase::addDatabase("QSQLITE");

    openDB(file);
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
//    delete edgeTbl;
    delete groupTbl;
    delete base;
}

void DBAbstract::openDB(QString file)
{
    if (nodeTbl != nullptr)
        delete nodeTbl;

//    if( edgeTbl != nullptr )
//        delete edgeTbl;

    if (groupTbl != nullptr)
        delete groupTbl;

    base->setDatabaseName(file);
    if (base->open()) {
        qDebug() << "baseIsOpen";
    } else {
        qDebug() << "cantOpenBase" << base->lastError().text();
    }

    nodeTbl  = new NodeTable(base);
//    edgeTbl  = new EdgTbl(base);
    groupTbl = new TermGroupTable(base);
}

void DBAbstract::checkCols()
{
    nodeTbl->checkCols();
//    edgeTbl->checkCols();
    groupTbl->checkCols();
}

void DBAbstract::normalizeNodesGroupUuid()
{
    QMap<int, QUuid> uidMatching;
    for (QUuid uuid : groupTbl->getAllGroupsUuid()) {
        QSqlRecord rec = groupTbl->getGroup(uuid);
        if (rec.count() == 0)
            continue;

        if (!rec.contains(TermGroupColumn::uid))
            continue;

        if (!rec.contains(TermGroupColumn::longUID))
            continue;

        int groupUid = rec.value(TermGroupColumn::uid).toInt();
        QString groupUuid = rec.value(TermGroupColumn::longUID).toString();

        uidMatching[groupUid] = groupUuid;
    }

    for (QUuid nodeUuid : nodeTbl->getAllNodesUuids()) {
        QSqlRecord rec = nodeTbl->getNodeSqlRecord(nodeUuid);

        if (!rec.contains(nodeTbl->uid))
            continue;
        bool ok = false;
        int currentGroup = rec.value(nodeTbl->termGroup).toInt(&ok);
        if (!ok)
            continue;
        if (uidMatching.contains(currentGroup))
            nodeTbl->setGroup(nodeUuid, uidMatching[currentGroup]);
    }
}

void DBAbstract::createAllTables()
{
    nodeTbl->createTable();
//    edgeTbl->createTable();
    groupTbl->createTable();
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
    checkCols();
//    edgeTbl->clearFromDuplicates();
    normalizeNodesGroupUuid();
}
