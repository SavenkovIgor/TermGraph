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
#include "source/Database/dbtablenames.h"
#include "source/Database/sqlqueryconstructor.h"

NodeTable::NodeTable(QSqlDatabase* base)
    : TblBase(base)
{
    assert(base != nullptr);
}

QUuid NodeTable::nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const
{
    auto where = WhereCondition();
    where.equal(NodeColumn::term, name);
    where.equal(NodeColumn::groupUuid, groupUuid.toString());
    auto nodesRecords = toRecVector(select(NodeColumn::uuid, where));

    if (!nodesRecords.isEmpty()) {
        return QUuid(nodesRecords.first().value(NodeColumn::uuid).toString());
    }

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

    auto query = SqlQueryConstructor::insertTerm(termInfo);
    return startQuery(query);
}

void NodeTable::deleteTerm(const QUuid& termUuid)
{
    auto query = SqlQueryConstructor::deleteTerm(termUuid);
    startQuery(query);
}

const char* NodeTable::tableName() const { return TableName::NODES; }

TColumn NodeTable::primaryKey() const { return NodeColumn::uuid; }

void NodeTable::initTable()
{
    auto query = SqlQueryConstructor::createTermsTable();
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
    auto query = SqlQueryConstructor::selectOneTerm(uuid);
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
    UuidList ret;

    WhereCondition where; // Take all uuids

    if (!groupUuid.isNull()) // Take only uuids in specifig group
        where = WhereCondition::columnEqual(NodeColumn::groupUuid, groupUuid.toString());

    auto sqlRecords = toRecVector(select(NodeColumn::uuid, where));

    for (auto& record : sqlRecords) {
        if (!record.contains(NodeColumn::uuid))
            continue;

        ret.push_back(QUuid(record.value(NodeColumn::uuid).toString()));
    }

    return filterEmptyUuids(ret);
}

NodeInfoContainer NodeTable::getNode(const QUuid& uuid)
{
    assert(!uuid.isNull());

    NodeInfoContainer info;

    auto records = toRecVector(select(getAllColumns(), whereUuidEqual(uuid)));

    if (records.isEmpty())
        return info;

    auto record = records.first();
    return recordToNodeInfo(record);
}

NodeInfoContainer::List NodeTable::getAllNodesInfo(const QUuid& groupUuid)
{
    assert(!groupUuid.isNull());

    NodeInfoContainer::List ret;
    auto                    where   = WhereCondition::columnEqual(NodeColumn::groupUuid, groupUuid.toString());
    auto                    records = toRecVector(select(getAllColumns(), where));

    for (auto& record : records) {
        NodeInfoContainer info = recordToNodeInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

QDateTime NodeTable::getLastEdit(const QUuid& uuid)
{
    auto field = getStringField(NodeColumn::lastEdit, uuid.toString());

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

RecVector NodeTable::getAllLastEditRecords()
{
    auto columns = TColumn::List();
    columns << NodeColumn::groupUuid;
    columns << NodeColumn::lastEdit;
    return toRecVector(select(columns));
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

    auto query = SqlQueryConstructor::updateTerm(nodeContainer);
    startQuery(query);

    return true;
}

NodeInfoContainer NodeTable::recordToNodeInfo(QSqlRecord& record)
{
    NodeInfoContainer info;

    info.uuid        = QUuid(record.value(NodeColumn::uuid).toString());
    info.term        = QueryTools::unVV(record.value(NodeColumn::term).toString());
    info.definition  = QueryTools::unVV(record.value(NodeColumn::definition).toString());
    info.description = QueryTools::unVV(record.value(NodeColumn::description).toString());
    info.examples    = QueryTools::unVV(record.value(NodeColumn::examples).toString());
    info.wikiUrl     = QueryTools::unVV(record.value(NodeColumn::wikiUrl).toString());
    info.wikiImage   = QueryTools::unVV(record.value(NodeColumn::wikiImage).toString());
    info.groupUuid   = QUuid(record.value(NodeColumn::groupUuid).toString());
    info.lastEdit    = QDateTime::fromString(record.value(NodeColumn::lastEdit).toString(), Qt::ISODate);

    return info;
}

WhereCondition NodeTable::whereUuidEqual(const QUuid& uuid) { return primaryKeyEqual(uuid.toString()); }

bool NodeTable::nodeExist(const QUuid& nodeUuid) { return isUuidExist(nodeUuid); }
