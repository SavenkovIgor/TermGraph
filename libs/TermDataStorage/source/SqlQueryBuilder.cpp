/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/SqlQueryBuilder.h"

#include <QFile>
#include <QThread>

#include "source/Database.h"

QSqlQuery SqlQueryBuilder::createAppConfigTable() const
{
    auto query = loadQuery(":/sql/version2/appConfig/CreateTable.sql");
    return query;
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

QSqlQuery SqlQueryBuilder::createGroupsTable() const
{
    auto query = loadQuery(":/sql/version2/groups/CreateTable.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::insertGroup(const GroupData &groupInfo) const
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

QSqlQuery SqlQueryBuilder::selectAllGroupUuids()
{
    auto query = loadQuery(":/sql/version2/groups/SelectAllUuids.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllGroups()
{
    auto query = loadQuery(":/sql/version2/groups/SelectAll.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::updateGroup(const GroupData &groupInfo) const
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

QSqlQuery SqlQueryBuilder::createTermsTable() const
{
    auto query = loadQuery(":/sql/version2/terms/CreateTable.sql");
    return query;
}

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
    auto query = loadQuery(":/sql/version2/terms/SelectAllLastEdit.sql");
    return query;
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
    QFile queryFile(queryPath);
    auto  open = queryFile.open(QIODevice::ReadOnly);
    assert(open);
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
    if (QSqlDatabase::contains(connectionName))
        return QSqlDatabase::database(connectionName);

    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(Database::mDbFilePath);
    assert(db.open());
    return db;
}
