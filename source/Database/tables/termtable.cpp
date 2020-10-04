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

#include "source/Database/tables/termtable.h"

#include "source/Database/dbinfo.h"
#include "source/Database/dbtools.h"
#include "source/Database/sqlquerybuilder.h"

QUuid TermTable::nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const
{
    auto query = SqlQueryBuilder().selectOneTerm(name, groupUuid);
    DbTools::startQuery2(query);

    auto nodesRecords = DbTools::getAllRecords(std::move(query));

    if (nodesRecords.size() == 1)
        return QUuid(nodesRecords.first().value("uuid").toString());

    return QUuid();
}

bool TermTable::addNode(const NodeInfoContainer& info)
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
    return DbTools::startQuery2(query);
}

void TermTable::deleteTerm(const QUuid& termUuid)
{
    auto query = SqlQueryBuilder().deleteTerm(termUuid);
    DbTools::startQuery2(query);
}

void TermTable::initTable()
{
    auto query = SqlQueryBuilder().createTermsTable();
    DbTools::startQuery2(query);
}

bool TermTable::isUuidExist(const QUuid& uuid)
{
    auto query = SqlQueryBuilder().selectOneTerm(uuid);
    DbTools::startQuery2(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

QUuid TermTable::generateNewUuid()
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

QDateTime TermTable::getLastEditNow() { return QDateTime::currentDateTimeUtc(); }

UuidList TermTable::getAllNodesUuids(const QUuid& groupUuid)
{
    UuidList  ret;
    QSqlQuery query;

    if (groupUuid.isNull())
        query = SqlQueryBuilder().selectAllTermUuids();
    else
        query = SqlQueryBuilder().selectAllTermUuids(groupUuid);

    DbTools::startQuery2(query);

    auto sqlRecords = DbTools::getAllRecords(std::move(query));

    for (auto& record : sqlRecords) {
        auto uuidStr = record.value("uuid").toString();
        ret.push_back(QUuid(uuidStr));
    }

    return ret;
}

NodeInfoContainer TermTable::getNodeInfo(const QUuid& uuid)
{
    assert(!uuid.isNull());

    NodeInfoContainer info;

    auto query = SqlQueryBuilder().selectTerm(uuid);
    DbTools::startQuery2(query);

    auto record = DbTools::getRecord(std::move(query));
    return recordToNodeInfo(record);
}

NodeInfoContainer::List TermTable::getAllNodesInfo(const QUuid& groupUuid)
{
    assert(!groupUuid.isNull());

    NodeInfoContainer::List ret;

    auto query = SqlQueryBuilder().selectAllTerms(groupUuid);
    DbTools::startQuery2(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto& record : records) {
        NodeInfoContainer info = recordToNodeInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

QDateTime TermTable::getLastEdit(const QUuid& uuid)
{
    auto query = SqlQueryBuilder().selectLastEdit(uuid);
    DbTools::startQuery2(query);

    auto record = DbTools::getRecord(std::move(query));

    auto field = record.value("lastEdit").toString();

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

RecVector TermTable::getAllLastEditRecords()
{
    auto query = SqlQueryBuilder().selectAllLastEditAndGroupUuid();
    DbTools::startQuery2(query);
    return DbTools::getAllRecords(std::move(query));
}

bool TermTable::updateNode(const NodeInfoContainer&             info,
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
    DbTools::startQuery2(query);

    return true;
}

NodeInfoContainer TermTable::recordToNodeInfo(QSqlRecord& record)
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

bool TermTable::nodeExist(const QUuid& nodeUuid) { return isUuidExist(nodeUuid); }
