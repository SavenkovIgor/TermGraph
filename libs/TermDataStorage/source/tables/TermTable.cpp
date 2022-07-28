// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/tables/TermTable.h"

#include "source/DbTools.h"
#include "source/SqlQueryBuilder.h"

void TermTable::initTable() { DbTools::start(SqlQueryBuilder().createTermsTable()); }

bool TermTable::exist(const TermUuid& uuid) const
{
    auto query = SqlQueryBuilder().selectOneTerm(uuid);
    DbTools::start(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

Result<TermData> TermTable::term(const TermUuid& uuid) const
{
    if (!exist(uuid))
        return ErrorCodes::TermUuidNotFound;

    auto query = SqlQueryBuilder().selectTerm(uuid);
    DbTools::start(query);

    auto record = DbTools::getRecord(std::move(query));
    return createTermData(record);
}

Result<TermData> TermTable::term(const QString& term, const GroupUuid& uuid) const
{
    if (term.simplified().isEmpty())
        return ErrorCodes::TermEmpty;

    auto query = SqlQueryBuilder().selectOneTerm(term, uuid);
    DbTools::start(query);

    if (query.next())
        return createTermData(query.record());

    return ErrorCodes::TermUuidNotFound;
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

Result<TermData> TermTable::addTerm(const TermData& info)
{
    // Generate new uuid if current is empty
    auto tUuid = info.uuid.value_or(generateNewUuid());

    if (exist(tUuid))
        return ErrorCodes::TermUuidAlreadyExist;

    if (info.term.simplified().isEmpty())
        return ErrorCodes::TermEmpty;

    if (term(info.term, info.groupUuid))
        return ErrorCodes::TermAlreadyExist;

    TermData termInfo = info;

    termInfo.uuid = tUuid;

    if (termInfo.lastEdit.isNull())
        termInfo.lastEdit = now();

    auto query = SqlQueryBuilder().insertTerm(termInfo);
    DbTools::start(query);

    return termInfo;
}

Result<TermData> TermTable::updateTerm(const TermData&                      info,
                                       DataStorageInterface::LastEditSource lastEditSource,
                                       bool                                 checkLastEdit)
{
    if (!info.uuid)
        return ErrorCodes::TermUuidInvalid;

    if (!exist(*info.uuid))
        return ErrorCodes::TermUuidNotFound;

    if (checkLastEdit) {
        const auto currentLastEdit = lastEdit(*info.uuid).value();
        const auto newLastEdit     = info.lastEdit;
        if (currentLastEdit > newLastEdit) // If db version is fresher, do nothing
            return ErrorCodes::NewerTermVersionFound;
    }

    TermData nodeContainer = info;

    if (lastEditSource == DataStorageInterface::AutoGenerate)
        nodeContainer.lastEdit = now();

    DbTools::start(SqlQueryBuilder().updateTerm(nodeContainer));

    return nodeContainer;
}

Result<TermData> TermTable::deleteTerm(const TermUuid& uuid)
{
    if (auto termData = term(uuid)) {
        DbTools::start(SqlQueryBuilder().deleteTerm(uuid));
        return termData.value();
    }

    return ErrorCodes::TermUuidNotFound;
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

Result<QDateTime> TermTable::lastEdit(const TermUuid& uuid)
{
    if (!exist(uuid))
        return ErrorCodes::TermUuidNotFound;

    auto query = SqlQueryBuilder().selectLastEdit(uuid);
    DbTools::start(query);

    auto record = DbTools::getRecord(std::move(query));

    auto field = record.value("lastEdit").toString();

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

TermData TermTable::createTermData(const QSqlRecord& record)
{
    auto gUuid = GroupUuid::create(record.value("groupUuid").toString());

    assert(gUuid);

    TermData info{
        .uuid        = TermUuid::create(record.value("uuid").toString()),
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
