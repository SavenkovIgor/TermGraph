// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataStorage/SqlQueryBuilder.h"

#include <QFile>
#include <QThread>

#include "source/TermDataStorage/Database.h"

QSqlQuery SqlQueryBuilder::createAppConfigTable() const
{
    return loadQuery(":/sql/version2/appConfig/CreateTable.sql");
}

QSqlQuery SqlQueryBuilder::insertConfigParameter(const QString &parameter, const QString &value) const
{
    auto query = loadQuery(":/sql/version2/appConfig/InsertParameter.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", value);
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneConfigParameter(const QString &parameter) const
{
    auto query = loadQuery(":/sql/version2/appConfig/SelectParameterCount.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryBuilder::selectConfigParameter(const QString &parameter) const
{
    auto query = loadQuery(":/sql/version2/appConfig/SelectParameter.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryBuilder::updateConfigParameter(const QString &parameter, const QString &newValue) const
{
    auto query = loadQuery(":/sql/version2/appConfig/UpdateParameter.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", newValue);
    return query;
}

QSqlQuery SqlQueryBuilder::createGroupsTable() const { return loadQuery(":/sql/version2/groups/CreateTable.sql"); }

QSqlQuery SqlQueryBuilder::insertGroup(const GroupSummary &groupInfo) const
{
    assert(groupInfo.uuid);

    auto query = loadQuery(":/sql/version2/groups/InsertGroup.sql");
    query.bindValue(":uuid", groupInfo.uuid->toString());
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryBuilder::selectGroup(const GroupUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/groups/SelectGroup.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectGroup(const QString &groupName) const
{
    auto query = loadQuery(":/sql/version2/groups/SelectGroupByName.sql");
    query.bindValue(":name", groupName);
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneGroup(const GroupUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/groups/SelectGroupCount.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllGroups() { return loadQuery(":/sql/version2/groups/SelectAll.sql"); }

QSqlQuery SqlQueryBuilder::updateGroup(const GroupSummary &groupInfo) const
{
    assert(groupInfo.uuid);

    auto query = loadQuery(":/sql/version2/groups/UpdateGroup.sql");
    query.bindValue(":uuid", groupInfo.uuid->toString());
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryBuilder::deleteGroup(const GroupUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/groups/DeleteGroup.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::createTermsTable() const { return loadQuery(":/sql/version2/terms/CreateTable.sql"); }

QSqlQuery SqlQueryBuilder::insertTerm(const TermData &termInfo) const
{
    assert(termInfo.uuid);

    auto query = loadQuery(":/sql/version2/terms/InsertTerm.sql");
    query.bindValue(":uuid", termInfo.uuid->toString());
    query.bindValue(":term", termInfo.term);
    query.bindValue(":definition", termInfo.definition);
    query.bindValue(":description", termInfo.description);
    query.bindValue(":examples", termInfo.examples);
    query.bindValue(":wikiUrl", termInfo.wikiUrl);
    query.bindValue(":wikiImage", termInfo.wikiImage);
    query.bindValue(":groupUuid", termInfo.groupUuid.toString());
    query.bindValue(":lastEdit", termInfo.lastEdit.toString(Qt::ISODate));
    return query;
}

QSqlQuery SqlQueryBuilder::selectTerm(const TermUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/terms/SelectTerm.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneTerm(const TermUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/terms/SelectTermCount.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneTerm(const QString &term, const GroupUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/terms/SelectTermWithNameAndGroup.sql");
    query.bindValue(":term", term);
    query.bindValue(":groupUuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllTerms(const GroupUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/terms/SelectTermsByGroup.sql");
    query.bindValue(":groupUuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectLastEdit(const TermUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/terms/SelectLastEdit.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllLastEditAndGroupUuid() const
{
    return loadQuery(":/sql/version2/terms/SelectAllLastEdit.sql");
}

QSqlQuery SqlQueryBuilder::updateTerm(const TermData &termInfo) const
{
    assert(termInfo.uuid);

    auto query = loadQuery(":/sql/version2/terms/UpdateTerm.sql");

    query.bindValue(":uuid", termInfo.uuid->toString());
    query.bindValue(":term", termInfo.term);
    query.bindValue(":definition", termInfo.definition);
    query.bindValue(":description", termInfo.description);
    query.bindValue(":examples", termInfo.examples);
    query.bindValue(":wikiUrl", termInfo.wikiUrl);
    query.bindValue(":wikiImage", termInfo.wikiImage);
    query.bindValue(":groupUuid", termInfo.groupUuid.toString());
    query.bindValue(":lastEdit", termInfo.lastEdit.toString(Qt::ISODate));

    return query;
}

QSqlQuery SqlQueryBuilder::deleteTerm(const TermUuid &uuid) const
{
    auto query = loadQuery(":/sql/version2/terms/DeleteTerm.sql");
    query.bindValue(":uuid", uuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::loadQuery(const QString &queryPath) const
{
    // Basic resource path check
    assert(queryPath.startsWith(":/"));

    auto connName = connectionNameForCurrentThread();
    auto query    = QSqlQuery(getDbForConnection(connName));

    query.prepare(loadQueryString(queryPath));
    return query;
}

QSqlQuery SqlQueryBuilder::recordsCount(const QString &tableName) const
{
    auto query = loadQuery(":/sql/common/RecordsCount.sql");
    query.bindValue(":tableName", tableName);
    return query;
}

QSqlQuery SqlQueryBuilder::dropTable(const QString &tableName) const
{
    QSqlQuery dropTableQuery = loadQuery(":/sql/common/DropTable.sql");
    dropTableQuery.bindValue(":tableName", tableName);
    return dropTableQuery;
}

QString SqlQueryBuilder::loadQueryString(const QString &queryPath) const
{
    QFile                 queryFile(queryPath);
    [[maybe_unused]] auto isOpen = queryFile.open(QIODevice::ReadOnly);
    assert(isOpen);
    return QString(queryFile.readAll());
}

QLatin1String SqlQueryBuilder::connectionNameForCurrentThread()
{
    auto threadPtrNum = reinterpret_cast<std::uintptr_t>(QThread::currentThread());
    auto threadStrId  = QString("connection_from_thread_0x%1").arg(threadPtrNum);
    return QLatin1String(threadStrId.toLocal8Bit());
}

QSqlDatabase SqlQueryBuilder::getDbForConnection(QLatin1String connectionName)
{
    if (QSqlDatabase::contains(connectionName)) {
        return QSqlDatabase::database(connectionName);
    }

    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(Database::mDbFilePath);
    [[maybe_unused]] auto isOpen = db.open();
    assert(isOpen);
    return db;
}
