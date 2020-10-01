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

QString SqlQueryConstructor::selectQuery(const QString &       tableName,
                                         const QStringList &   columns,
                                         const WhereCondition &where,
                                         const QString &       orderBy)
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

QSqlQuery SqlQueryConstructor::createAppConfigTable()
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/createappconfigtable.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::insertConfigParameter(const QString &parameter, const QString &value)
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/insertconfigkeyvalue.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", value);
    return query;
}

QSqlQuery SqlQueryConstructor::selectOneConfigParameter(const QString &parameter)
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/configparametercount.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryConstructor::selectConfigParameter(const QString &parameter)
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/selectparameter.sql");
    query.bindValue(":parameter", parameter);
    return query;
}

QSqlQuery SqlQueryConstructor::updateConfigParameter(const QString &parameter, const QString &newValue)
{
    auto query = loadQuery(":/sql/queries/version2/appConfig/updateconfigkeyvalue.sql");
    query.bindValue(":parameter", parameter);
    query.bindValue(":value", newValue);
    return query;
}

QSqlQuery SqlQueryConstructor::createGroupsTable()
{
    auto query = loadQuery(":/sql/queries/version2/groups/creategroupstable.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::insertGroup(const GroupInfoContainer &groupInfo)
{
    auto query = loadQuery(":/sql/queries/version2/groups/insertgroup.sql");
    query.bindValue(":uuid", groupInfo.uuid.toString());
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryConstructor::selectOneGroup(const QUuid &groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/groups/groupwithuuidcount.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::updateGroup(const GroupInfoContainer &groupInfo)
{
    auto query = loadQuery(":/sql/queries/version2/groups/updategroup.sql");
    query.bindValue(":uuid", groupInfo.uuid);
    query.bindValue(":name", groupInfo.name);
    query.bindValue(":comment", groupInfo.comment);
    return query;
}

QSqlQuery SqlQueryConstructor::deleteGroup(const QUuid &groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/groups/deletegroupbyuuid.sql");
    query.bindValue(":uuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::createTermsTable()
{
    auto query = loadQuery(":/sql/queries/version2/terms/createtermstable.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::insertTerm(const NodeInfoContainer &termInfo)
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

QSqlQuery SqlQueryConstructor::selectTerm(const QUuid &termUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/selecttermbyuuid.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectOneTerm(const QUuid &termUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/termwithuuidcount.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectOneTerm(const QString &term, const QUuid &groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/selecttermwithnameingroup.sql");
    query.bindValue(":term", term);
    query.bindValue(":groupUuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectAllUuids()
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectalluuids.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::selectAllUuids(const QUuid &groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectalluuidsingroup.sql");
    query.bindValue(":groupUuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectAllTerms(const QUuid &groupUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/selecttermsbygroupuuid.sql");
    query.bindValue(":groupUuid", groupUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectLastEdit(const QUuid &termUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectlastedit.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::selectAllLastEditAndGroupUuid()
{
    auto query = loadQuery(":/sql/queries/version2/terms/selectalllasteditandgroupuuid.sql");
    return query;
}

QSqlQuery SqlQueryConstructor::updateTerm(const NodeInfoContainer &termInfo)
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

QSqlQuery SqlQueryConstructor::deleteTerm(const QUuid &termUuid)
{
    auto query = loadQuery(":/sql/queries/version2/terms/deletetermbyuuid.sql");
    query.bindValue(":uuid", termUuid.toString());
    return query;
}

QSqlQuery SqlQueryConstructor::loadQuery(const QString &queryPath)
{
    QSqlQuery query;
    query.prepare(loadQueryString(queryPath));
    return query;
}

QSqlQuery SqlQueryConstructor::recordsCount(const QString &tableName)
{
    auto query = loadQuery(":/sql/queries/common/recordscount.sql");
    query.bindValue(":tableName", tableName);
    return query;
}

QSqlQuery SqlQueryConstructor::dropTable(const QString &tableName)
{
    QSqlQuery dropTableQuery = loadQuery(":/sql/queries/common/droptable.sql");
    dropTableQuery.bindValue(":tableName", tableName);
    return dropTableQuery;
}

QString SqlQueryConstructor::loadQueryString(const QString &queryPath)
{
    QFile queryFile(queryPath);
    auto  open = queryFile.open(QIODevice::ReadOnly);
    assert(open);
    return QString(queryFile.readAll());
}
