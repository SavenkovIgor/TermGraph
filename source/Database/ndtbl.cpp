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

#include "source/Database/ndtbl.h"

#include "source/Database/columns/nodecolumn.h"
#include "source/Database/dbinfo.h"
#include "source/Database/sqlquerybuilder.h"

NodeTable::NodeTable(QSqlDatabase* base)
    : TblBase(base)
{
    assert(base != nullptr);
}

QUuid NodeTable::nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const
{
    auto query = SqlQueryBuilder().selectOneTerm(name, groupUuid);
    startQuery(query);

    auto nodesRecords = getAllRecords(std::move(query));

    if (nodesRecords.size() == 1)
        return QUuid(nodesRecords.first().value("uuid").toString());

    return QUuid();
}

bool NodeTable::addNode(const NodeInfoContainer& info)
{
    assert(!info.term.simplified().isEmpty());
    assert(!nodeExist(info.uuid));

    // Создать вершину не удалось
    if (info.term.simplified().isEmpty())
        return false;

    // This node already exist
    if (nodeExist(info.uuid))
        return false;

    NodeInfoContainer termInfo = info;

    // Generate new uuid if current is empty
    if (termInfo.uuid.isNull())
        termInfo.uuid = generateNewUuid();

    if (termInfo.lastEdit.isNull())
        termInfo.lastEdit = getLastEditNow();

    auto query = SqlQueryBuilder().insertTerm(termInfo);
    return startQuery(query);
}

void NodeTable::deleteTerm(const QUuid& termUuid)
{
    auto query = SqlQueryBuilder().deleteTerm(termUuid);
    startQuery(query);
}

const char* NodeTable::tableName() const { return TableName::NODES; }

TColumn NodeTable::primaryKey() const { return NodeColumn::uuid; }

void NodeTable::initTable()
{
    auto query = SqlQueryBuilder().createTermsTable();
    startQuery(query);
}

TColumn::List NodeTable::getAllColumns() const
{
    TColumn::List lst;

    for (const auto& column : NodeColumn::columns)
        lst << column;

    return lst;
}

bool NodeTable::isUuidExist(const QUuid& uuid)
{
    auto query = SqlQueryBuilder().selectOneTerm(uuid);
    startQuery(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

QUuid NodeTable::generateNewUuid()
{
    QUuid uuid;
    for (int i = 0; i < 1000; i++) {
        uuid = QUuid::createUuid();
        if (!isUuidExist(uuid)) {
            break;
        }
    }
    return uuid;
}

QDateTime NodeTable::getLastEditNow() { return QDateTime::currentDateTimeUtc(); }

QString NodeTable::getLastEditNowString() { return getLastEditNow().toString(Qt::ISODate); }

UuidList NodeTable::getAllNodesUuids(const QUuid& groupUuid)
{
    UuidList  ret;
    QSqlQuery query;

    if (groupUuid.isNull())
        query = SqlQueryBuilder().selectAllTermUuids();
    else
        query = SqlQueryBuilder().selectAllTermUuids(groupUuid);

    startQuery(query);

    auto sqlRecords = getAllRecords(std::move(query));

    for (auto& record : sqlRecords) {
        auto uuidStr = record.value("uuid").toString();
        ret.push_back(QUuid(uuidStr));
    }

    return ret;
}

NodeInfoContainer NodeTable::getNodeInfo(const QUuid& uuid)
{
    assert(!uuid.isNull());

    NodeInfoContainer info;

    auto query = SqlQueryBuilder().selectTerm(uuid);
    startQuery(query);

    auto record = getRecord(std::move(query));
    return recordToNodeInfo(record);
}

NodeInfoContainer::List NodeTable::getAllNodesInfo(const QUuid& groupUuid)
{
    assert(!groupUuid.isNull());

    NodeInfoContainer::List ret;

    auto qBuilder = SqlQueryBuilder(DbConnectionName::threadLoadingConnection);
    auto query    = qBuilder.selectAllTerms(groupUuid);
    startQuery(query);

    auto records = getAllRecords(std::move(query));

    for (auto& record : records) {
        NodeInfoContainer info = recordToNodeInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

QDateTime NodeTable::getLastEdit(const QUuid& uuid)
{
    auto query = SqlQueryBuilder().selectLastEdit(uuid);
    startQuery(query);

    auto record = getRecord(std::move(query));

    auto field = record.value("lastEdit").toString();

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

RecVector NodeTable::getAllLastEditRecords()
{
    auto query = SqlQueryBuilder().selectAllLastEditAndGroupUuid();
    startQuery(query);
    return getAllRecords(std::move(query));
}

bool NodeTable::updateNode(const NodeInfoContainer&             info,
                           DataStorageInterface::LastEditSource lastEditSource,
                           bool                                 checkLastEdit)
{
    assert(!info.uuid.isNull());
    assert(nodeExist(info.uuid));

    if (info.uuid.isNull())
        return false;

    if (!nodeExist(info.uuid))
        return false;

    if (checkLastEdit) {
        const auto currentLastEdit = getLastEdit(info.uuid);
        const auto newLastEdit     = info.lastEdit;
        if (currentLastEdit > newLastEdit) // If db version is fresher, do nothing
            return false;
    }

    NodeInfoContainer nodeContainer = info;

    if (lastEditSource == DataStorageInterface::AutoGenerate)
        nodeContainer.lastEdit = getLastEditNow();

    auto query = SqlQueryBuilder().updateTerm(nodeContainer);
    startQuery(query);

    return true;
}

NodeInfoContainer NodeTable::recordToNodeInfo(QSqlRecord& record)
{
    NodeInfoContainer info;

    info.uuid        = QUuid(record.value("uuid").toString());
    info.term        = record.value("term").toString();
    info.definition  = record.value("definition").toString();
    info.description = record.value("description").toString();
    info.examples    = record.value("examples").toString();
    info.wikiUrl     = record.value("wikiUrl").toString();
    info.wikiImage   = record.value("wikiImage").toString();
    info.groupUuid   = QUuid(record.value("groupUuid").toString());
    info.lastEdit    = QDateTime::fromString(record.value("lastEdit").toString(), Qt::ISODate);

    return info;
}

WhereCondition NodeTable::whereUuidEqual(const QUuid& uuid) { return primaryKeyEqual(uuid.toString()); }

bool NodeTable::nodeExist(const QUuid& nodeUuid) { return isUuidExist(nodeUuid); }
