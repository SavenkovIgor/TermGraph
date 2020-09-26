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

#include "source/Database/sqlqueryconstructor.h"

#include <QFile>

#include "source/Database/tools/querytools.h"

QSqlQuery SqlQueryConstructor::createAppConfigTable()
{
    auto query = loadQuery(":/sql/queries/version2/createappconfigtable.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::createGroupsTable()
{
    auto query = loadQuery(":/sql/queries/version2/creategroupstable.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::createTermsTable()
{
    auto query = loadQuery(":/sql/queries/version2/createtermstable.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::dropTable(const QString& tableName)
{
    QSqlQuery dropTableQuery = loadQuery(":/sql/queries/common/droptable.sql");
    dropTableQuery.bindValue(":tableName", tableName);
    return dropTableQuery;
}

QSqlQuery SqlQueryConstructor::recordsCount(const QString& tableName)
{
    auto query = loadQuery(":/sql/queries/version2/recordscount.sql");
    query.bindValue(":tableName", tableName);
    return query;
}

QString SqlQueryConstructor::selectQuery(const QString&        tableName,
                                         const QStringList&    columns,
                                         const WhereCondition& where,
                                         const QString&        orderBy)
{
    QStringList qry;
    qry << "SELECT";
    qry << columns.join(QueryTools::joinParam);
    qry << "FROM";
    qry << tableName;
    if (where.getJoinedConditions() != "") {
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }
    if (orderBy.simplified() != "") {
        qry << "ORDER BY";
        qry << orderBy;
    }

    return qry.join(" ");
}

QSqlQuery SqlQueryConstructor::selectOneConfigParameter(const QString& parameter)
{
    auto query = loadQuery(":/sql/queries/version2/configparametercount.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryConstructor::selectOneGroup(const QUuid& groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/groupwithuuidcount.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectOneTerm(const QUuid& termUuid)
{
    auto query = loadQuery(":/sql/queries/version2/termwithuuidcount.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QString SqlQueryConstructor::insertQuery(const QString& tableName, const InsertContainer::List& values)
{
    QStringList columns;
    QStringList insertValues;

    for (const auto& value : values) {
        columns << value.getColumnName();
        insertValues << QueryTools::vv(value.getValue());
    }

    QStringList qry;
    qry << "INSERT INTO";
    qry << tableName;
    qry << "( " + columns.join(QueryTools::joinParam) + " )";
    qry << "VALUES";
    qry << "( " + insertValues.join(QueryTools::joinParam) + " )";

    return qry.join(" ");
}

QSqlQuery SqlQueryConstructor::insertConfigParameter(const QString& parameter, const QString& value)
{
    auto query = loadQuery(":/sql/queries/version2/insertconfigkeyvalue.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", value);
    return query;
}

QSqlQuery SqlQueryConstructor::insertGroup(const GroupInfoContainer& groupInfo)
{
    auto query = loadQuery(":/sql/queries/version2/insertgroup.sql");
    query.bindValue(":uuid", groupInfo.uuid.toString());
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryConstructor::updateConfigParameter(const QString& parameter, const QString& newValue)
{
    auto query = loadQuery(":/sql/queries/version2/updateconfigkeyvalue.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", newValue);
    return query;
}

QSqlQuery SqlQueryConstructor::updateTerm(const NodeInfoContainer& termInfo)
{
    auto query = loadQuery(":/sql/queries/version2/updateterm.sql");

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

QSqlQuery SqlQueryConstructor::updateGroup(const GroupInfoContainer& groupInfo)
{
    auto query = loadQuery(":/sql/queries/version2/updategroup.sql");
    query.bindValue(":uuid", groupInfo.uuid);
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryConstructor::deleteGroup(const QUuid& groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/deletegroupbyuuid.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::deleteTerm(const QUuid& termUuid)
{
    auto query = loadQuery(":/sql/queries/version2/deletetermbyuuid.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QString SqlQueryConstructor::loadQueryString(const QString& queryPath)
{
    QFile queryFile(queryPath);
    auto  open = queryFile.open(QIODevice::ReadOnly);
    assert(open);
    return QString(queryFile.readAll());
}

QSqlQuery SqlQueryConstructor::loadQuery(const QString& queryPath)
{
    QSqlQuery query;
    query.prepare(loadQueryString(queryPath));
    return query;
}
