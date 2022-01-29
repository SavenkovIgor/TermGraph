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

void TermTable::initTable() { DbTools::start(SqlQueryBuilder().createTermsTable()); }

bool TermTable::exist(const TermUuid& uuid)
{
    auto query = SqlQueryBuilder().selectOneTerm(uuid);
    DbTools::start(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

Opt<TermUuid> TermTable::find(const QString& term, const GroupUuid& uuid) const
{
    if (term.simplified().isEmpty())
        return std::nullopt;

    auto query = SqlQueryBuilder().selectOneTerm(term, uuid);
    DbTools::start(query);

    auto nodesRecords = DbTools::getAllRecords(std::move(query));

    if (nodesRecords.size() == 1) {
        auto uuidStr = nodesRecords.front().value("uuid").toString();

        if (auto uuid = TermUuid::create(uuidStr))
            return *uuid;
    }

    return std::nullopt;
}

Result<TermData> TermTable::term(const TermUuid& uuid)
{
    if (!exist(uuid))
        return DbErrorCodes::TermUuidNotFound;

    auto query = SqlQueryBuilder().selectTerm(uuid);
    DbTools::start(query);

    auto record = DbTools::getRecord(std::move(query));
    return createTermData(record);
}

Result<QDateTime> TermTable::lastEdit(const TermUuid& uuid)
{
    if (!exist(uuid))
        return DbErrorCodes::TermUuidNotFound;

    auto query = SqlQueryBuilder().selectLastEdit(uuid);
    DbTools::start(query);

    auto record = DbTools::getRecord(std::move(query));

    auto field = record.value("lastEdit").toString();

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

TermUuid::List TermTable::allUuids(Opt<GroupUuid> uuid)
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

Result<TermData::List> TermTable::allTerms(const GroupUuid& uuid)
{
    TermData::List ret;

    auto query = SqlQueryBuilder().selectAllTerms(uuid);
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto& record : records) {
        TermData info = createTermData(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

TermTable::RecordList TermTable::allLastEditRecords()
{
    auto query = SqlQueryBuilder().selectAllLastEditAndGroupUuid();
    DbTools::start(query);
    return DbTools::getAllRecords(std::move(query));
}

Result<void> TermTable::addTerm(const TermData& info)
{
    auto tUuid = TermUuid::create(info.uuid).value_or(generateNewUuid());

    if (exist(tUuid))
        return DbErrorCodes::TermUuidAlreadyExist;

    if (info.term.simplified().isEmpty())
        return DbErrorCodes::TermEmpty;

    if (find(info.term, info.groupUuid))
        return DbErrorCodes::TermAlreadyExist;

    TermData termInfo = info;

    // Generate new uuid if current is empty
    if (termInfo.uuid.isNull())
        termInfo.uuid = generateNewUuid();

    if (termInfo.lastEdit.isNull())
        termInfo.lastEdit = now();

    auto query = SqlQueryBuilder().insertTerm(termInfo);
    DbTools::start(query);

    return outcome::success();
}

Result<void> TermTable::updateTerm(const TermData&                      info,
                                   DataStorageInterface::LastEditSource lastEditSource,
                                   bool                                 checkLastEdit)
{
    auto tUuid = TermUuid::create(info.uuid);

    if (!tUuid.has_value())
        return DbErrorCodes::TermUuidInvalid;

    if (!exist(*tUuid))
        return DbErrorCodes::TermUuidNotFound;

    if (checkLastEdit) {
        const auto currentLastEdit = lastEdit(*tUuid).value();
        const auto newLastEdit     = info.lastEdit;
        if (currentLastEdit > newLastEdit) // If db version is fresher, do nothing
            return DbErrorCodes::NewerTermVersionFound;
    }

    TermData nodeContainer = info;

    if (lastEditSource == DataStorageInterface::AutoGenerate)
        nodeContainer.lastEdit = now();

    DbTools::start(SqlQueryBuilder().updateTerm(nodeContainer));

    return outcome::success();
}

Result<void> TermTable::deleteTerm(const TermUuid& uuid)
{
    if (!exist(uuid))
        return DbErrorCodes::TermUuidNotFound;

    DbTools::start(SqlQueryBuilder().deleteTerm(uuid));
    return outcome::success();
}

TermUuid TermTable::generateNewUuid()
{
    for (;;) {
        auto uuid = TermUuid::generate();
        if (!exist(uuid))
            return uuid;
    }
}

QDateTime TermTable::now() { return QDateTime::currentDateTimeUtc(); }

TermData TermTable::createTermData(QSqlRecord& record)
{
    auto gUuid = GroupUuid::create(record.value("groupUuid").toString());

    assert(gUuid);

    TermData info{
        .uuid        = QUuid(record.value("uuid").toString()),
        .term        = record.value("term").toString(),
        .definition  = record.value("definition").toString(),
        .description = record.value("description").toString(),
        .examples    = record.value("examples").toString(),
        .wikiUrl     = record.value("wikiUrl").toString(),
        .wikiImage   = record.value("wikiImage").toString(),
        .groupUuid   = *gUuid,
        .lastEdit    = QDateTime::fromString(record.value("lastEdit").toString(), Qt::ISODate),
    };

    return info;
}
