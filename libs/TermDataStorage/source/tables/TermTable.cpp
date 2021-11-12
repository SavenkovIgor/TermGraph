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

opt<TermUuid> TermTable::nodeUuidForNameAndGroup(const QString& name, const GroupUuid& uuid) const
{
    if (name.simplified().isEmpty())
        return std::nullopt;

    auto query = SqlQueryBuilder().selectOneTerm(name, uuid.get());
    DbTools::startQuery2(query);

    auto nodesRecords = DbTools::getAllRecords(std::move(query));

    if (nodesRecords.size() == 1) {
        auto uuidStr = nodesRecords.front().value("uuid").toString();

        if (auto uuid = TermUuid::create(uuidStr))
            return *uuid;
    }

    return std::nullopt;
}

result<void> TermTable::addNode(const TermData& info)
{
    if (info.uuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (nodeExist(info.uuid))
        return DbErrorCodes::UuidAlreadyExist;

    if (info.term.simplified().isEmpty())
        return DbErrorCodes::TermEmpty;

    TermData termInfo = info;

    // Generate new uuid if current is empty
    if (termInfo.uuid.isNull())
        termInfo.uuid = generateNewUuid();

    if (termInfo.lastEdit.isNull())
        termInfo.lastEdit = getLastEditNow();

    auto query = SqlQueryBuilder().insertTerm(termInfo);
    DbTools::startQuery2(query);

    return outcome::success();
}

result<void> TermTable::deleteTerm(const TermUuid& uuid)
{
    if (!nodeExist(uuid))
        return DbErrorCodes::UuidNotExist;

    auto query = SqlQueryBuilder().deleteTerm(uuid.get());
    DbTools::startQuery2(query);

    return outcome::success();
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

TermUuid::List TermTable::getAllNodesUuids(opt<GroupUuid> uuid)
{
    TermUuid::List ret;
    QSqlQuery      query;

    if (uuid)
        query = SqlQueryBuilder().selectAllTermUuids(*uuid);
    else
        query = SqlQueryBuilder().selectAllTermUuids();

    DbTools::startQuery2(query);

    auto sqlRecords = DbTools::getAllRecords(std::move(query));

    for (auto& record : sqlRecords) {
        auto uuidStr = record.value("uuid").toString();
        if (auto termUuid = TermUuid::create(uuidStr))
            ret.push_back(*termUuid);
    }

    return ret;
}

result<TermData> TermTable::getNodeInfo(const TermUuid& uuid)
{
    if (!nodeExist(uuid))
        return DbErrorCodes::UuidNotExist;

    TermData info;

    auto query = SqlQueryBuilder().selectTerm(uuid.get());
    DbTools::startQuery2(query);

    auto record = DbTools::getRecord(std::move(query));
    return recordToNodeInfo(record);
}

result<TermData::List> TermTable::getAllNodesInfo(const GroupUuid& uuid)
{
    TermData::List ret;

    auto query = SqlQueryBuilder().selectAllTerms(uuid.get());
    DbTools::startQuery2(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto& record : records) {
        TermData info = recordToNodeInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

result<QDateTime> TermTable::getLastEdit(const TermUuid& uuid)
{
    if (!nodeExist(uuid))
        return DbErrorCodes::UuidNotExist;

    auto query = SqlQueryBuilder().selectLastEdit(uuid);
    DbTools::startQuery2(query);

    auto record = DbTools::getRecord(std::move(query));

    auto field = record.value("lastEdit").toString();

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

RecordList TermTable::getAllLastEditRecords()
{
    auto query = SqlQueryBuilder().selectAllLastEditAndGroupUuid();
    DbTools::startQuery2(query);
    return DbTools::getAllRecords(std::move(query));
}

result<void> TermTable::updateNode(const TermData&                      info,
                                   DataStorageInterface::LastEditSource lastEditSource,
                                   bool                                 checkLastEdit)
{
    if (info.uuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!nodeExist(info.uuid))
        return DbErrorCodes::UuidNotExist;

    if (checkLastEdit) {
        const auto currentLastEdit = getLastEdit(*TermUuid::create(info.uuid)).value();
        const auto newLastEdit     = info.lastEdit;
        if (currentLastEdit > newLastEdit) // If db version is fresher, do nothing
            return DbErrorCodes::NewerTermVersionFound;
    }

    TermData nodeContainer = info;

    if (lastEditSource == DataStorageInterface::AutoGenerate)
        nodeContainer.lastEdit = getLastEditNow();

    auto query = SqlQueryBuilder().updateTerm(nodeContainer);
    DbTools::startQuery2(query);

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

bool TermTable::nodeExist(const QUuid& nodeUuid) { return isUuidExist(nodeUuid); }
