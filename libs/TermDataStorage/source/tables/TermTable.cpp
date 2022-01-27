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

#include "source/tables/TermTable.h"

#include "source/DbInfo.h"
#include "source/DbTools.h"
#include "source/SqlQueryBuilder.h"

Opt<TermUuid> TermTable::nodeUuidForNameAndGroup(const QString& name, const GroupUuid& uuid) const
{
    if (name.simplified().isEmpty())
        return std::nullopt;

    auto query = SqlQueryBuilder().selectOneTerm(name, uuid);
    DbTools::start(query);

    auto nodesRecords = DbTools::getAllRecords(std::move(query));

    if (nodesRecords.size() == 1) {
        auto uuidStr = nodesRecords.front().value("uuid").toString();

        if (auto uuid = TermUuid::create(uuidStr))
            return *uuid;
    }

    return std::nullopt;
}

Result<void> TermTable::addNode(const TermData& info)
{
    auto tUuid = TermUuid::create(info.uuid);

    if (!tUuid.has_value())
        return DbErrorCodes::TermUuidInvalid;

    if (nodeExist(*tUuid))
        return DbErrorCodes::TermUuidAlreadyExist;

    if (info.term.simplified().isEmpty())
        return DbErrorCodes::TermEmpty;

    TermData termInfo = info;

    // Generate new uuid if current is empty
    if (termInfo.uuid.isNull())
        termInfo.uuid = generateNewUuid();

    if (termInfo.lastEdit.isNull())
        termInfo.lastEdit = getLastEditNow();

    auto query = SqlQueryBuilder().insertTerm(termInfo);
    DbTools::start(query);

    return outcome::success();
}

Result<void> TermTable::deleteTerm(const TermUuid& uuid)
{
    if (!nodeExist(uuid))
        return DbErrorCodes::TermUuidNotFound;

    DbTools::start(SqlQueryBuilder().deleteTerm(uuid));
    return outcome::success();
}

void TermTable::initTable() { DbTools::start(SqlQueryBuilder().createTermsTable()); }

bool TermTable::isUuidExist(const TermUuid& uuid)
{
    auto query = SqlQueryBuilder().selectOneTerm(uuid);
    DbTools::start(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

TermUuid TermTable::generateNewUuid()
{
    for (;;) {
        auto uuid = TermUuid::generate();
        if (!isUuidExist(uuid))
            return uuid;
    }
}

QDateTime TermTable::getLastEditNow() { return QDateTime::currentDateTimeUtc(); }

TermUuid::List TermTable::getAllNodesUuids(Opt<GroupUuid> uuid)
{
    TermUuid::List ret;
    QSqlQuery      query;

    if (uuid)
        query = SqlQueryBuilder().selectAllTermUuids(*uuid);
    else
        query = SqlQueryBuilder().selectAllTermUuids();

    DbTools::start(query);

    auto sqlRecords = DbTools::getAllRecords(std::move(query));

    for (auto& record : sqlRecords) {
        auto uuidStr = record.value("uuid").toString();
        if (auto termUuid = TermUuid::create(uuidStr))
            ret.push_back(*termUuid);
    }

    return ret;
}

Result<TermData> TermTable::getNodeInfo(const TermUuid& uuid)
{
    if (!nodeExist(uuid))
        return DbErrorCodes::TermUuidNotFound;

    TermData info;

    auto query = SqlQueryBuilder().selectTerm(uuid);
    DbTools::start(query);

    auto record = DbTools::getRecord(std::move(query));
    return recordToNodeInfo(record);
}

Result<TermData::List> TermTable::getAllNodesInfo(const GroupUuid& uuid)
{
    TermData::List ret;

    auto query = SqlQueryBuilder().selectAllTerms(uuid);
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto& record : records) {
        TermData info = recordToNodeInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

Result<QDateTime> TermTable::getLastEdit(const TermUuid& uuid)
{
    if (!nodeExist(uuid))
        return DbErrorCodes::TermUuidNotFound;

    auto query = SqlQueryBuilder().selectLastEdit(uuid);
    DbTools::start(query);

    auto record = DbTools::getRecord(std::move(query));

    auto field = record.value("lastEdit").toString();

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

TermTable::RecordList TermTable::getAllLastEditRecords()
{
    auto query = SqlQueryBuilder().selectAllLastEditAndGroupUuid();
    DbTools::start(query);
    return DbTools::getAllRecords(std::move(query));
}

Result<void> TermTable::updateNode(const TermData&                      info,
                                   DataStorageInterface::LastEditSource lastEditSource,
                                   bool                                 checkLastEdit)
{
    auto tUuid = TermUuid::create(info.uuid);

    if (!tUuid.has_value())
        return DbErrorCodes::TermUuidInvalid;

    if (!nodeExist(*tUuid))
        return DbErrorCodes::TermUuidNotFound;

    if (checkLastEdit) {
        const auto currentLastEdit = getLastEdit(*tUuid).value();
        const auto newLastEdit     = info.lastEdit;
        if (currentLastEdit > newLastEdit) // If db version is fresher, do nothing
            return DbErrorCodes::NewerTermVersionFound;
    }

    TermData nodeContainer = info;

    if (lastEditSource == DataStorageInterface::AutoGenerate)
        nodeContainer.lastEdit = getLastEditNow();

    DbTools::start(SqlQueryBuilder().updateTerm(nodeContainer));

    return outcome::success();
}

TermData TermTable::recordToNodeInfo(QSqlRecord& record)
{
    TermData info;

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

bool TermTable::nodeExist(const TermUuid& uuid) { return isUuidExist(uuid); }
