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

#include "source/Database/sqlquerybuilder.h"

#include <QFile>
#include <QThread>

#include "source/Database/database.h"

QSqlQuery SqlQueryBuilder::createAppConfigTable() const
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/createappconfigtable.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::insertConfigParameter(const QString &parameter, const QString &value) const
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/insertconfigkeyvalue.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", value);
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneConfigParameter(const QString &parameter) const
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/configparametercount.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryBuilder::selectConfigParameter(const QString &parameter) const
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/selectparameter.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryBuilder::updateConfigParameter(const QString &parameter, const QString &newValue) const
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/updateconfigkeyvalue.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", newValue);
    return query;
}

QSqlQuery SqlQueryBuilder::createGroupsTable() const
{
    auto query = loadQuery(":/sql/queries/version2/groups/creategroupstable.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::insertGroup(const GroupInfoContainer &groupInfo) const
{
    auto query = loadQuery(":/sql/queries/version2/groups/insertgroup.sql");
    query.bindValue(":uuid", groupInfo.uuid.toString());
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryBuilder::selectGroup(const QUuid &groupUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/groups/selectgroupbyuuid.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectGroup(const QString &groupName) const
{
    auto query = loadQuery(":/sql/queries/version2/groups/selectgroupbyname.sql");
    query.bindValue(":name", groupName);
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneGroup(const QUuid &groupUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/groups/groupwithuuidcount.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllGroupUuids()
{
    auto query = loadQuery(":/sql/queries/version2/groups/selectalluuids.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllGroups()
{
    auto query = loadQuery(":/sql/queries/version2/groups/selectallgroups.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::updateGroup(const GroupInfoContainer &groupInfo) const
{
    auto query = loadQuery(":/sql/queries/version2/groups/updategroup.sql");
    query.bindValue(":uuid", groupInfo.uuid);
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryBuilder::deleteGroup(const QUuid &groupUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/groups/deletegroupbyuuid.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::createTermsTable() const
{
    auto query = loadQuery(":/sql/queries/version2/terms/createtermstable.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::insertTerm(const TermData &termInfo) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/insertterm.sql");
    query.bindValue(":uuid", termInfo.uuid.toString());
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

QSqlQuery SqlQueryBuilder::selectTerm(const QUuid &termUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selecttermbyuuid.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneTerm(const QUuid &termUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/termwithuuidcount.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectOneTerm(const QString &term, const QUuid &groupUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selecttermwithnameingroup.sql");
    query.bindValue(":term", term);
    query.bindValue(":groupUuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllTermUuids() const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectalluuids.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllTermUuids(const QUuid &groupUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectalluuidsingroup.sql");
    query.bindValue(":groupUuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllTerms(const QUuid &groupUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selecttermsbygroupuuid.sql");
    query.bindValue(":groupUuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectLastEdit(const QUuid &termUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectlastedit.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::selectAllLastEditAndGroupUuid() const
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectalllasteditandgroupuuid.sql");
    return query;
}

QSqlQuery SqlQueryBuilder::updateTerm(const TermData &termInfo) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/updateterm.sql");

    query.bindValue(":uuid", termInfo.uuid.toString());
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

QSqlQuery SqlQueryBuilder::deleteTerm(const QUuid &termUuid) const
{
    auto query = loadQuery(":/sql/queries/version2/terms/deletetermbyuuid.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryBuilder::loadQuery(const QString &queryPath) const
{
    auto connName = connectionNameForCurrentThread();
    auto query    = QSqlQuery(getDbForConnection(connName));

    query.prepare(loadQueryString(queryPath));
    return query;
}

QSqlQuery SqlQueryBuilder::recordsCount(const QString &tableName) const
{
    auto query = loadQuery(":/sql/queries/common/recordscount.sql");
    query.bindValue(":tableName", tableName);
    return query;
}

QSqlQuery SqlQueryBuilder::dropTable(const QString &tableName) const
{
    QSqlQuery dropTableQuery = loadQuery(":/sql/queries/common/droptable.sql");
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
